/*
Den enkleste nettverksbotten, og mest sannsynlig blir det to av disse (kun bluetooth-bot og lcd-bot er to nødvendige, unike bots).
*/

#include "PLabInternet.h"

//Gi denne botten senderID 2.
PLab_Internet mySocket(2);

//Pinnene til ultrasound
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
}
 
void loop() {
  mySocket.update();
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
   
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
   
  digitalWrite(triggerPin, LOW);
  // Mål hvor lang tid det tar før ekkoet kommer tilbake
  long duration = pulseIn(echoPin, HIGH);
  // Regn ut avstanden
  float distance = duration * 0.00017;  
  
  //Si ifra til LCD-bot og bluetooth-bot at zumo er funnet her om den er innen 6cm.
  if (distance < 0.06){
    mySocket.sendMessage(1,"funnet");
    mySocket.sendMessage(0,"funnet");
    //Sett et delay for å unngå nettverksjam.
    delay(500);  
  }
  
  delay(100);
}
