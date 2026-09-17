Railway Platform Display and Train Announcement System

📌 Overview

This is an embedded railway platform display and train announcement system developed as part of Embedded Systems training at Vector India Institute.

The system uses RTC-based time management to monitor train arrival schedules and displays train information on a 16×2 LCD. Train name and destination are scrolled on the display, while LEDs and a buzzer provide status and announcement indications.

_________________________________________________________________________________________________________________________________________________________________________________

🎯 Objective

The main objective of this project is to develop an embedded railway information system that provides:

- Real-time time and date display
- Train arrival information
- Train departure information
- Platform information
- Train name and destination display
- Train announcement indication
- Train information modification through keypad
- LED and buzzer alerts

_________________________________________________________________________________________________________________________________________________________________________________

⚙️ Key Features

Feature                  Description
RTC Clock                Maintains and displays the current time
Date Display             Displays the current date and day
Train Information        Stores train number, name, destination and timings
Arrival Display          Displays the upcoming train arrival time
Departure Calculation    Calculates departure time using arrival and travel time
Platform Display         Displays the assigned platform number
Scrolling Display        Scrolls train name and destination on the LCD
Train Announcement       Detects a train arriving one minute ahead
Keypad Menu              Allows administrator interaction
External Interrupt       Enters admin/modification mode
Train Modification       Allows arrival time and platform modification
Date Modification        Allows date modification
Time Modification        Allows RTC time modification
LEDs                     Indicates normal, announcement and admin modes
Buzzer                   Provides an audible announcement indication
LCD                      Displays system and train information

_________________________________________________________________________________________________________________________________________________________________________________

🧩 System Architecture

Inputs

- RTC
- 4×4 Matrix Keypad
- External Interrupt Button

Processing

- Train schedule checking
- Arrival time comparison
- Departure time calculation
- Train information management
- Admin mode processing

Outputs

- 16×2 LCD
- Orange LED
- Green LED
- Red LED
- Buzzer

Working Flow

RTC → Time Checking → Train Arrival Detection → LCD Display → LED/Buzzer Indication

_________________________________________________________________________________________________________________________________________________________________________________

🔌 Hardware Connections

=> LCD Interface

The 16×2 LCD uses an 8-bit data interface.
| LCD Signal | LPC21xx Connection |
|------------|--------------------|
| D0         | P0.8               |
| D1         | P0.9               |
| D2         | P0.10              |
| D3         | P0.11              |
| D4         | P0.12              |
| D5         | P0.13              |
| D6         | P0.14              |
| D7         | P0.15              |
| RS         | P0.16              |
| EN         | P0.17              |

The LCD is connected to the controller through the existing LCD interface used in the project.

=> 4×4 Matrix Keypad

The keypad uses four row lines and four column lines.

Keypad Signal	LPC21xx Connection

| Keypad Signal | LPC21xx Connection |
|---------------|--------------------|
| Row 1         | P1.16              |
| Row 2         | P1.17              |
| Row 3         | P1.18              |
| Row 4         | P1.19              |
| Column 1      | P1.20              |
| Column 2      | P1.21              |
| Column 3      | P1.22              |
| Column 4      | P1.23              |

The keypad is used to enter train numbers, time, date and platform information.

=> Peripheral Pin Connections
| Device | LPC21xx Pin |
|---|---|
| External Interrupt (EINT0) | P0.1 |
| Buzzer | P0.18 |
| Orange LED | P0.19 |
| Green LED | P0.20 |
| Red LED | P0.21 |

=> Control and Indicator Connections

Device                    Pin       Function
External Interrupt        P0.1      Enters Admin Mode
Orange LED                P0.19     Normal Mode
Green LED                 P0.20     Train Announcement
Red LED                   P0.21     Admin/Modification Mode
Buzzer                    Final      Train announcement / alert

_________________________________________________________________________________________________________________________________________________________________________________

🖥️ Display Information

The LCD displays:

- Current time
- Current date
- Day of the week
- Train number
- Platform number
- Arrival time
- Train name
- Destination
- Departure time
- Admin menu information

Example:

11019 P1 A:03:40
Kornak Express - Machilipatnam
D:05:02

_________________________________________________________________________________________________________________________________________________________________________________

🔄 Working Principle

1. The system initializes the RTC, LCD, keypad and external interrupt.
2. The RTC continuously maintains the current time and date.
3. Train information is stored in the train database.
4. The system compares the current RTC time with the stored train arrival times.
5. When a train is one minute away from arrival, the train information is displayed.
6. The train name and destination scroll on the second line of the LCD.
7. The departure time is displayed along with the scrolling information.
8. The green LED and buzzer indicate the train announcement.
9. The external interrupt is used to enter Admin Mode.
10. In Admin Mode, train arrival time, platform, date and RTC time can be modified.
11. The red LED indicates Admin/Modification Mode.
12. After exiting Admin Mode, the system returns to Normal Mode and the orange LED is activated.

_________________________________________________________________________________________________________________________________________________________________________________

🧱 Software Modules

Module                    Responsibility
main.c                    Main program and system control
train.c                   Train checking and train display functions
train.h                   Train structure and function declarations
lcd.c / lcd.h              LCD interface
KPM.c / KPM.h              Keypad scanning and numeric input
delay.c / delay.h          Delay functions
types.h                   Data type definitions

_________________________________________________________________________________________________________________________________________________________________________________

🛠️ Technologies Used

| Category | Technology |
|---|---|
| Microcontroller | LPC21xx ARM7 |
| Programming Language | Embedded C |
| Development Tool | Keil µVision |
| Simulation | Proteus |
| Display | 16×2 LCD |
| Input | 4×4 Matrix Keypad |
| Time Management | Internal RTC |
| Interrupt | External Interrupt EINT0 |
| Indicators | LEDs + Buzzer |

_________________________________________________________________________________________________________________________________________________________________________________

📊 Project Demonstration

The project demonstration includes:

- RTC time and date display
- Normal mode indication
- Train arrival detection
- Train information display
- Train name and destination scrolling
- Departure time display
- Green LED indication
- Buzzer announcement
- Admin mode
- Train time modification
- Platform modification
- Date modification
- RTC time modification

_________________________________________________________________________________________________________________________________________________________________________________

📷 Project Diagrams
Block Diagram

The system block diagram showing the relationship between the LPC21xx controller, RTC, keypad, LCD, buttons, buzzer and LEDs will be available here:
<img width="1536" height="960" alt="block diagram" src="https://github.com/user-attachments/assets/d1757002-9d75-462d-aa8d-61aa72c82241" />


Circuit Diagram
The following proteus schematic shows the hardware connections between the LPC21xx microcontroller and the connected peripherals.
<img width="1856" height="1326" alt="Screenshot 2026-09-17 111239" src="https://github.com/user-attachments/assets/5f6b6298-8daf-4653-9890-095949470997" />


_________________________________________________________________________________________________________________________________________________________________________________

🚀 Future Improvements

Possible future improvements include:

- More train entries
- Larger display
- Voice-based announcements
- EEPROM-based train data storage
- Automatic platform information management
- Improved user interface

_________________________________________________________________________________________________________________________________________________________________________________

🎓 Learning Outcomes

This project provided practical experience with:

- Embedded C programming
- ARM7/LPC21xx programming
- GPIO configuration
- RTC programming
- LCD interfacing
- Matrix keypad interfacing
- External interrupt handling
- LED control
- Buzzer control
- Structures and arrays in Embedded C
- Modular programming
- Hardware and software integration
- Proteus simulation
- Keil development environment

_________________________________________________________________________________________________________________________________________________________________________________

🎓 Training

Training Institute: Vector India Institute

Course: Embedded Systems

Project Type: Training Project

This project was provided as part of the Embedded Systems training at Vector India Institute.
