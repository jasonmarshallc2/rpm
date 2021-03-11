#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
void setup() {
  // put your setup code here, to run once:
  lcd.setCursor(0,0);
  lcd.write("hello");
}

void loop() {
  // put your main code here, to run repeatedly:

}
