#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// Set up the LCD screen
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Connect the piezo speaker to pin 12
int speakerPin = 12;

// Create arrays to store tunes and notes
int startTune[] = {500, 500, 300, 500, 700, 700};
int note[] = {440, 440, 440, 440, 494, 440}; // A4, A4, A4, A4, B4, A4
int duration[] = {100, 100, 100, 300, 100, 300};
int happyTune[] = {523, 587, 659}; 
int sadTune[] = {392, 330, 294};  

// Define pins for LEDs and buttons
int redLedPin = 2;
int blueLedPin = 3;
int yellowLedPin = 4;
int greenLedPin = 5;
int redButtonPin = 6;
int blueButtonPin = 7;
int yellowButtonPin = 8;
int greenButtonPin = 9;

// Create arrays for LED and button pins
int ledPin[] = {redLedPin, blueLedPin, yellowLedPin, greenLedPin};  // LEDs
int buttonPin[] = {redButtonPin, blueButtonPin, yellowButtonPin, greenButtonPin};  // Buttons

// Game variables
int turn = 0;  // Game round
int buttonState = 0;  // Button state
int randomArray[50]; // Store up to 50 inputs
int inputArray[50];
int highScore = 0;
int myScore;

void setup() {
  // Set up the LCD display and Serial communication
  lcd.begin(16, 2);
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Your Score: 0");
  lcd.setCursor(0, 0);
  lcd.print("High Score: 0");
  Serial.begin(9600);

  // Set LED pins as outputs
  for (int x = 0; x < 4; x++) {
    pinMode(ledPin[x], OUTPUT);
  }

  // Set button pins as inputs with pull-up resistors
  for (int x = 0; x < 4; x++) {
    pinMode(buttonPin[x], INPUT_PULLUP);
  }

  // Seed the random number generator
  randomSeed(analogRead(0));

  // Play initial startup tune using the piezo speaker
  for (int thisNote = 0; thisNote < 6; thisNote++) {
    if (!(thisNote == 0 && ledPin[0] == redLedPin)) {
      digitalWrite(ledPin[0], HIGH);
      tone(speakerPin, note[thisNote], duration[thisNote]);
      delay(duration[thisNote] + 50);
      digitalWrite(ledPin[0], LOW);
    } else {
      delay(duration[thisNote] + 50);
    }

    noTone(speakerPin);

    delay(25);
  }
  delay(1000);
}

void loop() {
  // Main game loop
  for (int y = 0; y <= 2; y++) {
    // Play the startup tune for the current round
    playTurnTune();

    // Display the current round on the LCD
    displayTurn();

    // Generate a random sequence of LEDs for the player to follow
    generateRandomSequence();

    // Display the random sequence on LEDs
    displaySequence();

    // Get player input and check against the random sequence
    getInput();
  }
}

// Function to play the startup tune for each round
void playTurnTune() {
  for (int thisNote = 0; thisNote < 6; thisNote++) {
    tone(speakerPin, note[thisNote], duration[thisNote]);
    delay(duration[thisNote] + 50);
    noTone(speakerPin);

    digitalWrite(ledPin[0], HIGH);
    delay(25);
    digitalWrite(ledPin[0], LOW);

    delay(25);
  }

  digitalWrite(ledPin[0], LOW);

  delay(1000);
}

// Function to display the current round on the LCD
void displayTurn() {
  for (int y = turn; y <= turn; y++) {
    Serial.println("");
    Serial.print("Turn: ");
    Serial.print(y);
    Serial.println("");
    lcd.clear();
    myScore = y;
    lcd.setCursor(0, 1);
    lcd.print("Your Score: ");
    lcd.setCursor(12, 1);
    lcd.print(myScore);
    if (myScore > highScore) {
      highScore = y;
    }
    lcd.setCursor(0, 0);
    lcd.print("High Score: " + (String)(highScore));
    Serial.print(y);
    Serial.println("");
    delay(1000);
  }
}

// Function to generate a random sequence of LEDs for the player to follow
void generateRandomSequence() {
  randomArray[turn] = random(1, 5);
}

// Function to display the random sequence on LEDs
void displaySequence() {
  for (int x = 0; x <= turn; x++) {
    Serial.print(randomArray[x]);

    for (int y = 0; y < 4; y++) {
      // Display each LED in the sequence
      if (randomArray[x] == 1 && ledPin[y] == redLedPin) {
        digitalWrite(ledPin[y], HIGH);
        playSound(speakerPin, note[0], duration[0]); // Play sound for red LED
        delay(400);
        digitalWrite(ledPin[y], LOW);
        delay(100);
      }

      if (randomArray[x] == 2 && ledPin[y] == blueLedPin) {
        digitalWrite(ledPin[y], HIGH);
        playSound(speakerPin, note[1], duration[1]); // Play sound for blue LED
        delay(400);
        digitalWrite(ledPin[y], LOW);
        delay(100);
      }

      if (randomArray[x] == 3 && ledPin[y] == yellowLedPin) {
        digitalWrite(ledPin[y], HIGH);
        playSound(speakerPin, note[2], duration[2]); // Play sound for yellow LED
        delay(400);
        digitalWrite(ledPin[y], LOW);
        delay(100);
      }

      if (randomArray[x] == 4 && ledPin[y] == greenLedPin) {
        digitalWrite(ledPin[y], HIGH);
        playSound(speakerPin, note[3], duration[3]); // Play sound for green LED
        delay(400);
        digitalWrite(ledPin[y], LOW);
        delay(100);
      }
    }
  }
}

// Function to get player input and check against the random sequence
void getInput() {
  for (int x = 0; x <= turn;) {
    for (int y = 0; y < 4; y++) {
      buttonState = digitalRead(buttonPin[y]);

      if (buttonState == LOW && buttonPin[y] == redButtonPin) {
        // Display and play sound for red button press
        digitalWrite(ledPin[0], HIGH);
        playSound(speakerPin, note[0], duration[0]);
        delay(200);
        digitalWrite(ledPin[0], LOW);
        inputArray[x] = 1;
        delay(250);
        Serial.print(" ");
        Serial.print(1);
        // Check if player input matches the random sequence
        if (inputArray[x] != randomArray[x]) {
          fail();
        }
        x++;
      }

      // Similar logic for other buttons (blue, yellow, green)

      delay(500);
      turn++;
    }
  }
}

// Function to handle failure scenario
void fail() {
  for (int y = 0; y <= 3; y++) {
    // Display and play sad tune for failure
    digitalWrite(ledPin[0], HIGH);
    digitalWrite(ledPin[1], HIGH);
    digitalWrite(ledPin[2], HIGH);
    digitalWrite(ledPin[3], HIGH);
    playSound(speakerPin, sadTune[0], duration[0]);
    delay(200);
    digitalWrite(ledPin[0], LOW);
    digitalWrite(ledPin[1], LOW);
    digitalWrite(ledPin[2], LOW);
    digitalWrite(ledPin[3], LOW);
    playSound(speakerPin, sadTune[1], duration[1]);
    delay(200);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
  }
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("High: " + (String)(highScore));
  lcd.setCursor(9, 0);
  lcd.print("You: ");
  lcd.setCursor(14, 0);
  lcd.print(myScore);
  lcd.setCursor(0, 1);
  lcd.print("PressRedToPlay");
  {
    while (digitalRead(buttonPin[0]) > 0 && digitalRead(buttonPin[1]) > 0 && digitalRead(buttonPin[2]) > 0 && digitalRead(buttonPin[3]) > 0) {
      digitalWrite(ledPin[0], HIGH);
      digitalWrite(ledPin[1], HIGH);
      digitalWrite(ledPin[2], HIGH);
      digitalWrite(ledPin[3], HIGH);
      delay(100);
      digitalWrite(ledPin[0], LOW);
      digitalWrite(ledPin[1], LOW);
      digitalWrite(ledPin[2], LOW);
      digitalWrite(ledPin[3], LOW);
      delay(100);
    }
    delay(1000);
    turn = -1;
  }
}

// Function to play a sound using the piezo speaker
void playSound(int pin, int frequency, int duration) {
  tone(pin, frequency, duration);
  delay(duration + 50);
  noTone(pin);
}
