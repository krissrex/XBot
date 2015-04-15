 /*
 BOT 3 EN SONE SOM IKKE ER DEFINERT ENDA
 Består av en ultralydsensor og bluetoothmaster. Bluetooth er ikke koblet enda.
 Rx/tx brukes til nettverk, ikke serial.
 */

// ZONE 3 - VASKE-ZONE
 

/*MERK AT INTERNETPINNENE (Rx/Tx) BRUKES OGSÅ AV SERIAL. ERGO KAN IKKE BÅDE SERIAL OG INTERNETT VÆRE I BRUK.*/ 
#include "PLabInternet.h"

//Gir bluetoothbot senderID = 3.
PLab_Internet mySocket(3);

//Angir hvilke pins trigpin og echopin til ultralyd er koblet til.
const int triggerPin = 8;
const int echoPin = 9;





void setup() {
  //Om botten mottar en melding, kall myMessageHandler
  mySocket.onReceive(myMessageHandler);
  // Definer hvilken type de forskjellige pinnene er
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}


//Håndterer meldinger med en gang de kommer inn via Rx. PASS PÅ AT SERIAL IKKE KALLES NOENSTEDS I KODEN.
void myMessageHandler(byte senderID, String message) // Event handler
{
  /*
  if(senderID == 1 && message == "funnet"){

  }
  */
}
 

void loop() {
  //Oppdaterer internettsokkelen. Kaller myMessageHandler om melding er mottatt.
  mySocket.update();
  
  //Kode for ultralyd, hentet ifra forelesningseksempel (mener det skal finnes et Ping-library et sted...)
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
   
  digitalWrite(triggerPin, LOW);
  // Mål hvor lang tid det tar før ekkoet kommer tilbake
  long duration = pulseIn(echoPin, HIGH);
  // Regn ut avstanden
  float distance = duration * 0.00017;  
  
  //Om zumo er innen 6cm rekkevidde, varsle om at den er funnet.
  if (distance < 0.06){
    //Sender melding til bot 1 (LCD-botten) om at zumo er funnet.
    mySocket.sendMessage(1,"funnet");
    /*Send over bluetooth til zumo at den må stoppe om den leter etter stasjon 0 + handlinger deretter*/
    //Sett inn et delay slik at botten ikke jammer nettverket med masse "funnet"-meldinger.
    delay(500);
  }
  
  //Delay mellom hver iterasjon, kortere delay = flere ultralydsjekker, og zumo oppdages raskere.
  delay(100);
}
