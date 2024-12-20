#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ctype.h>
#include <EEPROM.h>

#define ADDRESS 63
#define LCD_R 16
#define LCD_C 4
#define MAX_ATTEMPT 5
#define WAIT 30

const byte ROWS = 4;
const byte COLS = 4;
int i = 0, j = 0;
int attempt = 1;
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
  char correct[16];
  char arr[16];
  // correct = EEPROM.get(10, arr);
public:
  Doorlock() {}
  Doorlock(int delayForLock) {
    this->delayForLock = delayForLock;
  }

  void init() {
  }


  void enter(char s[]) {
    Serial.print("enter called.");
    lcd.clear();
    Serial.println(EEPROM.get(10, arr));

    int result = strcmp(EEPROM.get(10, arr), s);
    if (result == 0) {
      lcd.print("Pin matched");
      digitalWrite(11, HIGH);
      for (int i = this->delayForLock / 1000; i >= 0; i--) {
        lcd.setCursor(0, 1);
        lcd.print(i);
        delay(1000);
      }
      lcd.clear();
      for (int a = 0; a < 15; a++) {
        str[a] = 0;
      }
      //Serial.write(delayForLock);
      //delay(this->delayForLock);
      digitalWrite(11, LOW);
      attempt = 0;

    } else {
      lcd.print("Incorrect pin");
      for (int a = 0; a < 15; a++) {
        str[a] = 0;
      }
      delay(1000);
      lcd.clear();
      Serial.println("Incorrect pin");

      if (attempt >= MAX_ATTEMPT) {
        Serial.print("Maximum attempts exceded");
        lcd.setCursor(0, 0);
        lcd.print("Try after");
        lcd.setCursor(0, 1);
        lcd.print("seconds");
        for (int i = WAIT; i > 0; i--) {
          attempt = 1;
          lcd.setCursor(10, 0);
          //lcd.clear();
          if (i < 10) {
            lcd.setCursor(10, 0);
            lcd.print("0");
          }
          lcd.print(i);
          delay(1000);
        }
      }
      Serial.println(attempt);
      attempt++;
      lcd.clear();
    }
  }



  void changePassword() {
    //Now that's something breathtaking
    bool flag1 = true, flag2 = true;
    //Serial.println("change password called");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter the");
    lcd.setCursor(0, 1);
    lcd.print("correct pin: ");
    delay(1000);
    lcd.clear();
    //Serial.println("After delay");

    while (flag2) {
      char myKeys = myKeypad.getKey();
      //Doorlock doorlock(5000);
      // ChangePassword cp;


      if (myKeys) {
        if (((myKeys - '0' <= 9) && (myKeys - '0' >= 1)) || (myKeys - '0' == 0)) {
          str[i] = (myKeys);
          //Serial.print("Key pressed");
          lcd.setCursor(i, 0);
          lcd.print(myKeys);
          i++;
        } else if (myKeys - '0' == -13) {
          // flag1 = false;

          i = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Matching.");
          delay(1000);
          //doorlock.enterForPasswordChange(str);
          // cp.enter(str);
          //Serial.println("enter of change password called");
          lcd.clear();

          int result = strcmp(EEPROM.get(10, arr), str);
          if (result == 0) {
            lcd.print("Pin matched.");
            delay(1000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Enter new");
            lcd.setCursor(0, 1);
            lcd.print("password: ");
            delay(1000);
            lcd.clear();
            lcd.setCursor(0, 0);
            char myKeys = myKeypad.getKey();
            Doorlock doorlock(5000);
            for (int a = 0; a < 15; a++) {
              str[a] = 0;
            }


            while (flag2) {
              myKeys = myKeypad.getKey();
              if (myKeys) {
                if (((myKeys - '0' <= 9) && (myKeys - '0' >= 1)) || (myKeys - '0' == 0)) {
                  str[i] = (myKeys);
                  //Serial.print(myKeys);
                  lcd.setCursor(i, 0);
                  lcd.print(myKeys);
                  i++;
                } else if (myKeys - '0' == -13) {
                  flag2 = false;
                  i = 0;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Password changed");
                  delay(1000);
                  //strcpy(EEPROM.get(10, arr), str);
                  EEPROM.put(10, str);
                  // for (int i = 0; i < 15; i++) {
                  //   Serial.print(correct[i]);
                  //   Serial.print(" ");
                  // }

                } else if (myKeys - '0' == -6) {
                  i = 0;
                  for (int a = 0; a < 15; a++) {
                    str[a] = 0;
                  }
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Cleared");
                  delay(1000);
                  lcd.clear();
                } else {
                  //TODO
                  Serial.print("Other");
                }
              }
            }

          } else {
            lcd.print("Incorrect pin");
            delay(1000);
            flag2 = false;
          }

        } else if (myKeys - '0' == -6) {
          i = 0;
          for (int a = 0; a < 15; a++) {
            str[a] = 0;
          }
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Cleared");
          delay(1000);
          lcd.clear();
        } else {
          //TODO
          Serial.print("Other");
        }
      }
    }
  }
};



Doorlock doorlock(5000);
void setup() {

  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);

  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("Welcome!");
  delay(1000);
  lcd.clear();
  
  for(int i=0; i<16; i++){
  	str[i] = 0;  
  }
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Enter password:");
  char myKeys = myKeypad.getKey();

  if (myKeys) {
    if (((myKeys - '0' <= 9) && (myKeys - '0' >= 1)) || (myKeys - '0' == 0)) {
      str[i] = (myKeys);
      //Serial.print(myKeys);
      lcd.setCursor(i, 1);
      lcd.print("*");
      i++;
    } else if (myKeys - '0' == -13) {
      i = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Matching.");
      delay(1000);
      if(str[0] == 0){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Invalid password");
        delay(1000);
        
        lcd.clear();
      	lcd.setCursor(0, 0);
      	lcd.print("Enter password");
      	lcd.setCursor(0, 1);
      	
      } else {
        doorlock.enter(str);
      }

    } else if (myKeys - '0' == -6) {
      i = 0;
      for (int a = 0; a < 15; a++) {
        str[a] = 0;
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cleared");
      delay(1000);
      lcd.clear();
    } else if (myKeys - '0' == 17) {
      Serial.println("User changing password.");
      i = 0;
      for (int a = 0; a < 15; a++) {
        str[a] = 0;
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Change password");
      delay(1000);
      doorlock.changePassword();
      lcd.clear();
    } else if (myKeys - '0' == 18) {
      Serial.println("Backspace");
      lcd.setCursor(i - 1, 1);
      lcd.print(" ");
      str[i - 1] = 0;
      i--;
    } else if (myKeys - '0' == 20) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("The password is: ");
      lcd.setCursor(0, 1);
      lcd.print(str);
      delay(1000);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter password");
      lcd.setCursor(0, 1);
      for(int i=0; i<16; i++){
      	int element = str[i];
        if(element){
        	lcd.setCursor(i, 1);
          	lcd.print("*");
        }
      }
      
    } else {
      Serial.print("Other");
    }
  }
}