//Bibliotheken einbinden:
#include <Wire.h>
#include <EVShield.h>
#include <EVs_EV3Color.h>

EVShield evshield(0x34, 0x36);

EVs_EV3Color colorR;  //erster  Farbsensor heisst colorR
EVs_EV3Color colorL;  //zweiter Farbsensor heisst colorL

int SPEED = 15;
int MULTIPLIKATOR = 8;
void setup() {
  evshield.init(SH_HardwareI2C);

  colorR.init(&evshield, SH_BAS1);          //rechter Farbsensor ist an Port BAS1
  colorR.setMode(MODE_Color_MeasureColor);  //rechter Farbsensor sucht nach Farbe

  colorL.init(&evshield, SH_BBS1);          //linker Farbsensor ist an Port BBS1
  colorL.setMode(MODE_Color_MeasureColor);  //linker Farbsensor sucht nach Farbe

  // Warten auf GO-Knopf
  Serial.begin(9600);
  Serial.println("Start");
  while (!evshield.getButtonState(BTN_GO)) {}
  evshield.bank_a.ledSetRGB(0, 0, 0);
  evshield.bank_b.ledSetRGB(0, 0, 0);
}

void loop() {
  int farbeR = colorR.getVal();
  int farbeL = colorL.getVal();

  int delta = farbeL - farbeR;
  int s = delta * MULTIPLIKATOR;
  fahreOhneBremse(s + SPEED, -s + SPEED, 1);

  

  if (farbeR == 3) {
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
  }
  
}

/********************************************
              Funktionen
********************************************/


void fahre(int links, int rechts, int zeit) {
  evshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, -links);
  evshield.bank_b.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, -rechts);
  delay(zeit);
  evshield.bank_b.motorStop(SH_Motor_1, SH_Next_Action_Brake);
  evshield.bank_b.motorStop(SH_Motor_2, SH_Next_Action_Brake);
}

void fahreOhneBremse(int links, int rechts, int zeit) {
  evshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, -links);
  evshield.bank_b.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, -rechts);
  delay(zeit);
}