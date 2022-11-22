//Bibliotheken einbinden:
#include <Wire.h>
#include <EVShield.h>
#include <EVs_EV3Color.h>

EVShield evshield(0x34, 0x36);

EVs_EV3Color colorR;  //erster  Farbsensor heisst colorR
EVs_EV3Color colorL;  //zweiter Farbsensor heisst colorL

int SPEED = 255;
void setup() {
  pinMode(2, OUTPUT);   //Pinout linker Motor
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(5, OUTPUT);   //Pinout rechter Motor
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  evshield.init(SH_HardwareI2C);

  colorR.init(&evshield, SH_BBS1);          //rechter Farbsensor ist an Port BBS1
  colorR.setMode(MODE_Color_MeasureColor);  //rechter Farbsensor sucht nach Farbe

  colorL.init(&evshield, SH_BBS2);          //linker Farbsensor ist an Port BBS
  colorL.setMode(MODE_Color_MeasureColor);  //linker Farbsensor sucht nach Farbe

  // Warten auf GO-Knopf
  Serial.begin(9600);
  //Serial.println("Start");
  while (!evshield.getButtonState(BTN_GO)) {}
  evshield.bank_a.ledSetRGB(0, 0, 0);
  evshield.bank_b.ledSetRGB(0, 0, 0);
}

void loop() {
  int r = colorR.getVal();
  int l = colorL.getVal();


  if ((l == 6 && r == 6) || (l == 1 && r == 1)) m(SPEED, SPEED, 0);
  else if (l == 1) m(SPEED, -SPEED, 0);
  else if (r == 1) m(-SPEED, SPEED, 0);


 /* if (farbeR == 3) {
    fahre(0, 0, 1);
    delay(2000);
    fahre(40, 40, 70);
    delay(2000);
    if (farbeL == 3) {                       // Deadend
      evshield.bank_a.ledSetRGB(0, 100, 0);
      evshield.bank_b.ledSetRGB(0, 100, 0);
      fahre(100, -100, 9000);
    } else {                                // normales Grün
      evshield.bank_a.ledSetRGB(0, 100, 0);
      fahre(100, 100, 300);
      fahre(100, -100, 150);
      while (colorR.getVal() == 6) fahreOhneBremse(50, 0, 1);
      fahre(100, -100, 50);
    }
  }

  if (farbeL == 3) {
    fahre(100, 100, 300);
    fahre(-100, 100, 150);
    evshield.bank_a.ledSetRGB(0, 0, 100);
    while (colorL.getVal() == 6) fahreOhneBremse(-50, 0, 1);
    fahre(-100, 100, 50);
  }*/
  
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
  }
  else {
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
  }

  if (right > 0) {
    digitalWrite(2, LOW);
    digitalWrite(4, HIGH);
  }
  else {
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