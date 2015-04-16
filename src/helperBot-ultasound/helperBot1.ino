 /*
 Bot 1 har ansvaret være en sone.
 */

// ZONE 1 - SOVE-SONE


#include "PLabInternet.h"


//Gir LCD-botten senderID 1
PLab_Internet mySocket(2);
//Boolean som holder styr på om en bot har funnet zumoen. Kan gjøres langt schmudere.
boolean isFound = false;


//Pinnene til ultralyd
const int triggerPin = 8;
const int echoPin = 9;
 
void setup() {
  mySocket.onReceive(myMessageHandler);
  // Definer hvilken type de forskjellige pinnene er
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void myMessageHandler(byte senderID, String message) // Event handler
{
  // Denne er ikke lengre LCD bot.

  /*
  //Om bot 0 sier at den har funnet zumo
  if(senderID == 0 && message == "funnet"){
    //Fjern all tekst ifra skjerm.
    lcd.clear();
    //Print denne meldingen til skjerm.
    lcd.print("IN ZONE 0!");
    //Sett variabel til funnet.
    isFound = true;
    //La meldingen vises litt lengre enn kun når zumo blir funnet, slik at det kan sees under debug.
    delay(500);
  }
  if(senderID == 2 && message == "funnet"){
    lcd.clear();
    lcd.print("IN ZONE 2!");
    isFound = true;
    delay(500);
  }*/
}
 
void loop() {
  //Oppdater internett, kall relevante funksjoner om det er nødvendig.
  mySocket.update();
  //Kode for ultralyd.
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  // Mål hvor lang tid det tar før ekkoet kommer tilbake
  long duration = pulseIn(echoPin, HIGH);
  // Regn ut avstanden
  float distance = duration * 0.00017;  
  
  //Om den er innen 6cm rekkevidde:
  if (distance < 0.06 && !isFound){
    //Si ifra til bluetoothbot at den er funnet (trenger ikke skrive funnet1 med senderID, kun funnet.), slik at den kan kommunisere med zumo.
    mySocket.sendMessage(0, "funnet");
  }
  //Resetter isFound, om zumo f.eks. har forflyttet seg.
  isFound = false;
  delay(100);
}
