 /*
 Bot 1 har ansvaret for å printe output på LCD-skjerm, samt være en sone.
 LCD bør utnyttes til debug under utvikling, ettersom serial ikke kan kalles pga hardware-limitasjoner med antall rx/tx-pinner.
 Ved ferdigstilling burde LCD brukes som UI 
 */
 
 /*The circuit (hentet ifra en eller annen webside som plab-wikien linker til):
 * LCD RS pin to digital pin 2
 * LCD Enable pin to digital pin 3
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

#include "PLabInternet.h"
#include <LiquidCrystal.h>

//Gir LCD-botten senderID 1
PLab_Internet mySocket(1);
//Boolean som holder styr på om en bot har funnet zumoen. Kan gjøres langt schmudere.
boolean isFound = false;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2,3,4,5,6,7);

//Pinnene til ultralyd
const int triggerPin = 8;
const int echoPin = 9;
 
void setup() {
  mySocket.onReceive(myMessageHandler);
  // Definer hvilken type de forskjellige pinnene er
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  //LCD initialiseres med 16 tegn og to linjer.
  lcd.begin(16, 2);
  
}

void myMessageHandler(byte senderID, String message) // Event handler
{
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
  }
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
    mySocket.sendMessage(0, "funnet1");
    lcd.clear();
    lcd.print("IN ZONE 1!");
  }
  //Om ingen av bottene har funnet zumo, skriv ut til display noe panisk.
  else if(!isFound){
    lcd.clear();
    lcd.print("WHERE IS IT?!");
  }
  //Resetter isFound, om zumo f.eks. har forflyttet seg.
  isFound = false;
  delay(100);
}
