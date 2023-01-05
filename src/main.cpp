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

bool startState = LOW;

unsigned long startMillis;
unsigned long currentMillis;
unsigned long elapsedMillis;

void setup() {
  //define INPUT pins
  pinMode(startPin, INPUT);
  startDebouncer.attach(startPin);
  startDebouncer.interval(5); 
  // clean the display
  lcd.clear();
  // put your setup code here, to run once:
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Cronometro:");
  lcd.setCursor(0,1);
  lcd.print("Pulsa START");
}

void loop() {
  // Update the Bounce instances :
  startDebouncer.update();

  if ( startDebouncer.fell() ) {     // Call code if button transitions from HIGH to LOW
    startState = HIGH;         // Toggle start button state
    startMillis = millis();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cronometro:");
  }

  if (startState)
  {
    currentMillis = millis();
    elapsedMillis = (currentMillis - startMillis);
    
    unsigned long durMS = (elapsedMillis%1000);       //Milliseconds
    unsigned long durSS = (elapsedMillis/1000)%60;    //Seconds
    unsigned long durMM = (elapsedMillis/(60000))%60; //Minutes
 
    //String durMilliSec = timeMillis(durHH, durMM, durSS,durMS);
    sprintf(timeline,"%0.2lu:%0.2lu:%0.2lu", durMM, durSS, durMS);
    lcd.setCursor(0, 1);
    lcd.print(timeline);
  }
}