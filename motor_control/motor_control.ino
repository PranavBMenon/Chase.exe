/*
  L298N motor control over WiFi (ESP32)
  Web page: speed dial/slider, start/stop button, direction toggle.

  Wiring (ESP32 -> L298N):
    GPIO 25 -> ENA  (remove the ENA jumper on the L298N)
    GPIO 26 -> IN1
    GPIO 27 -> IN2
    GND     -> L298N GND (shared with motor supply ground)
  Motor on OUT1/OUT2, motor supply (6-12V) on L298N 12V terminal.

  Works with ESP32 Arduino core 2.x and 3.x.

  Speed mapping: slider 1-100% -> PWM 80-255 (motor won't turn below ~79).
  Slider 0% = off.
*/

#include <WiFi.h>
#include <WebServer.h>

const char* AP_SSID = "MotorControl";
const char* AP_PASS = "12345678";

const int ENA = 25;
const int IN1 = 26;
const int IN2 = 27;

const int PWM_FREQ = 1000;   // Hz
const int PWM_BITS = 8;      // 0-255
const int PWM_MIN  = 80;     // lowest duty at which the motor actually turns
const int PWM_MAX  = 255;

WebServer server(80);

bool running = false;
bool forward = true;
int speedPct = 50;           // 0-100

void pwmSetup() {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(ENA, PWM_FREQ, PWM_BITS);
#else
  ledcSetup(0, PWM_FREQ, PWM_BITS);
  ledcAttachPin(ENA, 0);
#endif
}

void pwmWrite(int duty) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(ENA, duty);
#else
  ledcWrite(0, duty);
#endif
}

void applyMotor() {
  if (!running || speedPct == 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    pwmWrite(0);
    return;
  }
  digitalWrite(IN1, forward ? HIGH : LOW);
  digitalWrite(IN2, forward ? LOW : HIGH);
  // 1..100 % -> 80..255 duty
  pwmWrite(map(speedPct, 1, 100, PWM_MIN, PWM_MAX));
}

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<title>Motor Control</title>
<style>
  :root {
    --bg: #0d1117; --card: #161b22; --line: #263040;
    --text: #e6edf3; --muted: #7d8a9a;
    --accent: #3b9eff; --go: #2ea043; --stop: #e5484d;
  }
  * { box-sizing: border-box; -webkit-tap-highlight-color: transparent; }
  body {
    margin: 0; min-height: 100vh; display: flex; align-items: center; justify-content: center;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
    background: radial-gradient(circle at 50% 0%, #162033 0%, var(--bg) 60%);
    color: var(--text); padding: 16px;
  }
  .card {
    width: 100%; max-width: 400px; background: var(--card); border: 1px solid var(--line);
    border-radius: 24px; padding: 24px; box-shadow: 0 20px 50px rgba(0,0,0,.45);
  }
  header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }
  h1 { font-size: 1.05rem; margin: 0; font-weight: 600; letter-spacing: .3px; }
  .badge {
    display: flex; align-items: center; gap: 8px; font-size: .8rem; color: var(--muted);
    background: #0d1117; border: 1px solid var(--line); padding: 6px 12px; border-radius: 999px;
  }
  .dot { width: 9px; height: 9px; border-radius: 50%; background: var(--muted); transition: .2s; }
  .badge.run .dot { background: var(--go); box-shadow: 0 0 10px var(--go); }
  .badge.err .dot { background: var(--stop); }

  .gauge { position: relative; width: 220px; height: 220px; margin: 16px auto 8px; }
  .gauge svg { width: 100%; height: 100%; transform: rotate(-90deg); }
  .gauge circle { fill: none; stroke-width: 14; stroke-linecap: round; }
  .gauge .bg { stroke: #202a38; }
  .gauge .fg { stroke: var(--accent); stroke-dasharray: 565.5; stroke-dashoffset: 565.5; transition: stroke-dashoffset .2s, stroke .3s; }
  .gauge.off .fg { stroke: #3a4658; }
  .center { position: absolute; inset: 0; display: flex; flex-direction: column; align-items: center; justify-content: center; }
  #speedVal { font-size: 3.4rem; font-weight: 700; line-height: 1; }
  #speedVal small { font-size: 1.2rem; color: var(--muted); font-weight: 500; }
  #arrow { font-size: 1.5rem; margin-top: 6px; color: var(--accent); transition: transform .3s; }
  #arrow.rev { transform: rotate(180deg); }

  input[type=range] {
    -webkit-appearance: none; appearance: none; width: 100%; height: 44px; background: transparent; margin: 8px 0 4px;
  }
  input[type=range]::-webkit-slider-runnable-track { height: 8px; border-radius: 8px; background: #202a38; }
  input[type=range]::-moz-range-track { height: 8px; border-radius: 8px; background: #202a38; }
  input[type=range]::-webkit-slider-thumb {
    -webkit-appearance: none; width: 30px; height: 30px; border-radius: 50%; background: #fff; margin-top: -11px;
    border: 4px solid var(--accent); box-shadow: 0 2px 8px rgba(0,0,0,.5);
  }
  input[type=range]::-moz-range-thumb {
    width: 22px; height: 22px; border-radius: 50%; background: #fff; border: 4px solid var(--accent);
  }
  .presets { display: flex; gap: 8px; margin: 4px 0 8px; }
  .presets button {
    flex: 1; margin: 0; padding: 10px 0; font-size: .85rem; background: #202a38; color: var(--muted);
    border-radius: 10px;
  }
  .presets button:active { background: #2b384b; color: var(--text); }

  button {
    width: 100%; border: 0; border-radius: 14px; color: #fff; cursor: pointer; font-weight: 600;
    transition: transform .08s, background .2s;
  }
  button:active { transform: scale(.97); }
  #toggle { padding: 18px; font-size: 1.15rem; margin-top: 12px; background: var(--go); letter-spacing: 1px; }
  #toggle.on { background: var(--stop); }
  #dir { padding: 14px; font-size: 1rem; margin-top: 12px; background: #263040; }
</style>
</head>
<body>
<div class="card">
  <header>
    <h1>Motor Control</h1>
    <div class="badge" id="badge"><span class="dot"></span><span id="status">Stopped</span></div>
  </header>

  <div class="gauge off" id="gauge">
    <svg viewBox="0 0 200 200">
      <circle class="bg" cx="100" cy="100" r="90"/>
      <circle class="fg" id="ring" cx="100" cy="100" r="90"/>
    </svg>
    <div class="center">
      <div id="speedVal">50<small>%</small></div>
      <div id="arrow">&#10148;</div>
    </div>
  </div>

  <input type="range" id="speed" min="0" max="100" value="50">
  <div class="presets">
    <button data-v="0">0</button>
    <button data-v="25">25</button>
    <button data-v="50">50</button>
    <button data-v="75">75</button>
    <button data-v="100">100</button>
  </div>

  <button id="toggle">START</button>
  <button id="dir">&#8646; Direction: Forward</button>
</div>

<script>
const $ = id => document.getElementById(id);
const speed = $('speed'), speedVal = $('speedVal'), toggle = $('toggle'), dir = $('dir');
const ring = $('ring'), gauge = $('gauge'), badge = $('badge'), statusEl = $('status'), arrow = $('arrow');
const C = 565.5;
let timer;

function setSpeedUI(v) {
  speedVal.innerHTML = v + '<small>%</small>';
  ring.style.strokeDashoffset = C * (1 - v / 100);
}

function render(s) {
  speed.value = s.speed;
  setSpeedUI(s.speed);
  toggle.textContent = s.running ? 'STOP' : 'START';
  toggle.classList.toggle('on', s.running);
  dir.innerHTML = '&#8646; Direction: ' + (s.forward ? 'Forward' : 'Reverse');
  arrow.classList.toggle('rev', !s.forward);
  gauge.classList.toggle('off', !s.running);
  badge.className = 'badge' + (s.running ? ' run' : '');
  statusEl.textContent = s.running ? 'Running' : 'Stopped';
}

function call(url) {
  return fetch(url).then(r => r.json()).then(render).catch(() => {
    badge.className = 'badge err';
    statusEl.textContent = 'Connection lost';
  });
}

speed.oninput = () => {
  setSpeedUI(speed.value);
  clearTimeout(timer);
  timer = setTimeout(() => call('/speed?v=' + speed.value), 80);
};
document.querySelectorAll('.presets button').forEach(b => {
  b.onclick = () => call('/speed?v=' + b.dataset.v);
});
toggle.onclick = () => call('/toggle');
dir.onclick = () => call('/dir');
call('/state');
setInterval(() => call('/state'), 3000);
</script>
</body>
</html>
)rawliteral";

void sendState() {
  String json = "{\"speed\":" + String(speedPct) +
                ",\"running\":" + (running ? "true" : "false") +
                ",\"forward\":" + (forward ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pwmSetup();
  applyMotor();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("Connect to WiFi '");
  Serial.print(AP_SSID);
  Serial.print("' then open http://");
  Serial.println(WiFi.softAPIP());   // usually 192.168.4.1

  server.on("/", []() { server.send_P(200, "text/html", PAGE); });

  server.on("/state", sendState);

  server.on("/speed", []() {
    if (server.hasArg("v")) {
      speedPct = constrain(server.arg("v").toInt(), 0, 100);
      applyMotor();
    }
    sendState();
  });

  server.on("/toggle", []() {
    running = !running;
    applyMotor();
    sendState();
  });

  server.on("/dir", []() {
    // Stop briefly before reversing to protect the driver
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    pwmWrite(0);
    delay(150);
    forward = !forward;
    applyMotor();
    sendState();
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
