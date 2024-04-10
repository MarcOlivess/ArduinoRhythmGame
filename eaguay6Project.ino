#include <TimeLib.h>

const int leftButtonPin = 3;
const int rightButtonPin = 4;
int rightButtonState = HIGH;
int leftButtonState = HIGH;
int leftLastButtonState = LOW;
int rightLastButtonState = LOW;

long leftLastDebounce = 0;
long rightLastDebounce = 0;

long debounceDelay = 50;

int noteTimes[] = {1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000};
char buttonStates[] = {'L','R','L','R','L','R','L','R','L','R'};

unsigned int currentNote = 0; // Index of the current note
unsigned long gameStartTime;
bool gameStarted = false;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);

  gameStartTime = millis();
  gameStarted = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!gameStarted) {
    return;
  }

  Serial.print("Current Time: ");
  Serial.println(currentTime);

  // Debounce left button
  int leftReading = digitalRead(leftButtonPin);
  if (leftReading != leftLastButtonState) {
    leftLastDebounceTime = currentTime;
  }
  if ((currentTime - leftLastDebounceTime) > debounceDelay) {
    if (leftReading != leftButtonState) {
      leftButtonState = leftReading;
      if (leftButtonState == LOW) {
        checkNoteHit('L', currentTime);
      }
    }
  }
  leftLastButtonState = leftReading;

  int rightReading = digitalRead(rightButtonPin);
  if (rightReading != rightLastButtonState) {
    rightLastDebounceTime = currentTime;
  }
  if ((currentTime - rightLastDebounceTime) > debounceDelay) {
    if (rightReading != rightButtonState) {
      rightButtonState = rightReading;
      if (rightButtonState == LOW) {
        checkNoteHit('R', currentTime);
      }
    }
  }
  rightLastButtonState = rightReading;
}

void checkNoteHit(char button, unsigned long hitTime) {
  if (currentNote < sizeof(noteTimes) / sizeof(noteTimes[0])) {
    // Calculate the difference between the hit time and the scheduled note time
    long timeDifference = hitTime - noteTimes[currentNote];

    // Check if the note was hit within +/- 100ms window and with the correct button
    if (abs(timeDifference) <= 100 && buttonStates[currentNote] == button) {
      Serial.println("Hit!");
    } else {
      Serial.println("Miss...");
    }
    
    currentNote++; // Move to the next note
  } else {
    // If all notes have been attempted, end the game
    Serial.println("Game Over!");
    gameStarted = false; // Prevent further input handling
  }
}
