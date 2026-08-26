# Automatic Soldering Helper

A small robot arm that can help hold wires, PCBs and other parts while I'm soldering.

I wanted to make something like a motorised helping hand that I can move around with buttons instead of having to keep adjusting normal helping hands every time.

I already had most of the motors and electronics so I decided to try make the arm mostly out of parts I already had.

## How It Works

The whole arm rotates around the base using a **28BYJ-48 stepper motor** and a **ULN2003 driver**.

The rest of the arm uses 4 small 9g servos:

- Shoulder
- Elbow
- Wrist
- Clamp

I originally planned on using a gear ratio for the base because I thought the stepper might struggle with the weight of the arm.

After printing most of the arm and seeing how light it actually was I decided to remove the whole gear ratio part and just attach the motor directly to the arm. This also makes the base way simpler and smaller.

## Testing

Before buying the final electronics I printed everything for the arm except the final motor base and connected it to an Arduino Uno just to confirm the idea would actually work.

I only have 3 potentiometers so for testing I used them to control the servos and switched between the wrist and clamp when needed.

Kinda surprisingly it worked pretty well.

There is a small bit of electrical noise making the servos vibrate but I'm adding a 1000uF capacitor to the final electronics which should help with that.

The Arduino is only being used for testing. The final version will use an ESP32.

## Clamp

I printed a small basic clamp for the first test just to make sure the servo could actually open and close something.

After that I redesigned it into a bigger clamp taking inspiration from some videos I saw on Instagram.

The bigger one should be much more useful for actually holding wires, connectors and small PCBs so I think I'll use that for the final version.

The clamp is controlled by another 9g servo.

## Controls

I wanted the final arm to use physical controls instead of needing a computer every time I want to move it.

I'm using:

- 3 potentiometers
- 6 momentary buttons

The potentiometers can be used to manually position the shoulder, elbow and wrist.

The buttons can then be used for things like:

- Saved positions
- Moving the base
- Opening and closing the clamp
- Home
- Changing modes

The ESP32 can save arm positions so I can move the arm somewhere useful and then return to that position later without manually adjusting everything again.

## Saved Positions

A saved position can store:

- Base position
- Shoulder angle
- Elbow angle
- Wrist angle
- Clamp position

This means I could have different positions for stuff like holding wires or holding a PCB.

The positions will be stored on the ESP32 so they don't disappear every time it loses power.

## Base

The base uses the 28BYJ-48 stepper motor I already had.

I originally designed a larger base with printed gears to increase the torque but after actually printing the arm it was much lighter than I expected.

Because of that I decided to remove the gears and just drive the rotating part directly from the stepper.

There will also be a limit switch so the arm can find its home position when it starts.

## Table Mount

For attaching the whole thing to my desk I'm going to reuse the clamp from my old helping hands.

It already works well for attaching stuff to the edge of a table so there isn't much point designing another clamp.

The helping hands clamp will just slot into a mount on the bottom of the arm.

## Electronics

The final controller will be an **ESP32 CH340C USB-C development board**.

The servos aren't going to be powered directly from the ESP32.

They will use the 5V power rail and all of the grounds will be connected together.

I'm also adding a 1000uF capacitor across the servo power to help with the small amount of electrical noise I noticed while testing.

The stepper motor will be controlled through its ULN2003 board.

## PCB

I don't really need a normal manufactured PCB for this project so I'm making a 3D printed one instead.

The PCB will basically have slots and paths where I can insert the wires to keep everything organised.

I redesigned the PCB after working out the final electronics and made sure to export the traces in the STEP this time because I forgot to last time and ended up drawing them by hand in Fusion.

The ESP32 and other electronics can then mount onto this inside the case.

## Firmware

The final firmware will run on the ESP32.

Instead of using GRBL I decided to make the position system specifically for the arm since only the base uses a stepper and everything else uses servos.

The firmware will handle:

- Manual movement
- Servo control
- Stepper control
- Clamp control
- Saved positions
- Base homing
- Smooth movement
- Physical buttons
- Potentiometer controls

This should make it pretty easy to use while I'm actually soldering instead of having to control it from my computer.

## BOM

| Item | Qty | Status | Price | Notes |
| --- | ---: | --- | ---: | --- |
| [ESP32-CH340C-Type-C](https://www.aliexpress.com/item/1005008723399881.html) | 1 | Buy | $5.03 | Main controller |
| [1000uF 50V Electrolytic Capacitors](https://www.aliexpress.com/item/1005005920255833.html) | 5 | Buy | $2.22 | Servo power smoothing |
| [M2 Hex Socket Button Head Screw Kit](https://www.aliexpress.com/item/1005007278965396.html) | 1 | Buy | $6.99 | Arm assembly |
| [DS228/DS428 12mm Green Momentary Pushbuttons](https://www.aliexpress.com/item/1005008558481882.html) | 6 | Buy | $3.25 | Physical controls |
| 28BYJ-48 5V Stepper Motor | 1 | Owned | $0.00 | Rotates the arm |
| ULN2003 Stepper Driver Board | 1 | Owned | $0.00 | Drives the stepper |
| 9g Servo | 4 | Owned | $0.00 | Shoulder, elbow, wrist and clamp |
| 10k Potentiometer | 3 | Owned | $0.00 | Manual arm controls |
| Limit Switch | 1 | Owned | $0.00 | Base homing |
| 10k Resistor | 1 | Owned | $0.00 | Limit switch pull-up |

### Order Cost

| | Cost |
| --- | ---: |
| Subtotal | $17.56 |
| Savings | -$0.19 |
| Shipping | $0.00 |
| Estimated import charges | $16.93 |
| **Total** | **$34.30** |

## Current Progress

So far I've:

- Designed the arm in CAD
- Printed most of the arm
- Designed the clamp
- Tested the servos with an Arduino Uno
- Confirmed the arm can actually move
- Simplified the rotating base
- Planned the ESP32 controls
- Planned the saved position system
- Designed the 3D printed PCB
- Worked out the final BOM

The main thing now is getting the final parts and putting everything together properly.

## Images

| Schematic | PCB | Case |
| --- | --- | --- |
| ![Schematic](https://github.com/user-attachments/assets/202e61b8-0b45-475a-8b2f-f29bcf091b24) | ![PCB](https://github.com/user-attachments/assets/575f32cb-42c5-48bb-a548-4b77e24fa8b1) | ![Case](https://github.com/user-attachments/assets/3e57d8ae-40e9-44c5-b045-906e9adf2c27) |
