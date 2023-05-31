#include <Arduino.h>
#include <SoftwareSerial.h>

//Définition des pins
#define statePin 2
#define rxPin 3 //Pin Rx du HC05 --> Pin 3 de la Nano
#define txPin 4 //Pin Tx du HC05 --> Pin 4 de la Nano
#define keyPin 5

//Définition des baudrates des communications séries
#define hc05BR 38400
#define nanoBR 9600

bool attente=true;
String choice;
int incomingByte;

SoftwareSerial hc05(txPin, rxPin); //Tx | Rx pdv HC05

void setup()
{
  Serial.begin(nanoBR);
  pinMode(keyPin, OUTPUT);

  digitalWrite(keyPin, LOW); attente=false;


  hc05.begin(hc05BR);
}

void loop()
{
 if (hc05.available()) Serial.write(hc05.read());
 if (Serial.available()) {incomingByte=Serial.read(); Serial.write(incomingByte); hc05.write(incomingByte);}
}

