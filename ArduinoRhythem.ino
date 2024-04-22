
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // LCD pins
const int greenLedPin = 8;
const int redLedPin = 9;
int score = 0;
int currentNoteColumn = 0;


// Arrays to store note types and times
char noteTypes[] = {'L','L','L','R','L','L','L','R','L','L','L','R','L','R','L','R','L','R','L','L','L','R','R','L','L','R','R','L','L','L','L','L','R','R','R','R','L','L','L','R','L','L','R','R','R','L','L','L','L','L','L','R','R','L','L','R','L','L','L','L','L','R'};
unsigned long noteTimes[] = {3330,3996,4662,5328,5994,6660,7326,7992,8658,9324,9990,10656,13320,13653,13986,14319,14985,15318,15651,15984,16317,16650,16983,17316,17649,17982,18315,18648,18981,19314,19647,20313,21312,21978,22644,23310,23976,24642,25308,25974,26640,27306,27972,28638,29304,30303,30636,30969,31302,31635,31968,32301,33633,34632,34965,35298,36297,36630,36963,37296,37629,37962};
char songMap[] = "            L  L  L  R  L  L  L  R  L  L  L  R        L R L R  L R L L L R R L L R R L L L L  L   R  R  R  R  L  L  L  R  L  L  R  R  R   L L L L L L R    R   L L R   L L L L L R   L L L L L R   L L L L L R     L R  L  L  L  R  L  L  L  R  L  L  L  R  L  L  L  R  L    L    L    L    L    L    L";


void setup() {
  lcd.begin(400, 2); // Initialize LCD
  pinMode(greenLedPin, OUTPUT); // Initialize LED pins
  pinMode(redLedPin, OUTPUT);
  Serial.begin(9600); // Initialize serial communication
   lcd.setCursor(0, 0);
   lcd.print(songMap);
}

void loop() {
  while (Serial.available() <= 0) {
    // Wait for serial input to start the game
  }

  char input = Serial.read();

  if (input == 's') {
    Serial.print("Started");
    startGame();
  }

  Serial.print("your score was: ");
  Serial.print(score);
}

void startGame() {
  unsigned long startTime = millis(); 
  int currentColumn = 0;
  int lastIndex = sizeof(noteTimes) / sizeof(noteTimes[0]) - 1;
  int i =0;
  int j = 0;
 unsigned long lastUpdateTime = 0;

  while (millis() - startTime < noteTimes[lastIndex]) {
    unsigned long currentTime = millis() - startTime;

      int time = noteTimes[i]; // Time of the note
      char noteType = noteTypes[i]; // Note type (R, L, RL)

    if (currentTime - lastUpdateTime >= 333) {
      j++;
      lcd.clear();

      lcd.setCursor(0, 0);

      if (j < strlen(songMap)) {
        // Print the substring of the songMap from j to the end
        lcd.print(&songMap[j]);
        // If the length of the substring is less than 16, print spaces to fill the rest of the line
        for (int k = strlen(&songMap[j]); k < 16; k++) {
          lcd.print(" ");
        }
      } else {
        // If the end of the songMap is reached, print spaces to clear the line
        for (int k = 0; k < 16; k++) {
          lcd.print(" ");
        }
      }
      
      lastUpdateTime = currentTime;
    }



      if (currentTime >= time) {
        
        Serial.print(noteType);
        Serial.print(":");
        Serial.println(time);
        currentColumn++;
        i++;
        unsigned long noteDisplayTime = millis();
        while (millis() - noteDisplayTime < 50) {
        }

      }
      checkSerialInput();
    }
  }


void checkSerialInput() {
  if (Serial.available() > 1) {
    char input = Serial.read();
    if (input == 'h') {
      score++;
      digitalWrite(greenLedPin, HIGH); // Turn on green LED
      digitalWrite(redLedPin, LOW);    // Turn off red LED1
    } else if (input == 'm') {
      digitalWrite(greenLedPin, LOW);  // Turn off green LED
      digitalWrite(redLedPin, HIGH);   // Turn on red LED
    }
  }
}

