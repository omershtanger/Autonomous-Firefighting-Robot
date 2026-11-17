# Autonomous Firefighting Robot

An autonomous four wheel drive robot that detects a flame in its surroundings, drives toward it, and puts it out with a water pump. No remote control is used: detection and suppression happen in real time.

<!-- Add a photo or GIF of the robot here, e.g. ![Robot](images/robot.jpg) -->

## Features

- **Flame detection:** IR flame sensors find the direction of the fire.
- **Autonomous navigation:** a 4WD chassis steers toward the flame source.
- **Fire suppression:** a servo sweeps the nozzle of a water pump to cover the flame.
- **Multi-component coordination:** sensors, motor drivers, the servo and the pump run together from one control loop.

## Hardware

| Component | Role |
|---|---|
| Arduino microcontroller | Main controller |
| IR flame sensors | Detect the flame and its direction |
| Motor driver + 4 DC motors | 4WD drive |
| Servo motor | Aims the water nozzle |
| Water pump + reservoir | Suppression |
| Battery pack | Power supply |

<!-- Add a wiring diagram / pin table here -->

## How It Works

1. **Scan:** the robot reads all flame sensors continuously.
2. **Locate:** sensor readings are compared to work out where the flame is.
3. **Approach:** the motors steer toward the flame until it is within range.
4. **Suppress:** the pump turns on while the servo sweeps the nozzle across the flame.
5. **Verify:** once the sensors no longer detect a flame, the pump stops and the robot goes back to scanning.

## Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/omershtanger/Autonomous-Firefighting-Robot.git
   ```
2. Open the sketch in the Arduino IDE.
3. Wire the components according to the pin definitions at the top of the code.
4. Upload the code to the board and power the robot.

## Author

**Omer Shtanger**
