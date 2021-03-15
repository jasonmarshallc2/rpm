#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int feedPin = 3; // digital pin 2 is the short wire
int spindlePin = 2;
// set number of hall trips for RPM reading (higher improves accuracy)
int hall_thresh = 5;
unsigned long feedStart = micros(); // used in rpm calculation
unsigned long spindleStart = micros(); // used in rpm calculation
unsigned long feedEndTime = 0; // used to capture the amount of time it took to reach the # of magnet reads, set as hall_thresh
unsigned long spindleEndTime = 0; 
bool feedOnState = false; // used to prevent dup counts
bool spindleOnState = false; // used to prevent dup counts
int feedCount = 0; // number of itterations through the count cycle
int spindleCount = 0; // number of itterations through the count cycle
float timePassedFeed = 0.0; // the calculated time that was elapsed to reach the hall_thresh
float timePassedSpindle = 0.0; // the calculated time that was elapsed to reach the hall_thresh
bool pausedScreen = false; 

void setup() {
  // initialize serial communication at 9600 bits per second:
  //Serial.begin(9600);
  pinMode(feedPin, INPUT); // set as input
  pinMode(spindlePin, INPUT); // set as input
  lcd.begin(16, 2); // screen size
  lcd.setCursor(0,0); // initialize the screen so it's not blank at startup
  lcd.write("Hello");
}

void loop() {

/*
 listen for signals
if counter is met, update the display, reset counter & start times
*/

 if (digitalRead(feedPin)==LOW){ // LOW is when the unit detects a magnet
   if(pausedScreen == true){
    lcd.clear();
   }   pausedScreen = false;
   if (feedOnState==false){ // prevent duplicate counts
     feedOnState = true;
     feedCount += 1;
     }
   } else {
     feedOnState = false; // prevent duplicate counts
     }
   if (feedCount>=hall_thresh){ // if the number of magnet reads reaches the min, update the display
     updateDisplay(0);  // 0 is passed to represent the first row in the display
     resetFeedVar(); // reset counters
     }
     
 if (digitalRead(spindlePin)==LOW){ // same as above, only for the spindle sensor
   if(pausedScreen == true){
    lcd.clear();
   }
   pausedScreen = false;
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

// Clear the line if one reading has stopped
if(micros() - (feedEndTime / 1000.0) > 5000){
    lcd.setCursor(0,0);
    lcd.print("                ");  
}
if(micros() - (spindleEndTime / 1000.0) > 5000){
    lcd.setCursor(0,1);
    lcd.print("                ");  
}

// When no magnets are read, clear the display and display a friendly message
if(!pausedScreen){
  if((millis() - (feedEndTime /1000.0) > 10000) && (millis() - (spindleEndTime /1000.0) > 10000)){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" This is boring!");
    lcd.setCursor(0,1);
    lcd.print(" Do Something");
    pausedScreen = true;
    }
  
}
delay(1); // delay in between reads for stability
}

// This method resets the various variables used while collecting the FEED magnet reads
void resetFeedVar(){
  feedStart = 0;
  feedStart = micros();
  feedOnState = false;
  feedCount = 1;
  timePassedFeed = 0.0;
}

// This method resets the various variables used while collecting the SPINDLE magnet reads
void resetSpindleVar(){
  spindleStart = 0;
  spindleStart = micros();
  spindleOnState = false;
  spindleCount = 1;
  timePassedSpindle = 0.0;
}

/* 
 This method is used to update the display once the magnet count thresholds are reached.
 You must pass an integer parameter that indicates which sensor you are working with.

 0 = the FEED sensor 
 1 = the SPINDLE sensor
*/
void updateDisplay(int row){
  float rpmVal = 0.0;
  if(row == 0){
    feedEndTime = micros();
    timePassedFeed = ((feedEndTime - feedStart)/1000000.0);
    rpmVal = (feedCount/timePassedFeed)*60.0;
    lcd.setCursor(0, 0);
    lcd.print("FEED ");
    lcd.print(int(rpmVal / 20)); // jerry add divide by number
    lcd.print(" .001/M          "); // jerry minutes?
  } else {
    spindleEndTime = micros();
    timePassedSpindle = ((spindleEndTime - spindleStart)/1000000.0);
    rpmVal = (spindleCount/timePassedSpindle)*60.0;
    lcd.setCursor(0, 1);
    lcd.print("SPINDLE ");
    lcd.print(int(rpmVal));
    lcd.print(" RPM         ");
  }
}
