# chase.exe 🏃📸
### `Human detected. Personal space not found.`

A human-tracking camera bogey built during a 12-hour hackathon using salvaged inkjet printer components, an ESP32, and a questionable amount of ambition.

Why hire a cameraman when you can run an executable?

---

## 💀 What is chase.exe?

**chase.exe** is a motorized camera slider designed to detect and follow human movement.

We took the linear rail and DC motor from an old inkjet printer, attached a smartphone, and turned a printer carriage into a camera bogey.

The goal? Build an affordable, intelligent camera system that automatically follows a person along a linear track.

What used to chase ink across paper now chases people.

---

## ⚙️ Hardware

| Component | Purpose |
|---|---|
| ESP32 | Embedded control |
| L298N | DC motor driver |
| Inkjet printer linear rail | Camera movement |
| Original printer DC motor | Bogey propulsion |
| Smartphone | Camera feed |
| Laptop | Computer vision processing |

## 🧠 Tech Stack

- Python
- OpenCV
- YOLO for human detection
- ESP32
- Wi-Fi communication

## 🔄 How it works

1. A smartphone mounted on the bogey captures video.
2. The video feed is streamed to a laptop.
3. Computer vision detects humans in the frame.
4. The person's position is calculated relative to the frame center.
5. Movement commands are sent to the ESP32.
6. The ESP32 controls the DC motor through the L298N.
7. The camera moves along the rail to follow the detected subject.

## ♻️ E-Waste? More like Free Hardware.

Instead of buying a dedicated camera slider, we repurposed components from an inkjet printer:

- Linear rail
- Carriage
- DC motor

**Printer:** 0  
**Cameraman:** 0  
**chase.exe:** Running...

## 🚧 Project Status

- [x] Salvaged printer rail and carriage
- [x] Camera bogey assembly
- [x] DC motor integration
- [x] ESP32 motor control
- [ ] Live human detection
- [ ] Autonomous tracking
- [ ] Smooth motion control

## 🔮 Future Plans

- Real-time human tracking
- Smooth acceleration and deceleration
- Wireless control interface
- Autonomous cinematic camera movement
- Obstacle detection
- Multiple tracking modes

## 👨‍💻 Built During

A 12-hour hackathon.

Built with recycled hardware, caffeine, and the inability to leave a printer alone.

## 📜 License

MIT License
