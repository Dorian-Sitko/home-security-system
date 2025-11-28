#include <Keypad.h>
#include <LiquidCrystal.h>

// --- LCD Pins ---
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// --- Keypad Setup ---
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {0, 1, 2, 3};  // R1-R4
byte colPins[COLS] = {4, 5, 6};     // C1-C3

Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- Sensor and Actuator Pins ---
const int trigPin = A2;
const int echoPin = A1;
const int tempPin = A0;
const int buzzerPin = A5;
const int switchPin = A3;
const int ledPin = A4;

// --- Variables ---
String password = "1234";
String input = "";
float distance, duration, temp;

void setup() {
  Serial.begin(9600);

  // Pin setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP); // assuming normally open button
  pinMode(ledPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("System Ready");
  delay(1500);
  lcd.clear();
}

void loop() {
  checkPassword();
  readUltrasonic();
  showTemperature();
  readFingerprint();
}

// --- Password Check ---
void checkPassword() {
  char key = customKeypad.getKey();
  if (key) {
    input += key;
    Serial.print(key);
    lcd.setCursor(0, 0);
    lcd.print("Entered: ");
    lcd.print(input);

    if (input.length() == password.length()) {
      if (input == password) {
        lcd.setCursor(0, 1);
        lcd.print("Access Granted ");
        digitalWrite(buzzerPin, HIGH);
        delay(1000);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(ledPin, HIGH);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Access Denied  ");
        digitalWrite(buzzerPin, HIGH);
        delay(500);
        digitalWrite(buzzerPin, LOW);
      }
      input = "";
      delay(1000);
      lcd.clear();
      digitalWrite(ledPin, LOW);
    }
  }
}

// --- Ultrasonic Sensor ---
void readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Optional: Display warning if someone is too close
  if (distance < 20) {
    lcd.setCursor(0, 0);
    lcd.print("Too Close!     ");
  }

  delay(100);
}

// --- Temperature Display ---
void showTemperature() {
  int analogVal = analogRead(tempPin);
  temp = analogVal * 0.48828125; // TMP36 conversion for °C

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temp, 1);
  lcd.print("C    ");

  delay(500);
}
// --- Fingerprint Scanner ---
readFingerprint(){
  
}

