#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
/* spindle
 * feed
*/

int feedPin = 2; // digital pin 2 is the hall pin
int spindlePin = 3; // pin 3 is for the spindle hall sensor
float feedThresh = 50.0; // set number of hall trips for feed RPM reading
float spindleThresh = 50.0; // set number of hall trips for spindle RPM reading
float startFeedTime = micros();
float startSpindleTime = micros();
float feedCount = 1;
float spindleCount = 1;
bool onStateFeed = false;
bool onStateSpindle = false;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  pinMode(feedPin, INPUT); // make the hall pin an input:
  pinMode(spindlePin, INPUT); 
  feedPin = HIGH;
  spindlePin = HIGH;
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows:
}

/*
 * This method will update the display one row at a time. Updating the display
 * one row at a time is necessary because the RPM of the Feed and Spindle are differnt 
 * and as a result will refresh the current RPM values at different times.  The parameter
 * sent to displayOut will reference the line number to update.  0 = top line, 
 * 1 = bottom line. The total width of the display is 16, need to truncate as required. 
 * 
 * Example:
 * 
 * Spindle = xxx
 * Feed = xxx
 * 
 */
 
void displayOut(int lineNum){
/*  
 * The first line of the display is the spindle RPM output,  
 * the second line of the display is the feed RPM output.
 */
   int rowNum = lineNum;
   if (rowNum == 0){
    // print information about spindle RPM
    float end_time = micros();
    float time_passed = ((end_time-startSpindleTime)/1000000.0);
    float rpm_val = (feedCount/time_passed)*60.0;
    Serial.print("Spindle ");
    Serial.println(rpm_val);
    delay(1);        // delay in between reads for stability
  
    //Print to lcd
    lcd.setCursor(0, 1);
    lcd.print("SPINDLE ");
    lcd.println(rpm_val);
    
   } else if (rowNum == 1){
    // print information about feed RPM
    float end_time = micros();
    float time_passed = ((end_time-startFeedTime)/1000000.0);
    float rpm_val = (feedCount/time_passed)*60.0;
    Serial.print("Feed ");
    Serial.println(rpm_val);
    delay(1);        // delay in between reads for stability
  
    //Print to lcd
    lcd.setCursor(0, 0);
    lcd.print("FEED ");
    lcd.println(rpm_val);
  }
}  

void resetDisplay(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("   Idle!");
  lcd.println(" Do Something!");
}

void loop() {
// process feed hall sensor
  if (digitalRead(feedPin)==0){
    if (onStateFeed==false){
      onStateFeed = true;
      feedCount+=1.0;
    }
  } else{
    onStateFeed = false;
  }
// Once the number of data reads exceeds the threshold set at the top, calc the avg and display
  if (feedCount>=feedThresh){
    displayOut(1);
    // reset the counters and time because the threshold was met
    startFeedTime = micros();
    onStateFeed = false;
    feedCount = 1;
  }

// process the spindle hall sensor
  if (digitalRead(spindlePin) == 0) {
    if (onStateSpindle == false){
      onStateSpindle = true;
      spindleCount += 1;
    }
  } else {
    onStateSpindle = false;
  }
// Once the number of data reads exceeds the threshold set at the top, calc the avg and display
  if (spindleCount >= spindleThresh){
    displayOut(0);
    // reset the counters and time because the threshold was met
    startSpindleTime = micros();
    onStateSpindle = false;
    spindleCount = 1;
  }



  
/* 
 *Reset the display to blank if the rpm reaches zero for one second
 *
 *I only picked on value to determine if the display should reset
 *you can add more logic here to determine if both are idle, but I 
 *assumed that if one was idle, both were idle.
 */
  if (millis() - (startFeedTime * 1000) > 1000){
    resetDisplay();
  }
}
