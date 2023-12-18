# Arduino Bomberman Game :bomb: :video_game: :joystick: :computer:

[Setup]: [Photo]("https://github.com/BetJohn/Bomberman-Arduino-Project/blob/main/Setup.jpg")

## Description :page_facing_up:

The Arduino Bomberman Game is a classic Bomberman-style game implemented on an Arduino Uno microcontroller. It uses various components such as a joystick, a matrix LED display, and an LCD screen to create an interactive gaming experience. The game involves navigating through a matrix, destroying walls, and strategically placing bombs to clear the terrain.

## Features :sparkles:

- Classic Bomberman gameplay.
- Randomly generated terrain for each game.
- Highscore tracking.
- LCD display for game information.
- Joystick control for player movement.

## Hardware Requirements :computer:

- Arduino Uno microcontroller
- Joystick module
- Matrix LED display (e.g., MAX7219)
- LCD screen (16x2)
- Various resistors, wires, and buttons

## Installation :wrench:

1. Clone this repository to your local machine.
   git clone https://github.com/your-username/arduino-bomberman.git
2. Connect the Arduino Uno to your computer.

3. Upload the provided Arduino sketch (bomberman.ino) to your Arduino board using the Arduino IDE.

4. Follow the wiring instructions provided in the sketch comments to connect the components correctly.

5. Power on your Arduino and start playing the game!

## How to Play  :video_game:

1. Navigate through the matrix using the joystick.
2. Place bombs strategically to destroy walls.
3. Clear the entire terrain to progress to the next level.
4. Be careful not to get caught in bomb explosions.
5. Achieve the highest score possible.

## Controls :joystick:

- Joystick:
  - UP: Move Up
  - DOWN: Move Down
  - LEFT: Move Left
  - RIGHT: Move Right
  - BUTTON PRESS: Place Bomb

## Configuration :wrench:

- The game offers various configuration options, including LCD brightness and highscore reset.

## How it Works :bulb:

The game is implemented using the Arduino Uno microcontroller. The Arduino sketch (bomberman.ino) contains the code for the game logic, which is executed on the Arduino board. The game uses various components, including a joystick, a matrix LED display, and an LCD screen, to create an interactive gaming experience.

The menu logic: :page_with_curl:

- The menu is displayed on the LCD screen.
- The user can navigate through the menu using the joystick.
- The user can select an option by pressing the joystick button.
- The menu options include:
  - Start Game
  - Reset Highscore
  - Change LCD Brightness
  - About
  - How to Play
  - Reset Highscore

The game logic: :game_die:

- The game is displayed on the matrix LED display.
- The user can navigate through the matrix using the joystick.
- The user can place bombs by pressing the joystick button.
- The game ends when the player is caught in a bomb explosion or when the entire terrain is cleared.
- The game score is displayed on the LCD screen.
- After the game ends, the user can choose to play again or return to the menu.
- The game score is saved in the Arduino EEPROM memory and can be reset from the menu.

[Back to Top](#table-of-contents)