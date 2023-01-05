#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Bounce2.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

signed short minutes, secondes;
char timeline[16];

// define pins for buttons
const int startPin = 6;
Bounce startDebouncer = Bounce();

bool readyState = LOW;
bool startState = LOW;

// define pins for ultrasound sensor HC-SR04
const int trigger = 10, echo = 9;

const int buzzerPin = 8;

unsigned long startMillis;
unsigned long currentMillis;
unsigned long elapsedMillis;

unsigned long lastLapTime;
unsigned long bestLapTime;

void checkBestTime(unsigned long lastLap)
{
  if (lastLap < 1000) 
  {
    return;
  }

  if (lastLap < bestLapTime)
  {
    bestLapTime = lastLap;
  }
  unsigned long durMS = (bestLapTime%1000);       //Milliseconds
  unsigned long durSS = (bestLapTime/1000)%60;    //Seconds
  unsigned long durMM = (bestLapTime/(60000))%60; //Minutes

  sprintf(timeline,"Mejor %0.2lu:%0.2lu:%0.2lu", durMM, durSS, durMS);
  lcd.setCursor(0, 1);
  lcd.print(timeline);
}

void startLap()
{
  if (startMillis != 0)
  {
    currentMillis = millis();
    lastLapTime = (currentMillis - startMillis);
    checkBestTime(lastLapTime);
  }
  
  startMillis = millis();
}

void setup() {
  //define INPUT pins
  pinMode(startPin, INPUT);
  pinMode(echo, INPUT);
  // define OUTPUT pins
  pinMode(trigger, OUTPUT);
  startDebouncer.attach(startPin);
  startDebouncer.interval(5); 
  // clean the display
  lcd.clear();
  // put your setup code here, to run once:
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Pulsa START");
  bestLapTime = 600000;
}

void loop() {
  // Update the Bounce instances :
  startDebouncer.update();

  if ( startDebouncer.fell() ) {     // Call code if button transitions from HIGH to LOW
    readyState = !readyState;         // Toggle start button state
    startState = LOW;
    if (readyState)
    {
      lcd.setCursor(0, 0);
      lcd.print(" A correr!      ");
    }
  }

  if (startState)
  {
    currentMillis = millis();
    elapsedMillis = (currentMillis - startMillis);
    
    unsigned long durMS = (elapsedMillis%1000);       //Milliseconds
    unsigned long durSS = (elapsedMillis/1000)%60;    //Seconds
    unsigned long durMM = (elapsedMillis/(60000))%60; //Minutes

    sprintf(timeline,"Vuelta %0.2lu:%0.2lu:%0.2lu", durMM, durSS, durMS);
    lcd.setCursor(0, 0);
    lcd.print(timeline);
  }

  digitalWrite(trigger, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigger, LOW);
  long time = pulseIn(echo, HIGH);
  long distance = time/59;

  if (distance < 8 && distance > 2 && readyState)
  {
    // start lap
    tone(buzzerPin, 100, 100);
    startState = HIGH;
    //lcd.clear();
    startLap();
  }
}