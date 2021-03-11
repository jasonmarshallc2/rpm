#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int feedPin = 2; // digital pin 2 is the hall pin
int spindlePin = 3;
// set number of hall trips for RPM reading (higher improves accuracy)
int hall_thresh = 5;
unsigned long feedStart = micros();
unsigned long spindleStart = micros();
unsigned long end_time = 0;
unsigned long boringStartTime = millis();
//float feedCount = 1.0;
//feedStart = 0;
//feedStart = micros();
bool feedOnState = false;
bool spindleOnState = false;
int feedCount = 1;
int spindleCount = 1;
float timePassedFeed = 0.0;
float timePassedSpindle = 0.0;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // make the hall pin an input:
  pinMode(feedPin, INPUT);
  pinMode(spindlePin, INPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.write("Hello");
}

void loop() {

/*
 listen for signals
if counter is met, update the display, reset counter and start times
*/

 if (digitalRead(feedPin)==LOW){
   if (feedOnState==false){
     feedOnState = true;
     feedCount += 1;
     }
   } else {
     feedOnState = false;
     }
   if (feedCount>=hall_thresh){
     updateDisplay(0); 
     resetFeedVar();
     }
     
 if (digitalRead(spindlePin)==LOW){
   if (spindleOnState==false){
     spindleOnState = true;
     spindleCount += 1;
     }
   } else {
     spindleOnState = false;
     }
   if (spindleCount>=hall_thresh){
     updateDisplay(1); 
     resetSpindleVar();
     }

Serial.print("end_time: ");
Serial.print(end_time / 1000.0);
Serial.print(" millis: ");
Serial.println(millis());
if(millis() - (end_time /1000.0) > 10000){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" This is boring!");
  lcd.setCursor(0,1);
  lcd.print(" Do Something");
  }
}

void resetFeedVar(){
  feedStart = 0;
  feedStart = micros();
  feedOnState = false;
  feedCount = 1;
  timePassedFeed = 0.0;
}
void resetSpindleVar(){
  spindleStart = 0;
  spindleStart = micros();
  spindleOnState = false;
  spindleCount = 1;
  timePassedSpindle = 0.0;
}
void updateDisplay(int row){
  end_time = micros();
  float rpmVal = 0.0;
//  lcd.clear();
  if(row == 0){
    timePassedFeed = ((end_time-feedStart)/1000000.0);
    rpmVal = (feedCount/timePassedFeed)*60.0;
    lcd.setCursor(0, 0);
    lcd.print("FEED ");
    lcd.print(rpmVal);
    lcd.print(" RPM");
  } else {
    timePassedSpindle = ((end_time-spindleStart)/1000000.0);
    rpmVal = (spindleCount/timePassedSpindle)*60.0;
    lcd.setCursor(0, 1);
    lcd.print("SPINDLE ");
    lcd.print(rpmVal);
    lcd.print(" RPM");
  }


  Serial.print(rpmVal);
  Serial.println(" RPM");
  delay(1);        // delay in between reads for stability

}
