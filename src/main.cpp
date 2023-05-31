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
#define FIN_COURSE 8
#define hc05BR 38400
#define nanoBR 9600

//Déclaration des pins sur lesquels les capteurs sont branchés
int GLED = 4;
int Laser = 6;
int LightSensor = 7;
int Pot = 5;

//Initialisation des variables
bool currStatus;
bool prevStatus;
bool chrono = 0;
int nb_fd=0, nb_fm=0;
int seuil = 600, pot = 0;
double tps, start;

SoftwareSerial hc05(txPin, rxPin); //Tx | Rx pdv HC05

void setup()
{
  // Init Serial at 115200 Baud Rate 
  Serial.begin(9600); 
  hc05.begin(hc05BR);
  //Déclaration de l'état des capteurs digitaux (entrée ou sortie)
  pinMode(Laser, OUTPUT); digitalWrite(Laser, HIGH); 
  pinMode(GLED, OUTPUT); 
  digitalWrite(GLED, LOW); 
}


void loop()
{
 /* calibratePot();
  Serial.println(seuil);*/
  //Acquisition des états logiques des BO (1: passant, 0: bloqué)
  GetSensorStatus();

  //Détection des fd
  if (!currStatus && prevStatus) {nb_fd++;}

  //Détection des fm
  else if (currStatus && !prevStatus) {nb_fm++;}
  
  //Détection du 1er front montant de la BO de départ; démarrage du chrono
  if (nb_fd >= 1 && !chrono) {
    chrono = 1; 
    hc05.write(TOP_DEPART);
    }
  //Serial.println(nb_fd);
  if(hc05.available() && hc05.read()==FIN_COURSE){
    nb_fd=0;
    nb_fm=0;
    chrono=0;
  }

}


void GetSensorStatus()
{
  prevStatus = currStatus;
  //Lecture des valeurs analogiques des photodiodes
  //Conversion analogique --> logique
  if (analogRead(LightSensor)>seuil) currStatus=1; 
  else currStatus=0; 
  Serial.println(analogRead(LightSensor));
}


 void calibratePot()
 {
  if (hc05.available()){
  String result=hc05.readString();
  seuil=result.toInt();
  Serial.println(seuil);
  }
}
