#include <SoftwareSerial.h>

// =========================
// PINS
// =========================

const int BTN_PINS[] = {2, 3, 4, 5};
const int LED_PINS[] = {10, 11, 12, 13};

const int NUM_PINS = 4;
const int BUZZER = 7;

// UNO → ESP32
// RX = D9
// TX = D6
SoftwareSerial espSerial(9, 6);

// =========================
// GAME SETTINGS
// =========================

const int MAX_SEQ_LEN = 9;
const int TOTAL_ATTEMPTS = 10;

// =========================
// GAME VARIABLES
// =========================

int sequenceArr[MAX_SEQ_LEN];
int seqLen = 0;

int correctCount = 0;
int wrongCount = 0;
int attempts = 0;

// =========================
// FUNCTION DECLARATIONS
// =========================

void turnOffAllLEDs();
void startNewGame();
void addRandomStep();
void showSequence();
bool isPlayerRight();
int waitForButtonPress();
void gameOverAnimation();
void winningAnimation();
void showScoreCard();

// =========================
// SETUP
// =========================

void setup()
{
  Serial.begin(9600);

  // Start communication with ESP32
  espSerial.begin(9600);

  // Buttons and LEDs
  for (int i = 0; i < NUM_PINS; i++)
  {
    pinMode(BTN_PINS[i], INPUT_PULLUP);

    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  // Buzzer
  pinMode(BUZZER, OUTPUT);

  // Random sequence
  randomSeed(analogRead(A0));

  startNewGame();
}

// =========================
// MAIN LOOP
// =========================

void loop()
{
  // Show sequence to player
  showSequence();

  turnOffAllLEDs();

  // Check player's answer
  bool correct = isPlayerRight();

  turnOffAllLEDs();

  delay(200);

  // One attempt completed
  attempts++;

  if (correct)
  {
    correctCount++;

    Serial.println("Correct!");

    if (seqLen < MAX_SEQ_LEN)
    {
      addRandomStep();
    }
  }
  else
  {
    wrongCount++;

    Serial.println("Wrong!");

    gameOverAnimation();
  }

  // Display current score
  Serial.print("Attempt: ");
  Serial.print(attempts);
  Serial.print("/");
  Serial.println(TOTAL_ATTEMPTS);

  Serial.print("Correct: ");
  Serial.println(correctCount);

  Serial.print("Wrong: ");
  Serial.println(wrongCount);

  delay(500);

  // After 10 attempts
  if (attempts >= TOTAL_ATTEMPTS)
  {
    showScoreCard();

    delay(3000);

    startNewGame();
  }
}

// =========================
// TURN OFF ALL LEDs
// =========================

void turnOffAllLEDs()
{
  for (int i = 0; i < NUM_PINS; i++)
  {
    digitalWrite(LED_PINS[i], LOW);
  }
}

// =========================
// START NEW GAME
// =========================

void startNewGame()
{
  seqLen = 0;

  correctCount = 0;
  wrongCount = 0;
  attempts = 0;

  turnOffAllLEDs();

  addRandomStep();

  Serial.println();
  Serial.println("========================");
  Serial.println("      NEW GAME");
  Serial.println("========================");
  Serial.println("You have 10 attempts.");
}

// =========================
// ADD RANDOM STEP
// =========================

void addRandomStep()
{
  if (seqLen >= MAX_SEQ_LEN)
  {
    return;
  }

  int newStep = random(0, NUM_PINS);

  sequenceArr[seqLen] = newStep;

  seqLen++;

  Serial.print("New step: ");
  Serial.println(newStep);
}

// =========================
// SHOW SEQUENCE
// =========================

void showSequence()
{
  turnOffAllLEDs();

  Serial.println();
  Serial.println("Watch the sequence!");

  for (int i = 0; i < seqLen; i++)
  {
    int currentStep = sequenceArr[i];

    delay(300);

    digitalWrite(LED_PINS[currentStep], HIGH);

    switch (currentStep)
    {
      case 0:
        tone(BUZZER, 1000);
        break;

      case 1:
        tone(BUZZER, 1300);
        break;

      case 2:
        tone(BUZZER, 1600);
        break;

      case 3:
        tone(BUZZER, 1900);
        break;
    }

    delay(500);

    digitalWrite(LED_PINS[currentStep], LOW);

    noTone(BUZZER);

    delay(300);
  }

  turnOffAllLEDs();
}

// =========================
// CHECK PLAYER ANSWER
// =========================

bool isPlayerRight()
{
  Serial.println("Your turn!");

  for (int i = 0; i < seqLen; i++)
  {
    int playerInput = waitForButtonPress();

    Serial.print("You pressed: ");
    Serial.println(playerInput);

    if (playerInput != sequenceArr[i])
    {
      return false;
    }
  }

  return true;
}

// =========================
// WAIT FOR BUTTON
// =========================

int waitForButtonPress()
{
  turnOffAllLEDs();

  int pressedIndex = -1;

  while (pressedIndex == -1)
  {
    for (int i = 0; i < NUM_PINS; i++)
    {
      if (digitalRead(BTN_PINS[i]) == LOW)
      {
        delay(50);

        if (digitalRead(BTN_PINS[i]) == LOW)
        {
          pressedIndex = i;
          break;
        }
      }
    }

    delay(5);
  }

  // Turn on selected LED
  digitalWrite(LED_PINS[pressedIndex], HIGH);

  switch (pressedIndex)
  {
    case 0:
      tone(BUZZER, 1000);
      break;

    case 1:
      tone(BUZZER, 1300);
      break;

    case 2:
      tone(BUZZER, 1600);
      break;

    case 3:
      tone(BUZZER, 1900);
      break;
  }

  // Wait until button is released
  while (digitalRead(BTN_PINS[pressedIndex]) == LOW)
  {
    delay(10);
  }

  digitalWrite(LED_PINS[pressedIndex], LOW);

  noTone(BUZZER);

  delay(100);

  return pressedIndex;
}

// =========================
// GAME OVER ANIMATION
// =========================

void gameOverAnimation()
{
  for (int k = 0; k < 2; k++)
  {
    for (int i = 0; i < NUM_PINS; i++)
    {
      digitalWrite(LED_PINS[i], HIGH);
    }

    tone(BUZZER, 500);

    delay(150);

    turnOffAllLEDs();

    noTone(BUZZER);

    delay(150);
  }
}

// =========================
// WINNING ANIMATION
// =========================

void winningAnimation()
{
  for (int k = 0; k < 3; k++)
  {
    for (int i = 0; i < NUM_PINS; i++)
    {
      digitalWrite(LED_PINS[i], HIGH);
    }

    tone(BUZZER, 1500);

    delay(200);

    turnOffAllLEDs();

    noTone(BUZZER);

    delay(200);
  }
}

// =========================
// SCORE CARD + ESP32
// =========================

void showScoreCard()
{
  Serial.println();
  Serial.println("========================");
  Serial.println("       SCORE CARD");
  Serial.println("========================");

  Serial.print("Correct : ");
  Serial.print(correctCount);
  Serial.print("/");
  Serial.println(TOTAL_ATTEMPTS);

  Serial.print("Wrong   : ");
  Serial.print(wrongCount);
  Serial.print("/");
  Serial.println(TOTAL_ATTEMPTS);

  Serial.println("========================");

  // =========================
  // SEND SCORE TO ESP32
  // =========================

  espSerial.print("SCORE:");
  espSerial.print(correctCount);
  espSerial.print(",");
  espSerial.print(wrongCount);
  espSerial.print(",");
  espSerial.println(attempts);

  Serial.println("Score sent to ESP32!");
}