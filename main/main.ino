//Bibliotheken einbinden:
#include <Wire.h>
#include <EVShield.h>
#include <EVs_EV3Color.h>

EVShield evshield(0x34, 0x36);

EVs_EV3Color colorR;  //erster  Farbsensor heisst colorR
EVs_EV3Color colorL;  //zweiter Farbsensor heisst colorL

int s = 30;  //s steht fuer Geschwindigkeit(speed)

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
}

void loop() {
  if (colorR.getVal() == 6) {
    Serial.println("rechts weiss");
  }

  if (colorR.getVal() == 1) {
    Serial.println("rechts schwarz");
  }

  if (colorL.getVal() == 6) {
    Serial.println("links weiss");
  }

  if (colorL.getVal() == 1) {
    Serial.println("links schwarz");
  }
}

/********************************************
              Funktionen
********************************************/


void fahre(int links, int rechts, int zeit) {
  if (links > 100) {
    links = 100;
  }
  if (rechts > 100) {
    rechts = 100;
  }
  if (links < -100) {
    links = -100;
  }
  if (rechts < -100) {
    rechts = -100;
  }
  evshield.bank_b.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, -links);
  evshield.bank_b.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, -rechts);
  delay(zeit);
  evshield.bank_b.motorStop(SH_Motor_1, SH_Next_Action_Brake);
  evshield.bank_b.motorStop(SH_Motor_2, SH_Next_Action_Brake);
}

void fahreOhneBremse(int links, int rechts, int zeit) {
  if (links > 100) {
    links = 100;
  }
  if (rechts > 100) {
    rechts = 100;
  }
  if (links < -100) {
    links = -100;
  }
  if (rechts < -100) {
    rechts = -100;
  }
}    