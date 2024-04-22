#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Initialize LCD with the pin numbers for RS, E, D4, D5, D6, D7
const int greenLedPin = 8; // Pin for the green LED
const int redLedPin = 9;   // Pin for the red LED
int score = 0;             // Initialize score
int currentNoteIndex = 0;  // Index of the current note being played

// Arrays to store note types and times
char noteTypes[] = {'L', 'L', 'L', 'R', 'L', 'L', 'L', 'R', 'L', 'L', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'L', 'L', 'R', 'R', 'L', 'L', 'R', 'R', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'L', 'L', 'L', 'R', 'L', 'L', 'R', 'R', 'R', 'L', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'L', 'L', 'R', 'L', 'L', 'L', 'L', 'L', 'R'};
unsigned long noteTimes[] = {3330, 3996, 4662, 5328, 5994, 6660, 7326, 7992, 8658, 9324, 9990, 10656, 13320, 13653, 13986, 14319, 14985, 15318, 15651, 15984, 16317, 16650, 16983, 17316, 17649, 17982, 18315, 18648, 18981, 19314, 19647, 20313, 21312, 21978, 22644, 23310, 23976, 24642, 25308, 25974, 26640, 27306, 27972, 28638, 29304, 30303, 30636, 30969, 31302, 31635, 31968, 32301, 33633, 34632, 34965, 35298, 36297, 36630, 36963, 37296, 37629, 37962};
char songMap[] = "            L  L  L  R  L  L  L  R  L  L  L  R        L R L R  L R L L L R R L L R R L L L L  L   R  R  R  R  L  L  L  R  L  L  R  R  R   L L L L L L R    R   L L R   L L L L L R   L L L L L R   L L L L L R     L R  L  L  L  R  L  L  L  R  L  L  L  R  L  L  L  R  L    L    L    L    L    L    L";

void setup() {
  lcd.begin(16, 2); // Initialize LCD with 16 columns and 2 rows
  pinMode(greenLedPin, OUTPUT); // Set up LED pins
  pinMode(redLedPin, OUTPUT);
  Serial.begin(9600); // Start serial communication at 9600 bps
}

void loop() {
  while (Serial.available() <= 0) {
    scrollText(songMap, 150); // Scroll the song map text every 150 milliseconds
  }

  char input = Serial.read();

  if (input == 's') {
    Serial.println("Game Started");
    startGame();
  }
}

void startGame() {
  unsigned long startTime = millis();
  unsigned long lastUpdateTime = 0;
  int j = 0;

  while (currentNoteIndex < sizeof(noteTimes) / sizeof(noteTimes[0])) {
    unsigned long currentTime = millis() - startTime;

    // Scroll text without clearing the entire screen
    if (currentTime - lastUpdateTime >= 333) {
      lcd.setCursor(0, 0);
      if (j < strlen(songMap)) {
        lcd.print(&songMap[j]);
        j++;
      } else {
        j = 0; // Reset the scroll
      }
      lastUpdateTime = currentTime;
    }

    if (currentTime >= noteTimes[currentNoteIndex]) {
      Serial.print(noteTypes[currentNoteIndex]);
      Serial.print(": ");
      Serial.println(noteTimes[currentNoteIndex]);
      currentNoteIndex++;
    }

    checkSerialInput();
  }

  Serial.println("Game Over!");
  Serial.print("Final score was: ");
  Serial.println(score);
  // Optionally reset the game state here
}

void checkSerialInput() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == 'h') {
      score++;
      digitalWrite(greenLedPin, HIGH); // Turn on green LED
      digitalWrite(redLedPin, LOW);    // Turn off red LED
      delay(200); // Show LED for 200 ms
      digitalWrite(greenLedPin, LOW);
    } else if (input == 'm') {
      digitalWrite(greenLedPin, LOW);  // Turn off green LED
      digitalWrite(redLedPin, HIGH);   // Turn on red LED
      delay(200); // Show LED for 200 ms
      digitalWrite(redLedPin, LOW);
    }
  }
}

void scrollText(char* text, unsigned int delayTime) {
  static unsigned int pos = 0; // Current position of scrolling
  static unsigned long lastUpdateTime = 0; // Last update timestamp
  if (millis() - lastUpdateTime > delayTime) {
    lcd.setCursor(0, 0); // Set the cursor at the beginning of the first line
    if (pos < strlen(text)) {
      // Only print up to the end of the first line or the end of the text
      int charCount = 0;
      while (charCount < 16 && (pos + charCount) < strlen(text)) {
        lcd.print(text[pos + charCount]);
        charCount++;
      }
      // Fill the rest of the line with spaces if any
      while (charCount < 16) {
        lcd.print(" ");
        charCount++;
      }
    } else {
      lcd.print("                "); // Clear line after the end of the text
      pos = 0; // Reset position for continuous scrolling
    }
    pos++;
    lastUpdateTime = millis();
  }
}