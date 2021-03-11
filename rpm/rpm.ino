#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int feedPin = 2; // digital pin 2 is the hall pin
int spindlePin = 3;
// set number of hall trips for RPM reading (higher improves accuracy)
float hall_thresh = 5.0;
unsigned long start;
float feedCount = 1.0;
  
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // make the hall pin an input:
  pinMode(feedPin, INPUT);
        // set up the LCD's number of columns and rows:
      lcd.begin(16, 2);
      lcd.setCursor(0,0);
      lcd.write("Hello");
}



// the loop routine runs over and over again forever:
void loop() {
  // preallocate values for tach
  feedCount = 1.0;

  start = 0;
  start = micros();
  bool on_state = false;
  // counting number of times the hall sensor is tripped
  // but without double counting during the same trip
  while(true){
      if (digitalRead(feedPin)==0){
        if (on_state==false){
          on_state = true;
          feedCount+=1.0;
        }
      } else{
        on_state = false;
      }
      if (feedCount>=hall_thresh){
        break;
      }
  }
  updateDisplay(); 
}

void updateDisplay(){
  // print information about Time and RPM
  unsigned long end_time = micros();
  float time_passed = ((end_time-start)/1000000.0);
Serial.print("Time Passed: ");
Serial.print(time_passed);
Serial.println("s");
  float rpm_val = (feedCount/time_passed)*60.0;
  Serial.print(rpm_val);
  Serial.println(" RPM");
  delay(1);        // delay in between reads for stability
  //Print to lcd
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hello Jerry");
      lcd.setCursor(0, 1);
      lcd.print("FEED ");
      lcd.print(rpm_val);
      lcd.print(" RPM");
}
