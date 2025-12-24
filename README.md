# Solar–Hydro Hybrid Renewable Energy System

This project implements a proof of concept **hybrid renewable energy harvesting system** that combines **solar tracking** and **hydroelectric generation** using **Arduino-based embedded systems**.  
The system adapts to environmental conditions in real time to maximise energy capture and protect hardware.

---

## System Overview

The system uses **two Arduino microcontrollers**:

- **Arduino 1 (Main Controller)**  
  Handles solar tracking, actuator control, sensor fusion, mode switching (solar ↔ hydro), and TFT display output.

- **Arduino 2 (Thermal Controller)**  
  Dedicated exclusively to the **AMG88xx 8×8 thermal camera** for **overheating detection**.

When overheating is detected, Arduino 2 sends a digital signal to Arduino 1, which reorients the solar panel away from the sun.

---

## Sensors & Actuators

- **LDRs** – Detect light intensity for single-axis solar tracking  
- **AMG88xx Thermal Camera** – Detects panel overheating  
- **Humidity Sensor (DHT11)** – Predicts rainfall to switch to hydro mode  
- **Accelerometer (MMA8451)** – Measures panel tilt and orientation  
- **Servo Motor** – Rotates the solar panel  
- **Hydroelectric Turbine (DC Motor)** – Generates power from collected rainwater  
- **TFT LCD Display** – Displays real-time system status  

---

## Repository Structure

```text
.
├── AMG88xxConnected/
│   └── Code for the secondary Arduino
│      (Thermal camera only – MUST be run first)
│
├── arduino1_fullCode/
│   └── Main Arduino code
│      (Solar tracking, sensors, actuators, display, control logic)
│
├── libraries/
│   └── All required Arduino libraries for this project
│
├── servoInitialPosition/
│   └── Utility script to move the servo to a known horizontal position
│      (Ensures correct initial conditions)

```
##  How to Run the System

**Important:**  
This project **can only be run with the correct electronic connections in place**.  
Ensure that all sensors, actuators, and both Arduino boards are wired **exactly as described in the project documentation** before uploading any code. Running the system without proper connections may lead to incorrect behaviour or hardware damage.

---

### Required Setup
- Two Arduino boards connected correctly
- All sensors and actuators wired and powered
- Required libraries installed (see `libraries/` folder)
- USB connection to each Arduino for code upload

---

### Initialise Servo Position (Required)

Upload and run the code in:

```text
servoInitialPosition
```
This moves the servo motor to a known horizontal reference position, ensuring correct and repeatable initial conditions before starting the system.

```text
AMG88xxConnected
```
This code runs on the second Arduino and handles the AMG88xx thermal camera.
It must be running before the main system code, as it continuously monitors temperature and sends overheating signals to the main Arduino.

```text
arduino1_fullCode
```
This starts the full system, including:
- Solar tracking using LDRs
- Overheating protection using thermal camera feedback
- Rain detection and automatic hydro mode switching
- Servo motor control
- Real-time system status display on the TFT screen
  
```text
libraries
```
This folder includes all the libraries needed to run this project.

