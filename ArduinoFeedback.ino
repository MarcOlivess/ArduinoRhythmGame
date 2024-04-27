
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // LCD pins
const int greenLedPin = 8;
const int redLedPin = 9;
const int musicPin = 7; // we use a greeting card module to store the mp3, activatable through digital write. 
int score = 0;


// A map of all notes for the game, used for displaying in real time 

unsigned long noteTimes[] = {
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
// the postion in which the letter will appear, left or right, 
char noteTypes[] = {
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
void setup() {
  lcd.begin(16, 2); // Initialize LCD
  pinMode(greenLedPin, OUTPUT); // Initialize LED pins
  pinMode(redLedPin, OUTPUT);
  pinMode(musicPin, OUTPUT);
  Serial.begin(9600); // Initialize serial communication
  lcd.setCursor(0, 0);
}

void loop() {
  lcd.print("Welcome, Press a");
  lcd.setCursor(0, 1);
  lcd.print("Button to play");

  while (Serial.available() <= 0) {
    // Wait for serial input to start the game
  }

  char input = Serial.read();

  // We use chars to minimize the data sent, allowing for faster handling
  // starts game loop until game is over
  if (input == 's') {
    lcd.clear();
    lcd.setCursor(0, 0);
    Serial.print("Started");
    startGame();
  }

  //when game over, show score. 

  Serial.print("your score was: ");
  Serial.print(score);

 //wait for another input to start the game again
//Meanwhile, show score again just to make sure it was shown
  //along with white space to make sure its the only thing on lcd

  while (Serial.available() <= 0) {
    
  lcd.setCursor(0, 0);
  lcd.print("Your score was: ");
  lcd.setCursor(0, 1);
   lcd.print("        ");
  lcd.print(score);
   lcd.print("        ");

  lcd.setCursor(0, 0);
  }


}

void startGame() {
  unsigned long startTime = millis(); 
  int currentColumn = 0;
  
  unsigned long lastIndex = sizeof(noteTimes) / sizeof(noteTimes[0]) - 1;
  int i = 0;
  
  unsigned long clearTime = 0;

  digitalWrite(musicPin, HIGH);

  //before while loop, it sets up the song start time, the point at which it should end
  //and starts the music right before the game, should be neglegible time diffrence


  // the while loop continues to process notes for display 
  // uses the arrays for keeping time, and note. 

  while (millis() - startTime < noteTimes[lastIndex]) {
    unsigned long currentTime = millis() - startTime;

    
      //Casts the current ntoe and time to variables 
      //that we handle during the loop
     
      unsigned long time = noteTimes[i]; // Time of the note
      char noteType = noteTypes[i]; // Note type (R, L, RL)
      
      //Display section start
      //Displays the correct note on the correct side for game, 
      // plays the note 100 ms before the note should be played in game  
      if (currentTime >= time-100){
      
      if(noteType == 'L'){

        lcd.setCursor(3, 0);

      } else {
        lcd.setCursor(12, 0);
      }
      lcd.print(noteType);
  
      //Waits a time before clearing the LCD       
        if (currentTime - clearTime >= 100) {
          lcd.clear();
          clearTime = currentTime; // Update the clear time
        }
      
      }
    
      //LCD display end

       //Serial confirmation code
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
      // Fucntion to check the input from the 2nd arduino. 
      // see, fucntion for details 
      checkSerialInput();
    }
  }
 

//When a avaible, check the input above 1 (to avoid white noise) 
//when H is read, add to score, and turn on green for feed back
//when M is sent, subtract from score and turn on red for feedback

//Error handling is checked in the controller arduino
//This allows for the best distrabution of actions and calculation
//which properlly allows for acurate real time game play 
void checkSerialInput() {
  if (Serial.available() > 1) {
    char input = Serial.read();
    if (input == 'h') {
      score++;
      digitalWrite(greenLedPin, HIGH); // Turn on green LED
      digitalWrite(redLedPin, LOW);    // Turn off red LED1
    } else if (input == 'm') {
      score--;
      digitalWrite(greenLedPin, LOW);  // Turn off green LED
      digitalWrite(redLedPin, HIGH);   // Turn on red LED
    }
  }
}

