# Automatic Soldering Helper

## Project Idea

I want to make a small robot arm that can help me while soldering and working on electronics.

It will basically work like a motorised third hand. It can hold wires, PCBs and other small parts in position while I solder them.

I already have most of the parts so I should be able to make this without buying much.

The whole arm will rotate using a **28BYJ-48 stepper motor** and the rest of the arm will use the handful of **9g servos** I already have.

It will mainly be controlled using physical buttons.

## Planned Features

* 28BYJ-48 stepper motor rotating the whole arm
* Servo controlled shoulder
* Servo controlled elbow
* Servo controlled wrist
* Servo controlled clamp
* Physical control buttons
* 5 saved arm positions
* One button to move the arm back to its home position
* Fine movement buttons for adjusting the arm
* Interchangeable end attachments
* Emergency retract button

I won't add automatic clamping yet. I can add sensors for that later if I want.

The stepper motor will rotate the entire arm around the base.

The servos will only have to move the arm itself.

I will try to keep the arm sections short so the 9g servos don't have to lift too much weight.

Probably around **70-100mm per arm section** to start with.

## Controls

I want most controls to be physical buttons so I can use them while working.

Something like:

```
[ LEFT ] [ RIGHT ]

[ UP ]   [ DOWN ]

[ IN ]   [ OUT ]

[ OPEN CLAMP ]

[ CLOSE CLAMP ]

[ POSITION 1 ]
[ POSITION 2 ]
[ POSITION 3 ]
[ POSITION 4 ]
[ POSITION 5 ]

[ HOME ]

[ RETRACT ]
```

## Saved Positions

The controller will save different useful positions.

For example:

- Position 1
  - Wire soldering

- Position 2
  - PCB holding

- Position 3
  - Connector holding

- Position 4
  - Heatshrink / wire work

- Position 5
  - Custom position

The servo and stepper positions can be saved in memory.

Then pressing one button will make the arm return to that position.

## Home Position

The arm will also have a home position where it moves completely away from the work area.

Something like:

***Clamp opens -> Arm lifts -> Elbow retracts -> Base rotates away -> HOME***

This will also work as an emergency retract button if the arm gets too close to the soldering iron.

## Interchangeable Attachments

I want the end of the arm to be removable so I can make different attachments.

### Wire Clamp

Small padded jaws for holding two wires together.

### PCB Holder

A wider attachment for holding the edge of a small PCB.

### Solder Holder

An attachment that can hold solder wire close to where I am soldering.

### Small Part Holder

For connectors, switches, components and other small parts.

I can add more attachments later.

## Parts

| Part                       |          Qty | Notes                            |
| -------------------------- | -----------: | -------------------------------- |
| 28BYJ-48 stepper motor     |            1 | Rotates entire arm               |
| ULN2003 stepper board      |            1 | Drives 28BYJ-48                  |
| 9g servos                  |            4 | Shoulder, elbow, wrist and clamp |
| ESP32                      |            1 | Main controller                  |
| Push buttons               |       ~12-15 | Physical controls                |
| 5V power supply            |            1 | Servos + electronics             |
| Wires                      |      Various | Wiring                           |
| Screws                     |      Various | Arm assembly                     |
| 3D printed arm parts       |      Various | Main structure                   |
| Rubber / foam              | Small amount | Clamp padding                    |
| Scrap wood / printed base  |            1 | Bench mounting                   |
| USB cable                  |            1 | ESP32                            |
| Bearings                   |     Optional | Help support rotating base       |

Most of this should be stuff I already have.

## Power

The servos **wont be powered directly from the ESP32**.

I will use a separate 5V supply for:
- Servos
- 28BYJ / ULN2003
- ESP32

All of the grounds will be connected together.

## PCB

I don't think I need to make a custom PCB for the first version.
I might try a 3d printed pcb with a slot to insert cables.

There aren't that many parts and most of the wiring is just:

* servo connectors
* buttons
* ULN2003
* ESP32
* power

I can use:

**breadboard → test everything → perfboard**

I think making a PCB before I know the final arm design and number of buttons would probably just make more work.

## Next Steps

### Design

* [x] Find all of the 9g servos I have
* [x] Find the 28BYJ-48 and ULN2003 board
* [x] Decide which ESP32 to use - ESP32 Dev Board
* [ ] Measure the 9g servos
* [ ] Measure the 28BYJ-48 and ULN2003 board
* [ ] Decide the maximum reach of the arm
* [ ] Decide the arm section lengths
* [ ] Design the rotating base in CAD
* [ ] Design a bearing / centre support for the rotating base
* [ ] Design the stepper gear drive
* [ ] Design the shoulder joint
* [ ] Design the first arm section
* [ ] Design the elbow joint
* [ ] Design the second arm section
* [ ] Design the wrist
* [ ] Design the servo clamp
* [ ] Design a removable attachment system
* [ ] Design the wire holding attachment
* [ ] Design the PCB holding attachment
* [ ] Design the solder holding attachment
* [ ] Design the small part holding attachment
* [ ] Design the button/control enclosure
* [ ] Assemble everything in CAD and check for collisions
* [ ] Check that the arm can reach the useful area of the workbench
* [ ] Reduce unnecessary weight from the arm parts

### Electronics

* [ ] Decide how many physical buttons are needed
* [ ] Assign every button to an ESP32 GPIO
* [ ] Assign the four servos to GPIO pins
* [ ] Assign the ULN2003 inputs to GPIO pins
* [ ] Design the full wiring diagram
* [ ] Decide how the 5V power will be distributed
* [ ] Make sure the ESP32, servos and ULN2003 share a common ground
* [ ] Decide whether to use breadboard, perfboard or a 3D printed wiring board
* [ ] Design the electronics mounting area into the base

### Software

* [ ] Plan manual base left/right controls
* [ ] Plan shoulder up/down controls
* [ ] Plan elbow in/out controls
* [ ] Plan wrist controls
* [ ] Plan clamp open/close controls
* [ ] Plan fine movement mode
* [ ] Plan the HOME sequence
* [ ] Plan the emergency RETRACT sequence
* [ ] Plan how the five saved positions will work
* [ ] Decide how saved positions will be stored in ESP32 flash
* [ ] Write the basic firmware structure
* [ ] Write the servo movement functions
* [ ] Write the stepper movement functions
* [ ] Write the button handling code
* [ ] Write the saved-position system

### Grant / Build Preparation

* [ ] Make the complete BOM
* [ ] Mark all parts I already own
* [ ] Find prices and links for anything I still need
* [ ] Export screenshots/renders of the finished CAD
* [ ] Export STL files for all printed parts
* [ ] Make the final wiring diagram
* [ ] Finish the project README / journal
* [ ] Apply for the grant

### After Parts / Grant

* [ ] Test all of the servos
* [ ] Test the 28BYJ-48
* [ ] Print the first prototype
* [ ] Assemble the rotating base
* [ ] Assemble the arm
* [ ] Test the arm with no load
* [ ] Check if the servos can handle the designed arm length
* [ ] Adjust the CAD if the arm flexes or the servos struggle
* [ ] Wire the physical controls
* [ ] Upload and test the firmware
* [ ] Calibrate the servo limits
* [ ] Calibrate the saved positions
* [ ] Test the wire holder
* [ ] Test the PCB holder
* [ ] Test it while actually soldering
* [ ] Make improvements based on real use

