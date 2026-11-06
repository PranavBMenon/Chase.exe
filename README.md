# chase.exe

**Human detected. Personal space not found.**

A camera bogey made from an old inkjet printer. It moves a phone along the printer's rail, with the eventual goal of tracking people automatically.

The printer used to move a printhead around. Now it moves a camera around. We think that's a reasonable career change.

## What is this?

`chase.exe` is a motorized camera slider built by taking apart an old inkjet printer and reusing its linear rail and motor.

The idea is to make a camera that can follow a person instead of just sitting in one place. We're using an ESP32 to control the motor, and a laptop for the computer vision part.

## Built with

- ESP32
- L298N motor driver
- Linear rail and DC motor salvaged from an inkjet printer
- Smartphone camera
- Python and OpenCV
- YOLO for person detection
- Wi-Fi communication

## How it works

1. The phone streams video to a laptop.
2. The laptop processes the video and detects people.
3. The detection is used to decide which direction the camera should move.
4. The ESP32 controls the motor through the L298N.
5. The camera moves along the printer rail.

Basically, a printer has been repurposed to chase humans.

## Why?

Because throwing away an entire printer just because it ran out of ink felt like a waste.

We wanted to see how much of an ordinary printer could be reused for something completely different. Turns out, the rail and motor make a pretty decent starting point for a camera slider.

## Current status

- [x] Salvaged printer rail and motor
- [x] Motorized camera movement
- [x] ESP32 motor control
- [ ] Person detection
- [ ] Automatic human tracking
- [ ] Smoother movement and better tracking

Some features are still being worked on. This is a hackathon project, not a finished product.

## Future plans

- Make tracking less jittery
- Add limits so the camera doesn't drive itself off the rail
- Improve movement speed and accuracy
- Make the camera follow people more reliably

## The name

`chase.exe`

We thought it was funny. The printer disagrees.

---

Built during a hackathon using parts that probably weren't supposed to do this.