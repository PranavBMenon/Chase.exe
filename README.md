# chase.exe

**A camera bogey built from a printer that gave up on printing.**

`chase.exe` is a motorized camera bogey made by repurposing the linear rail and motor from an old inkjet printer. Instead of moving a printhead across paper, it moves a phone along a rail.

The goal was pretty simple: take something that would probably end up as e-waste and turn it into something useful (and hopefully a little cooler than a printer).

## What went into it?

- Linear rail and carriage from an inkjet printer
- Original printer DC motor
- ESP32
- L298N motor driver
- Smartphone as the camera
- Laptop for control and experimentation

## How it works

The printer's original rail and motor handle the movement. An ESP32 controls the motor through an L298N driver, allowing the camera to move along the rail.

The phone sits on the carriage and acts as the camera.

What used to move ink around now moves a camera around. That's pretty much the entire idea.

## Why a printer?

Printers have some surprisingly useful parts inside them. The linear rail was already designed for controlled movement, so instead of building a mechanism from scratch, we decided to reuse it.

A bit of dismantling, wiring, and experimenting later, we had a working camera bogey.

## Current status

- [x] Printer rail and carriage repurposed
- [x] Original motor reused
- [x] ESP32 motor control
- [x] Camera movement along the rail
- [ ] Further movement and control improvements

## Ideas for later

- Automated camera movement
- Computer vision-based tracking
- Better speed control
- Smoother movement

These are ideas for future improvements, not features the current build necessarily has.

## The name

`chase.exe`

The camera might chase something eventually. For now, it's just chasing its own potential.

---

Made during a hackathon.  
One less printer in the landfill. One more questionable use of an ESP32.