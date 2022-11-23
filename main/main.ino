//Bibliotheken einbinden:
#include <Wire.h>
#include <EVShield.h>
#include <EVs_EV3Color.h>

#define MS_PER_DEG 12

EVShield evshield(0x34, 0x36);

EVs_EV3Color colorR;  //first color sensors name is colorR
EVs_EV3Color colorL;  //second color sensors name is colorL

int SPEED = 255;
void setup() {
  pinMode(2, OUTPUT);  //Pinout left motor
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(5, OUTPUT);  //Pinout right motor 
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  evshield.init(SH_HardwareI2C);

  colorR.init(&evshield, SH_BBS2);          //right color sensor on port BBS2
  colorR.setMode(MODE_Color_MeasureColor);  //right color sensor searches for colors

  colorL.init(&evshield, SH_BBS1);          //left color sensor on port BBS1
  colorL.setMode(MODE_Color_MeasureColor);  //left color sensor searches for colors

  // Waiting for go button 
  Serial.begin(9600);
  //Serial.println("Go");
  while (!evshield.getButtonState(BTN_GO)) {
    evshield.bank_a.ledSetRGB(0, 255, 0);
    evshield.bank_b.ledSetRGB(0, 255, 0);
    delay(50);
    evshield.bank_a.ledSetRGB(0, 0, 0);
    evshield.bank_b.ledSetRGB(0, 0, 0);
    delay(50);
  }
}

void loop() {
  int r = colorR.getVal();
  int l = colorL.getVal();

  if ((l == 6 && r == 6) || (l == 1 && r == 1)) m(SPEED, SPEED, 0); 
  else if (l == 1) m(-SPEED, SPEED, 0);
  else if (r == 1) m(SPEED, -SPEED, 0);

  if (r == 3 || l == 3) {
    long time = millis();
    m(-255, -255, 0);
    while (millis() - time < 300) {
      if (colorR.getVal() == 1 || colorL.getVal() == 1) {
        m(255, 255, 400);
        break;
      }
    }
    m(255, 255, 300);
    bool green_left = false;
    bool green_right = false;
    time = millis();
    m(255, -255, 0);
    while (millis() - time < 100) {
      if (colorL.getVal() == 3) green_left = true;
      if (colorR.getVal() == 3) green_right = true;
    }
    m(-255, 255, 100);
    time = millis();
    m(-255, 255, 0);
    while (millis() - time < 100) {
      if (colorL.getVal() == 3) green_left = true;
      if (colorR.getVal() == 3) green_right = true;
    }
    m(255, -255, 100);

    if (green_left && green_right) m(-255, 255, MS_PER_DEG * 180);
    else if (green_left) {
      m(255, 255, 300);
      m(-255, 255, MS_PER_DEG * 90);
      m(255, 255, 100);
    } else if (green_right) {
      m(255, 255, 300);
      m(255, -255, MS_PER_DEG * 90);
      m(255, 255, 100);
    }
  }
}

/********************************************
              Funktionen
********************************************/

void m(int left, int right, int duration) {
  if (left < -255) left = -255;
  if (left > 255) left = 255;
  if (right < -255) right = -255;
  if (right > 255) right = 255;

  if (left > 0) {
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
  } else {
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
  }

  if (right > 0) {
    digitalWrite(2, LOW);
    digitalWrite(4, HIGH);
  } else {
    digitalWrite(2, HIGH);
    digitalWrite(4, LOW);
  }

  analogWrite(5, abs(left));
  analogWrite(3, abs(right));
  delay(duration);
  if (duration != 0) stop();
}

void stop() {
  analogWrite(5, 0);
  analogWrite(3, 0);
}