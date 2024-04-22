//Start Game: 0x01
//Hit: 0x01 
//Miss: 0x00
//Game Over: 0x04
//Score Data: Follow 0x04 with score bytes


#include <TimeLib.h>

const int leftButtonPin = 3;
const int rightButtonPin = 4;

int leftButtonState;
int rightButtonState;
int leftLastButtonState = HIGH; // Assume buttons start unpressed
int rightLastButtonState = HIGH;

unsigned long leftLastDebounceTime = 0;
unsigned long rightLastDebounceTime = 0;
unsigned long debounceDelay = 50;

long noteTimes[] = {
    3330, 3996, 4662, 5328, 5994, 6660, 7326, 7992, 8658, 9324,
    9990, 10656, 13320, 13653, 13986, 14319, 14985, 15318, 15651, 15984,
    16317, 16650, 16983, 17316, 17649, 17982, 18315, 18648, 18981, 19314,
    19647, 20313, 21312, 21978, 22644, 23310, 23976, 24642, 25308, 25974,
    26640, 27306, 27972, 28638, 29304, 30303, 30636, 30969, 31302, 31635,
    31968, 32301, 33633, 34632, 34965, 35298, 36297, 36630, 36963, 37296,
    37629, 37962, 38961, 39294, 39627, 39960, 40293, 40626, 41625, 41958,
    42291, 42624, 42957, 43290, 44955, 45288, 45954, 46620, 47286, 47952,
    48618, 49284, 49950, 50616, 51282, 51948, 52614, 53280, 53946, 54612,
    55278, 55944, 56610, 57942, 59274, 60606, 61938, 63270, 64602
};
char buttonStates[] = {
    'L', 'L', 'L', 'R', 'L', 'L', 'L', 'R', 'L', 'L',
    'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'L',
    'L', 'R', 'R', 'L', 'L', 'R', 'R', 'L', 'L', 'L',
    'L', 'L', 'R', 'R', 'R', 'R', 'L', 'L', 'L', 'R',
    'L', 'L', 'R', 'R', 'R', 'L', 'L', 'L', 'L', 'L',
    'L', 'R', 'R', 'L', 'L', 'R', 'L', 'L', 'L', 'L',
    'L', 'R', 'L', 'L', 'L', 'L', 'L', 'R', 'L', 'L',
    'L', 'L', 'L', 'R', 'L', 'R', 'L', 'L', 'L', 'R',
    'L', 'L', 'L', 'R', 'L', 'L', 'L', 'L', 'R', 'L',
    'L', 'L', 'R', 'L', 'L', 'L', 'L', 'L', 'L'
};
int numElements = sizeof(noteTimes) / sizeof(noteTimes[0]);
unsigned int currentNote = 0; // Index of the current note
unsigned long gameStartTime;
bool gameStarted = false;
int correct = 0;
bool notePrompted = false;

void setup() {
  Serial.begin(9600);
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
}

void loop() {
  if (!gameStarted) {
    if (digitalRead(leftButtonPin) == LOW || digitalRead(rightButtonPin) == LOW) {
      initializeGame();
      delay(200); // Prevent multiple starts due to button bouncing
    }
    return; // Game already ended
  }

  unsigned long currentTime = millis() - gameStartTime;

  // Prompt for the next note 100ms before it is due
  if (currentNote < numElements && !notePrompted && currentTime >= noteTimes[currentNote] - 100) {
    Serial.print("Prepare to hit: ");
    Serial.println(buttonStates[currentNote] == 'L' ? "Left" : "Right");
    notePrompted = true; // Prevent multiple prompts for the same note
  }

  // Debouncing and reading for the left button
  int leftReading = digitalRead(leftButtonPin);
  if (leftReading != leftLastButtonState) {
    leftLastDebounceTime = currentTime;
  }
  if ((currentTime - leftLastDebounceTime) > debounceDelay && leftReading != leftButtonState) {
    leftButtonState = leftReading;
    if (leftButtonState == LOW) {
      checkNoteHit('L', currentTime);
    }
  }
  leftLastButtonState = leftReading;

  // Debouncing and reading for the right button
  int rightReading = digitalRead(rightButtonPin);
  if (rightReading != rightLastButtonState) {
    rightLastDebounceTime = currentTime;
  }
  if ((currentTime - rightLastDebounceTime) > debounceDelay && rightReading != rightButtonState) {
    rightButtonState = rightReading;
    if (rightButtonState == LOW) {
      checkNoteHit('R', currentTime);
    }
  }
  rightLastButtonState = rightReading;

  // Reset prompt flag after the note time has passed
  if (currentNote < numElements && currentTime > noteTimes[currentNote]) {
    notePrompted = false; // Allow the next note to be prompted
    currentNote++; // Move to the next note
  }

  // Game over condition if all notes are played
  if (currentNote >= numElements && currentTime > noteTimes[numElements - 1] + 200) {
    endGame();
  }
}

void checkNoteHit(char button, unsigned long hitTime) {
  if (currentNote < numElements) {
    long timeDifference = hitTime - noteTimes[currentNote];

    // Check if the note was hit within +/- 100ms window and with the correct button
    if (abs(timeDifference) <= 850 && buttonStates[currentNote] == button) {
      Serial.println("Hit!");
      correct++;
      Serial.write('h');
    } else {
      Serial.write('m');
      Serial.print("Miss... Expected Index and Time at : ");
      Serial.print(currentNote);
      Serial.print(" ");
      Serial.print(noteTimes[currentNote]);
      Serial.print(" ms, Pressed: ");
      Serial.print(hitTime);
      Serial.print(" ms, Off by: ");
      Serial.print(timeDifference);
      Serial.println(" ms");
      Serial.print("Pressed button: ");
      Serial.print(button == 'L' ? "Left" : "Right");
      Serial.print(", Expected button: ");
      Serial.println(buttonStates[currentNote] == 'L' ? "Left" : "Right");
    }
  }
}

void initializeGame() {
  gameStartTime = millis();
  currentNote = 0;  // Reset the current note index
  correct = 0;      // Reset the score
  gameStarted = true; // Start the game
  Serial.write('s'); // Send start game signal
}
void endGame() {
  Serial.write(0x04);
  float percentageCorrect = ((float)correct / numElements) * 100.0;
  unsigned char scoreToSend = (unsigned char)percentageCorrect;
  Serial.write(scoreToSend);
  gameStarted = false;
  
  Serial.println("Game Over!");
  Serial.print("You got ");
  Serial.print(correct);
  Serial.print(" out of ");
  Serial.print(numElements);
  Serial.println(" notes correct!");
  gameStarted = false; // Properly stop game logic
}
