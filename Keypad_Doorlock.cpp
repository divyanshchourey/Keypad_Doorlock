#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ctype.h>

#define ADDRESS 63
#define LCD_R 16
#define LCD_C 3

const byte ROWS = 4;
const byte COLS = 4;
int i = 0, j = 0;
char str[16];

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad myKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(ADDRESS, LCD_R, LCD_C);

class Doorlock {
private:
  int delayForLock;
  //don't waste space in creating long array
  char correct[16] = { '1', '2', '3', '4' };
public:
  Doorlock() {}
  Doorlock(int delayForLock) {
    this->delayForLock = delayForLock;
  }

  void init() {
  }

  void enter(char s[]) {
    lcd.clear();

    int result = strcmp(correct, s);
    if (result == 0) {
      //---------------------------------- yaha tak ho gya hai
      lcd.print("Pin matched");
      digitalWrite(11, HIGH);
      //Serial.write(delayForLock);
      delay(this->delayForLock);
      digitalWrite(11, LOW);

    } else {
      lcd.print("Incorrect pin");
    }
  }
  
  void changePassword(){
  	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter the correct pin: ");
    delay(1000);
    
    char myKeys = myKeypad.getKey();
    Doorlock doorlock(5000);

    if (myKeys) {
      if (((myKeys - '0' <= 9) && (myKeys - '0' >= 1)) || (myKeys - '0' == 0)) {
        str[i] = (myKeys);
        //Serial.print(myKeys);
        lcd.setCursor(i, 0);
        lcd.print(myKeys);
        i++;
      } else if (myKeys - '0' == -13) {
        i = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Matching.");
        delay(1000);
        doorlock.enter(str);
      } else {
        Serial.print("Other");
      }
    }
    
  }
};

Doorlock doorlock(5000);

void setup() {

  lcd.begin(LCD_R, LCD_C);
  lcd.init();
  lcd.backlight();

  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  Serial.begin(9600);
}

void loop() {
  char myKeys = myKeypad.getKey();

  if (myKeys) {
    if (((myKeys - '0' <= 9) && (myKeys - '0' >= 1)) || (myKeys - '0' == 0)) {
      str[i] = (myKeys);
      //Serial.print(myKeys);
      lcd.setCursor(i, 0);
      lcd.print(myKeys);
      i++;
    } else if (myKeys - '0' == -13) {
      i = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Matching.");
      delay(1000);
      doorlock.enter(str);

    } else if (myKeys - '0' == -6) {
      i = 0;
      for(int a=0; a<15; a++){
        str[a] = 0;  
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cleared");
      delay(1000);
      lcd.clear();
    } else if (myKeys - '0' == 17) {
      i = 0;
      for(int a=0; a<15; a++){
        str[a] = 0;  
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Change password");
      delay(1000);
      doorlock.changePassword();
      lcd.clear();
    } else {
      Serial.print("Other");
    }
  }
}
