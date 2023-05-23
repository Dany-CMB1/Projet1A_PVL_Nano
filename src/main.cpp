#include <Arduino.h>
#include <SoftwareSerial.h>

//Déclaration pins module BT
#define statePin 2
#define rxPin 3 //Pin Rx du HC05 --> Pin 3 de l'Arduino
#define txPin 4//Pin Tx du HC05 --> Pin 4 de l'Arduino
#define keyPin 5

#define CALIBRAGE_TEMPS 5
#define CALIBRAGE_POT 6
#define TOP_DEPART 7

//Déclaration des pins sur lesquels les capteurs sont branchés
int GLED = 4;
int Laser = 3;
int LightSensor = 1;
int Pot = 5;

//Initialisation des variables
bool currStatus;
bool prevStatus;
bool chrono = 0;
int nb_fd, nb_fm;
int seuil = 0, pot = 0;
double tps, start;

SoftwareSerial hc05(txPin, rxPin); //Tx | Rx pdv HC05

void setup()
{
  // Init Serial at 115200 Baud Rate 
  Serial.begin(115200); 
  //Déclaration de l'état des capteurs digitaux (entrée ou sortie)
  pinMode(Laser, OUTPUT); digitalWrite(Laser, HIGH); 
  
  pinMode(GLED, OUTPUT); 
  digitalWrite(GLED, LOW); 

  calibratePot();
  calibrateTime();
}


void loop()
{
  //Acquisition des états logiques des BO (1: passant, 0: bloqué)
  GetSensorStatus();

  //Détection des fd
  if (!currStatus && prevStatus) {nb_fd++;}

  //Détection des fm
  else if (currStatus && !prevStatus) {nb_fm++;}
  
  //Détection du 1er front montant de la BO de départ; démarrage du chrono
  if (nb_fm == 1 && !chrono) {chrono = 1; hc05.write(TOP_DEPART);}


}

//Unsigned long: 32 bis soit 4 octets mais serial.read() ne permet que d'écrire un octet
//Ecriture en plusieurs paquets d'un octet
void  writeUnsignedLong(unsigned long msg)
{
  for (int i=0; i<sizeof(unsigned long);i++) hc05.write(msg<<(sizeof(int)*i));
}

void GetSensorStatus()
{
  prevStatus = currStatus;
  //Lecture des valeurs analogiques des photodiodes
  //Conversion analogique --> logique
  if (analogRead(LightSensor)>seuil) currStatus=1; 
  else currStatus=0; 
}

void calibrateTime()
 {
  //Attente synchronisation demande calibrage temps
  while(!(hc05.available() && hc05.read()==CALIBRAGE_TEMPS)){hc05.write(CALIBRAGE_TEMPS);}

  writeUnsignedLong(millis());
 }

 void calibratePot()
 {
  //Attente synchronisation demande réglage potentiomètre
  while(!(hc05.available() && hc05.read()==CALIBRAGE_POT)){hc05.write(CALIBRAGE_POT);}

  //Attente réponse
  while(!hc05.available()){}
  seuil = hc05.read();
 }