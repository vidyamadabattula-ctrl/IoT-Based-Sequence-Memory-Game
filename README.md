# IoT-Based-Sequence-Memory-Game
Developed an IoT-based sequence memory game using Arduino UNO, LEDs, push buttons, and buzzer with automated scoring across 10 attempts.
# IoT-Based Sequence Memory Game

## Overview
An interactive memory game built using Arduino Uno and ESP32. Players repeat LED sequences using push buttons, and the game tracks their performance.

## Features
- Four push buttons and LEDs for sequence-based gameplay.
- Buzzer feedback for user interaction.
- Score calculation based on correct and incorrect attempts.
- ESP32 Wi-Fi connectivity for sending scores to ThingSpeak.
- Cloud-based score monitoring and visualization.

## Hardware Components
- Arduino Uno
- ESP32 development board
- Four push buttons
- Four LEDs and suitable current-limiting resistors
- Buzzer
- Jumper wires and breadboard

## Project Structure
- `Arduino_Uno_Game/` — memory game and score calculation.
- `ESP32_ThingSpeak/` — Wi-Fi communication and cloud data upload.
- `images/` — project photos and flowchart.

## How It Works
1. The Arduino generates a sequence for the player to memorize.
2. The player presses the corresponding buttons.
3. The Arduino checks the input and calculates the score.
4. The ESP32 sends the score to ThingSpeak over Wi-Fi.

## Setup
1. Open each sketch in Arduino IDE.
2. Select the appropriate board and COM port.
3. Install the required libraries.
4. Configure Wi-Fi credentials and ThingSpeak settings locally.
5. Upload the sketches to the respective boards and follow the serial monitor output.

## Author
Vidya Sri Madabattula
