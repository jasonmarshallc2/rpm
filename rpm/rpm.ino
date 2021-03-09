#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int hall_pin = 2; // digital pin 2 is the hall pin
float hall_thresh = 10.0; // set number of hall trips for RPM reading (higher improves accuracy)
float start = micros();
float hall_count = 1;
bool on_state = false;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  pinMode(hall_pin, INPUT); // make the hall pin an input:
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows:

}

void displayOut(){
  // print information about Time and RPM
  float end_time = micros();
  float time_passed = ((end_time-start)/1000000.0);
  float rpm_val = (hall_count/time_passed)*60.0;
  Serial.print(rpm_val);
  Serial.println(" RPM");
  delay(1);        // delay in between reads for stability

  //Print to lcd
  lcd.setCursor(0, 0);
  lcd.println("Hello Jerry");
  lcd.setCursor(0, 1);
  lcd.print("FEED ");
  lcd.print(rpm_val);
  lcd.println(" RPM");
}

void resetDisplay(){
  lcd.clear();
}

void loop() {

  if (digitalRead(hall_pin)==0){
    if (on_state==false){
      on_state = true;
      hall_count+=1.0;
    }
  } else{
    on_state = false;
  }
  if (hall_count>=hall_thresh){
    displayOut();
    start = micros();
    on_state = false;
    hall_count = 1;
  }

  if (millis() - (start * 1000) > 1000){
    resetDisplay();
  }
}
