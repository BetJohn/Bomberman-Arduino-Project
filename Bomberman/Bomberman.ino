#include <LiquidCrystal.h>
#include "LedControl.h"  // need the library
#include <EEPROM.h>
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 3;
const byte d6 = 5;
const byte d7 = 4;
const byte intensityPin = 6;
const byte dinPin = 12;    // pin 12 is connected to the MAX7219 pin 1
const byte clockPin = 11;  // pin 11 is connected to the CLK pin 13
const byte loadPin = 10;   // pin 10 is connected to LOAD pin 12
const int pinSW = A2;  // digital pin connected to switch output
const int pinX = A1;   // A1 - analog pin connected to X output
const int pinY = A0;   // A0 - analog pin connected to Y output
const int instructionsLength = 14;
const int resetLenght = 3;
const int highscoreLength = 3;
const int menuLength = 6;
const int aboutLenght = 7;
const int gameResultsLength = 10;
const int bombInterval = 100;
const int matrixSize = 16;
const int playerInterval = 300;
const long requiredHoldTime = 300;
byte matrixBrightness = 2;
byte swState = LOW;
byte lastSwState = LOW;
byte currentMatrix;
bool playerState = 0;
bool joyMoved = false;
bool gameON = false;
bool bombAlive = false;
bool gameOverShown = false;
bool gameOver = false;
char highscores[4][10] = { "ONE", "TWO", "THREE", "" };
char playerName[15];
char letter = 'A';
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  //DIN, CLK, LOAD, No. DRIVER
const char *menuOptions[] = { "Start Game", "Highscore", "LCD Brightness", "About", "How to play", "Reset Highscore", " " };
const char *reset[] = { "Reset scores?", "YES", "NO", " " };
const char *about[] = { "This game was", "made by Ioan", "In this lesson","materials were:", "LCD, JOYSTICK,", "Matrix, Wires", "and ARDUINO UNO", "" };
const char *instructions[] = { "Clear the whole", "terrain so you", "can escape...", "THE MATRIX!!!", "Use the button", "to place bombs","You have 3 lives", "Swipe LEFT to", "back/reset", "And RIGHT for", "confirmation","When type  name","go after 'Z' to", "START GAME", "" };
const char *gameResults[] = { "Game Over!", "Score: ", "Congratulations", "You`ve beaten:", "players", "Next time will", "be much better", "You can check", "Highscore from", "main menu", " " };
const int rowDirections[] = { -1, 0, 1, 0 };
const int colDirections[] = { 0, 1, 0, -1 };
int *scores[] = { 100, 70, 20, 0 };
int wallsAlive;
int intensity;
int resetPosition;
int option;
int lives = 3;
int minThreshold = 100;
int maxThreshold = 900;
int xValue = 0;
int yValue = 0;
int bombState = 0;
int memoryAddress;
int menuPosition;
int highscorePosition = 0;
int aboutPosition = 0;
int instructionsPosition = 0;
int resetLength = 3;
int beatenPlayers = 0;
int resultsPosition = 0;
long lastMillisBomb;
long lastMillisPlayer;
long releaseTime;
long lastPressButton;
long buttonHoldTimer;
long bombBlinks;
long personalScore = 0;
byte heart[] = {
  B00000,
  B00000,
  B01010,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};
byte top[] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B01110,
  B11111,
  B00000,
  B00000
};
byte bulb[] = {
  B10101,
  B00000,
  B01110,
  B11111,
  B11111,
  B01110,
  B01110,
  B00100
};
byte questionMark[] = {
  B01110,
  B10001,
  B10001,
  B00010,
  B00100,
  B00100,
  B00000,
  B00100
};
byte rightSword[] = {
  B01000,
  B10000,
  B10000,
  B10000,
  B01000,
  B00101,
  B00010,
  B00101
};
byte leftSword[] = {
  B00100,
  B00010,
  B00001,
  B00001,
  B00010,
  B10100,
  B01000,
  B10100
};
struct Position {
  byte row;
  byte col;
  byte value;
} playerPosition, bombPosition;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int minim(int a, int b) {
  return a < b ? a : b;
}
int maxim(int a, int b) {
  return a > b ? a : b;
}
byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0 },
  { 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0 }
};
void sayWelcome() {
  lcd.setCursor(0, 0);  //col, row
  lcd.print("Welcome to..");
  lcd.setCursor(0, 1);
  lcd.print("BOMBERMAN!");
  delay(1000);
}
void getInformations() {
  memoryAddress = 0;
  EEPROM.get(400, intensity);
  for (int i = 0; i < 3; i++) {
    EEPROM.get(memoryAddress, highscores[i]);
    EEPROM.get(memoryAddress + 200, scores[i]);
    memoryAddress += 20;
  }
}
void setInformation() {
  memoryAddress = 0;
  for (int i = 0; i < 3; i++) {
    EEPROM.put(memoryAddress, highscores[i]);
    EEPROM.put(memoryAddress + 200, scores[i]);
    memoryAddress += 20;
  }
  EEPROM.put(400, intensity);
}
void lightUpMatrix() {
  for (int row = 0; row < matrixSize / 2; row++) {
    for (int col = 0; col < matrixSize / 2; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}
void setup() {
  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(6, heart);
  lcd.createChar(1, top);
  lcd.createChar(2, bulb);
  lcd.createChar(3, questionMark);
  lcd.createChar(4, rightSword);
  lcd.createChar(5, leftSword);
  lc.shutdown(0, false);                 // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness);  // sets brightness (0~15 possible values)
  lc.clearDisplay(0);                    // clear screen
  playerPosition.row = 3;
  playerPosition.col = 3;
  playerPosition.value = 2;
  pinMode(intensityPin, OUTPUT);
  setInformation();
  getInformations();
  pinMode(pinSW, INPUT_PULLUP);
  sayWelcome();
  delay(1000);
  printMenu();
  playerName[0] = '\0';
  wallsAlive = countWalls();
  currentMatrix = 0;
}
int countWalls() {
  int nr = 0;
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (matrix[row][col]) {
        nr++;
      }
    }
  }
  return nr;
}
void updateGame();

void destroyWalls(Position bombPosition) {
  bool alreadyDied = false;
  for (int i = 0; i < 4; i++) {
    if (fieldExists({ bombPosition.row + rowDirections[i], bombPosition.col + colDirections[i], matrix[bombPosition.row + rowDirections[i]][bombPosition.col + colDirections[i]] })) {
      if (matrix[bombPosition.row + rowDirections[i]][bombPosition.col + colDirections[i]] == 1) {
        wallsAlive--;
        personalScore += 10;
        matrix[bombPosition.row + rowDirections[i]][bombPosition.col + colDirections[i]] = 0;
        lightOFF(bombPosition.row + rowDirections[i], bombPosition.col + colDirections[i]);
        updateGame();
      }
    }
    if ((bombPosition.row + rowDirections[i] == playerPosition.row && bombPosition.col + colDirections[i] == playerPosition.col) || (bombPosition.row == playerPosition.row && bombPosition.col == playerPosition.col)) {
      if (alreadyDied == false) {
        lives--;
        alreadyDied = true;
      }
      updateGame();
      if (lives == 0) {
        gameOver = true;
      }
      break;
    }
  }
}
void printResetHighscore() {
  lcd.clear();
  for (int i = 0; i < 2; i++) {
    int startIndex = resetPosition + i;
    lcd.setCursor(0, i);
    lcd.print(reset[startIndex]);
  }
  lcd.setCursor(15, 0);
  lcd.blink();
}

void executeMove(int direction) {
  /*
  direction: 0 - UP , 1- RIGHT, 2 - DOWN, 3 - LEFT 
  option : 0 mainMenu, 1 print HighSCORES, 2
  */
  switch (option) {
    case 50:
      {
        if (direction == 0) {  // UP
          printGameResults();
          resultsPosition = maxim(0, resultsPosition - 1);
        } else if (direction == 2) {  // DOWN
          resultsPosition = minim(gameResultsLength - 1, resultsPosition + 1);
          printGameResults();
        }
        break;
      }
    case 0:
      {
        if (direction == 0) {  // UP
          printMenu();
          menuPosition = maxim(0, menuPosition - 1);
        } else if (direction == 2) {  // DOWN
          menuPosition = minim(menuLength - 1, menuPosition + 1);
          printMenu();
        }
        break;
      }
    case 1:
      {
        if (direction == 0) {  // UP
          highscorePosition = maxim(0, highscorePosition - 1);
          printHighScore();
        } else if (direction == 2) {  // DOWN
          highscorePosition = minim(highscoreLength - 1, highscorePosition + 1);
          printHighScore();
        } else if (direction == 1) {  // RIGHT
          printHighScore();
        } else {
          highscorePosition = 0;
        }
        break;
      }
    case 2:
      {
        if (direction == 0) {  // UP
          intensity = minim(255, intensity + 10);
          EEPROM.put(400, intensity);
          analogWrite(intensityPin, intensity);
          printIntensity();
        } else if (direction == 2) {  // DOWN
          intensity = maxim(0, intensity - 10);
          EEPROM.put(400, intensity);
          analogWrite(intensityPin, intensity);
          printIntensity();
        } else if (direction == 1) {  //RIGHT
          printIntensity();
        }
        break;
      }
    case 3:
      {
        if (direction == 0) {  // UP
          aboutPosition = maxim(0, aboutPosition - 1);
          printAbout();
        } else if (direction == 2) {  // DOWN
          aboutPosition = minim(aboutLenght - 1, aboutPosition + 1);
          printAbout();
        } else if (direction == 1) {  // RIGHT
          printAbout();
        } else {
          aboutPosition = 0;
        }
        break;
      }
    case 4:
      {
        if (direction == 0) {  // UP
          instructionsPosition = maxim(0, instructionsPosition - 1);
          printHowToPlay();
        } else if (direction == 2) {  // DOWN
          instructionsPosition = minim(instructionsLength - 1, instructionsPosition + 1);
          printHowToPlay();
        } else if (direction == 1) {  // RIGHT
          printHowToPlay();
        } else {
          instructionsPosition = 0;
        }
        break;
      }
    case 5:
      {
        if (direction == 0) {  // UP
          resetPosition = maxim(0, resetPosition - 1);
          printResetHighscore();
        } else if (direction == 2) {  // DOWN
          resetPosition = minim(resetLength - 1, resetPosition + 1);
          printResetHighscore();
        } else if (direction == 1 && resetPosition == 1) {  // RIGHT
          resetHighScore();
          option = 0;
          menuPosition = 0;
          printMenu();
        } else {
          resetPosition = 0;
        }
        break;
      }
    case 100:
      {
        if (direction == 0) {  // UP
          letter = 'A' < --letter ? letter : 'A';
          createName();
        } else if (direction == 2) {  // DOWN
          letter = ++letter;
          if (letter > 'Z') {
            playerName[strlen(playerName) + 1] = '\0';
            option = 1000;
            startGame();
          } else {
            createName();
          }
        } else if (direction == 1) {  //RIGHT
          if (!gameON) {
            createName();
            gameON = true;
          } else {
            playerName[strlen(playerName)] = letter;
            playerName[strlen(playerName) + 1] = '\0';
            letter = 'A';
            createName();
          }
        }
        break;
      }
  }
}

int calculateRealRow(int row) {
  int realRow;
  switch (currentMatrix) {
    case 0:
      {
        realRow = row;
        break;
      }
    case 1:
      {
        realRow = row;
        break;
      }
    case 2:
      {
        realRow = row - 8;
        break;
      }
    case 3:
      {
        realRow = row - 8;
        break;
      }
  }
  return realRow;
}
int calculateRealCol(int col) {
  int realCol;
  switch (currentMatrix) {
    case 0:
      {
        realCol = col;
        break;
      }
    case 1:
      {
        realCol = col - 8;
        break;
      }
    case 2:
      {
        realCol = col - 8;
        break;
      }
    case 3:
      {
        realCol = col;
        break;
      }
  }
  return realCol;
}
void blinkPlayer() {
  long currentMillis = millis();
  if (currentMillis - lastMillisPlayer > playerInterval) {
    if (playerState) {
      playerState = 0;
    } else {
      playerState = 1;
    }
    int realRow, realCol;
    realRow = calculateRealRow(playerPosition.row);
    realCol = calculateRealCol(playerPosition.col);
    matrix[playerPosition.row][playerPosition.col] = playerState;
    lc.setLed(0, realRow, realCol, playerState);
    lastMillisPlayer = currentMillis;
  }
}
void printIntensity() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(intensity);
  lcd.blink();
}

void randomMap() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      matrix[row][col] = random(100) / 50;
    }
  }
  playerPosition.row = random(8);
  playerPosition.col = random(8);
}
void moveJoyStick() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  Serial.print("X:");
  Serial.print(xValue);
  Serial.print(" | Y:");
  Serial.println(yValue);
  if (xValue < minThreshold && joyMoved == false) {
    Serial.println("Moved LEFT");
    if (option == 100) {
      for (int i = 0; i < 15; i++)
        playerName[i] = '\0';  // reset name
      gameON = false;
      letter = 'A';
      randomMap();
    }
    aboutPosition = 0;
    lives = 3;
    personalScore = 0;
    option = 0;
    resultsPosition = 0;
    menuPosition = 0;
    instructionsPosition = 0;
    executeMove(0);
    joyMoved = true;
  }
  if (xValue > maxThreshold && joyMoved == false) {
    Serial.println("Moved RIGHT");
    executeMove(1);
    joyMoved = true;
  }
  if (yValue < minThreshold && joyMoved == false) {
    Serial.println("Moved DOWN");
    executeMove(2);
    joyMoved = true;
  }
  if (yValue > maxThreshold && joyMoved == false) {
    Serial.println("Moved UP");
    executeMove(0);
    joyMoved = true;
  }
  if (xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }
  swState = digitalRead(pinSW);
  if (swState != lastSwState) {
    if (swState == LOW) {  // button is pressed
      buttonHoldTimer = millis();
      lastPressButton = buttonHoldTimer;
    } else {  // button is released
      Serial.println("Button was released!");
      releaseTime = millis();
      Serial.print("Button was pressed at:");
      Serial.println(lastPressButton);
      Serial.print("Button was released at:");
      Serial.println(releaseTime);
      if (releaseTime - lastPressButton > requiredHoldTime && releaseTime > 4000) {

        if (menuPosition == 0) {
          option = 100;
        } else {
          option = menuPosition;
        }
        if (menuPosition == 5) {
          executeMove(0);
        } else {
          executeMove(1);
        }
      }
    }
  }
  lastSwState = swState;
}
bool fieldExists(Position nextPosition) {
  if (nextPosition.row < 0 || nextPosition.col < 0 || nextPosition.row > 15 || nextPosition.col > 15)
    return false;
  return true;
}
bool moveAvailable(Position nextPosition) {
  if (fieldExists(nextPosition) && nextPosition.value == 0)
    return true;
  return false;
}
void updateLedsMatrix() {
  switch (currentMatrix) {
    case 0:
      {
        for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
            lc.setLed(0, i, j, matrix[i][j]);
          }
        }
        break;
      }
    case 1:
      {
        for (int i = 0; i < 8; i++) {
          for (int j = 8; j < 15; j++) {
            lc.setLed(0, i, j - 8, matrix[i][j]);
          }
        }
        break;
      }
    case 2:
      {
        for (int i = 8; i < 15; i++) {
          for (int j = 8; j < 15; j++) {
            lc.setLed(0, i - 8, j - 8, matrix[i][j]);
          }
        }
        break;
      }
    case 3:
      {
        for (int i = 8; i < 15; i++) {
          for (int j = 0; j < 8; j++) {
            lc.setLed(0, i - 8, j, matrix[i][j]);
          }
        }
        break;
      }
  }
}
void checkMatrix(int row, int col) {
  int oldMatrix = currentMatrix;
  if (row < 8 && col < 8) {
    currentMatrix = 0;
  } else if (row < 8 && col >= 8) {
    currentMatrix = 1;
  } else if (row >= 8 && col >= 8) {
    currentMatrix = 2;
  } else if (row >= 8 && col < 8) {
    currentMatrix = 3;
  }
  if (oldMatrix != currentMatrix) {  // update led matrix
    Serial.println("Update");
    updateLedsMatrix();
  }
}
// void
void lightOFF(byte row, byte col) {
  int realRow, realCol;
  realRow = calculateRealRow(row);
  realCol = calculateRealCol(col);
  matrix[row][col] = 0;
  lc.setLed(0, realRow, realCol, 0);
}
void movePosition(int row, int col) {
  playerPosition.row = row;
  playerPosition.col = col;
  checkMatrix(row, col);
}

void playJoyStick() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  Serial.print("X:");
  Serial.print(xValue);
  Serial.print(" | Y:");
  Serial.println(yValue);
  if (xValue < minThreshold && joyMoved == false) {
    if (moveAvailable({ playerPosition.row, playerPosition.col - 1, matrix[playerPosition.row][playerPosition.col - 1] })) {  // it`s possible to move on left
      Serial.println("Moved LEFT");
      lightOFF(playerPosition.row, playerPosition.col);
      movePosition(playerPosition.row, playerPosition.col - 1);
    }
    joyMoved = true;
  }
  if (xValue > maxThreshold && joyMoved == false) {
    if (moveAvailable({ playerPosition.row, playerPosition.col + 1, matrix[playerPosition.row][playerPosition.col + 1] })) {  // it`s possible to move on right
      Serial.println("Moved RIGHT");
      lightOFF(playerPosition.row, playerPosition.col);
      movePosition(playerPosition.row, playerPosition.col + 1);
    }
    joyMoved = true;
  }
  if (yValue < minThreshold && joyMoved == false) {
    if (moveAvailable({ playerPosition.row + 1, playerPosition.col, matrix[playerPosition.row + 1][playerPosition.col] })) {  // it`s possible to move down
      Serial.println("Moved DOWN");
      lightOFF(playerPosition.row, playerPosition.col);
      movePosition(playerPosition.row + 1, playerPosition.col);
    }
    joyMoved = true;
  }
  if (yValue > maxThreshold && joyMoved == false) {
    if (moveAvailable({ playerPosition.row - 1, playerPosition.col, matrix[playerPosition.row - 1][playerPosition.col] })) {  // it`s possible to move up
      Serial.println("Moved UP");
      lightOFF(playerPosition.row, playerPosition.col);
      movePosition(playerPosition.row - 1, playerPosition.col);
    }
    joyMoved = true;
  }
  if (xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }
  swState = digitalRead(pinSW);
  if (swState != lastSwState) {
    if (swState == LOW) {  // button is pressed
      buttonHoldTimer = millis();
      lastPressButton = buttonHoldTimer;
    } else {  // button is released
      Serial.println("Button was released!");
      releaseTime = millis();
      Serial.print("Button was pressed at:");
      Serial.println(lastPressButton);
      Serial.print("Button was released at:");
      Serial.println(releaseTime);
      if (releaseTime - lastPressButton > requiredHoldTime) {
        Serial.println("Button was hold! Bomb has been planted");
        // lightOFF(bombPosition.row, bombPosition.col);
        bombPosition.col = playerPosition.col;
        bombPosition.row = playerPosition.row;
        bombAlive = true;
        bombBlinks = 0;
      }
    }
  }
  lastSwState = swState;
}
void printMenu() {
  lcd.clear();
  for (int i = 0; i < 2; i++) {
    int startIndex = menuPosition + i;
    lcd.setCursor(0, i);
    lcd.print(menuOptions[startIndex]);
    if (startIndex == 0) {
      lcd.print("   ");
      lcd.write(5);
      lcd.write(4);
    } else if (startIndex == 1) {
      lcd.print("     ");
      lcd.write(1);
    } else if (startIndex == 2) {
      lcd.write(2);
    } else if (startIndex == 3) {
      lcd.print("         ");
      lcd.write(3);
    } else if (startIndex == 4) {
      lcd.print("   ");
      lcd.write(3);
    }
  }
  lcd.setCursor(15, 0);
  lcd.blink();
}
void blinkBomb() {
  long currentMillis = millis();
  if (currentMillis - lastMillisBomb > bombInterval) {
    bombBlinks++;
    if (bombState) {
      bombState = 0;
    } else {
      bombState = 1;
    }
    lastMillisBomb = currentMillis;
    int realRow, realCol;
    realRow = calculateRealRow(bombPosition.row);
    realCol = calculateRealCol(bombPosition.col);
    matrix[bombPosition.row][bombPosition.col] = bombState;
    lc.setLed(0, realRow, realCol, bombState);
  }
}
void printHighScore() {
  lcd.clear();
  for (int i = 0; i < 2; i++) {
    int startIndex = highscorePosition + i;
    lcd.setCursor(0, i);
    lcd.print(highscores[startIndex]);
  }
  lcd.setCursor(15, 0);
  lcd.blink();
}
void resetHighScore() {
  const char defaultNames[3][10] = {"Player 1", "Player 2", "Player 3"};
  for (int i = 0; i < 3; i++) {
    scores[i] = 0;
    strcpy(highscores[i], defaultNames[i]);
    Serial.print(highscores[i]);
  }
}

void printAbout() {
  lcd.clear();
  for (int i = 0; i < 2; i++) {
    int startIndex = aboutPosition + i;
    lcd.setCursor(0, i);
    lcd.print(about[startIndex]);
  }
  lcd.setCursor(15, 0);
  lcd.blink();
}
void printHowToPlay() {
  lcd.clear();
  for (int i = 0; i < 2; i++) {
    int startIndex = instructionsPosition + i;
    lcd.setCursor(0, i);
    lcd.print(instructions[startIndex]);
  }
  lcd.setCursor(15, 0);
  lcd.blink();
}
void createName() {
  lcd.clear();
  if (strlen(playerName)) {
    lcd.print(playerName);
  }
  lcd.print(letter);
  lcd.setCursor(strlen(playerName), 1);
  lcd.blink();
}
void updateGame() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.print(personalScore);
  lcd.setCursor(0, 1);
  lcd.print(playerName);
  lcd.print(": ");
  lcd.write(6);
  lcd.print(lives);
}
void startGame() {
  randomMap();
  lightUpMatrix();
  updateGame();
  option = 1000;
}
void checkhighScore() {
  option = 50;
  gameOver = false;
  gameOverShown = false;
  for (int i = 0; i < highscoreLength; i++) {
    if (scores[i] < personalScore) {
      beatenPlayers = highscoreLength - i;
      updateHighScore();
      setInformation();
      break;
    }
  }
  printGameResults();
}
void updateHighScore() {
  for (int i = highscoreLength - 1; i > highscoreLength - beatenPlayers; i--) {
    strcpy(highscores[i], highscores[i - 1]);
    scores[i] = scores[i - 1];
  }

  strcpy(highscores[highscoreLength - beatenPlayers], playerName);
  scores[highscoreLength - beatenPlayers] = personalScore;

  strcpy(highscores[highscoreLength], " ");
  scores[highscoreLength] = 0;
}
void printGameResults() {
  lcd.clear();
  for (int i = 0; i < 2; i++) {
    int startIndex = resultsPosition + i;
    if (resultsPosition == 3) {
      if (beatenPlayers) {
        lcd.print(beatenPlayers);
      } else {
        lcd.print("0");
      }
    }
    lcd.setCursor(0, i);
    lcd.print(gameResults[startIndex]);
    if (startIndex == 1) {
      lcd.print(personalScore);
    }
  }
  lcd.setCursor(15, 0);
  lcd.blink();
}
void loop() {
  if (option == 1000) {
    if (gameOver == false) {
      blinkPlayer();
      playJoyStick();
      if (bombAlive && bombBlinks < 30) {
        blinkBomb();
      } else if (bombAlive && bombBlinks == 30) {
        destroyWalls(bombPosition);
        bombAlive = false;
      }
      if (wallsAlive <= 0) {
        gameOver = true;
      }
    } else {
      if (gameOverShown == false) {
        for (int row = 0; row < matrixSize / 2; row++) {
          for (int col = 0; col < matrixSize / 2; col++) {
            lc.setLed(0, row, col, true);  // turns on LED at col, row
            delay(25);
          }
        }
        for (int row = 0; row < matrixSize / 2; row++) {
          for (int col = 0; col < matrixSize / 2; col++) {
            lc.setLed(0, row, col, false);  // turns off LED at col, row
            delay(25);
          }
          gameOverShown = true;
        }
        Serial.println("Game Over!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Game Over!");
        checkhighScore();
      }
    }
  } else {
    moveJoyStick();
  }
}