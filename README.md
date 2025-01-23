# Space Invaders Game:

This repository contains the code for a simple Space Invaders game implemented on an Arduino using a LiquidCrystal display, a joystick, and a buzzer. The game involves controlling a spaceship to shoot down moving enemies while avoiding collisions.

# Features:

Player Controls: Move the spaceship vertically using a joystick.
Shooting Mechanism: Fire bullets toward enemies using a fire button with cooldown.
Enemy Movement: Enemies move horizontally across the screen and alternate rows when reaching the edge.
Scoring: Gain points for each enemy hit by bullets.
Speed Increase: Enemy speed increases gradually over time.
Game Over: Ends when an enemy reaches the player's spaceship.

# Hardware Requirements:

- Arduino board (tested on Arduino Uno)
- LiquidCrystal library for Arduino
- Joystick module
- Buzzer

# Circuit Setup:
## LCD Wiring:

I have attatched the wiring diagram, as that would be easier for ya :).

# Joystick and Buzzer:

- A0: VRx
- A1: VRy
- GND: GND on Breadboard
- +5v: 5v on Breadboard
- SW: Pin 7
- (For the joystick, I have attatched an image to help to know what wires to use)
  
- Buzzer pin (negative side): Pin 8
- Buzzer pin (positive side): +5v on Breadboard
- (I have attatched an image to help)

# Gameplay
- Objective: Shoot down enemies moving across the screen.
- Controls: Move the spaceship up and down using the joystick. Press the fire button to shoot bullets.
- Score: Earn points for each enemy hit by bullets. The game ends when an enemy reaches your spaceship.
