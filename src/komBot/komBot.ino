/* 
KomBot er kommunikasjons bot.
Addresse: 0

*/

#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#include "PLabInternet.h"
#include <LiquidCrystal.h>


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


/*------------Bluetooth---------*/
// Se https://github.com/IDI-PLab/Lecture-examples/blob/master/Forelesning_9_Bluetooth_master/PLAB_BT_Binding.ino

/*
Arduino  HC-05
GND    ---  GND
Pin 10  ---  RXD
Pin 11  ---  TXD
Pin 9  ---  KEY
Pin 8  ---  VCC
*/

#define rxPin 10
#define txPin 11
#define keyPin 9
#define pwrPin 8


/*-----------Buttons----------*/
const int btn_select = 13;  // blue square button
const int btn_next = 12;    // yellow round button



PLabBTSerial btSerial(txPin, rxPin);

//Gir bluetoothbot senderID = 0.
PLab_Internet mySocket(0);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2,3,4,5,6,7);



void setup()
{
    //Om botten mottar en melding, kall myMessageHandler
	  mySocket.onReceive(myMessageHandler);

    //Set up buttons
    pinMode(btn_select, INPUT);
    pinMode(btn_next, INPUT);


    //LCD initialiseres med 16 tegn og to linjer.
    lcd.begin(16, 2);
  	
	  btSetup(); // Setter opp bt for master
	  run(); //kjører BT pairing
}


void loop()
{
	
}



// Bluetooth master mode setup, så den kan sende meldinger
void btSetup(){
  	pinMode(pwrPin, OUTPUT);
    pinMode(keyPin, OUTPUT);
    digitalWrite(pwrPin,LOW);
    digitalWrite(keyPin,LOW);
    myDelay(5000);
    digitalWrite(keyPin,HIGH);
    myDelay(500);
    digitalWrite(pwrPin,HIGH);   
  
    btSerial.begin(38400); // Baud rate starter på 38400 før pairing.
}


//Håndterer meldinger med en gang de kommer inn via Rx. PASS PÅ AT SERIAL IKKE KALLES NOENSTEDS I KODEN.
void myMessageHandler(byte senderID, String message) // Event handler
{
  //Kode for LCD- og ???sone-bot, om den melder at zumo er der.
  if(senderID == 1 && message == "funnet"){
    /*Send over bluetooth til zumo at den må stoppe om den leter etter stasjon 1 + handlinger deretter*/
    btSerial.write("1");
  }
  //Kode for ???sone-bot, om den melder at zumo er der.
  if(senderID == 2 && message == "funnet"){
    btSerial.write("2");
  }

  if (senderID == 3 && message == "funnet"){
    btSerial.write("3");
  }
}


void myDelay(int milli) {
  lcd.clear();

  for (int i = 0; i < milli; i+=500) {
    lcd.print(".");
    delay(500);
  };
  lcd.clear();
}

void echo() {
      lcd.setCursor(1,0);
      lcd.print("                ");
      lcd.setCursor(1,0);

      lcd.print(":");
      while (btSerial.available()) {
       char c = btSerial.read();
       lcd.write(c);
      };
};


String stringArr[10];
int noOfBTSlavesFound = 0;

void run() {
    int buttonState = digitalRead(btn_next);

    while (buttonState != HIGH) {
      buttonState = digitalRead(btn_next);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Push2start pair");
      delay(2000);
    }

    btSerial.write("AT+ORGL\r\n"); delay(100); echo();
    btSerial.write("AT+RMAAD\r\n"); delay(100); readIfDataBT();
    btSerial.write("AT+PSWD=1234\r\n"); delay(100); readIfDataBT();
    btSerial.write("AT+ROLE=1\r\n"); delay(100); readIfDataBT();
    btSerial.write("AT+CMODE=0\r\n"); delay(100); readIfDataBT();
    btSerial.write("AT+INIT\r\n"); delay(100); readIfDataBT();  
    btSerial.write("at+inqm=1,10,4\r\n"); delay(100); readIfDataBT();
    btSerial.write("AT+INQ\r\n"); delay(500); readIfDataBT();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Srch 20s for BT:"); myDelay(20000);
    
    noOfBTSlavesFound = 0;
    String myString = "";
 		//   while (myString.indexOf("OK") < 0) {
    while (btSerial.available()) {
         myString = readBT();
         stringArr[noOfBTSlavesFound] = myString;
         noOfBTSlavesFound++;
      	// Print the incomming data to console
 		//        Serial.print("BT Unit code: ");
 		//        Serial.println(myString);
    }

	//  noOfBTSlavesFound--;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Found "); lcd.write(noOfBTSlavesFound);
    for (int i=0; i < noOfBTSlavesFound; i++) {
        int c1 = stringArr[i].indexOf(':');
      	int c2 = stringArr[i].indexOf(':', c1+1);
      	int c3 = stringArr[i].indexOf(':', c2+1);
      	int c4 = stringArr[i].indexOf(',', c3+1);  
 	 	    String s1 = stringArr[i].substring(c1+1,c2); 
    	  String s2 = stringArr[i].substring(c2+1,c3);
		    String s3 = stringArr[i].substring(c3+1,c4);       
 		//     Serial.println(stringArr[i]);
		//     Serial.println(s1+","+s2+","+s3);
      	String s = "AT+RNAME?"+s1+","+s2+","+s3;
      	char chars[s.length()];
      	s.toCharArray(chars,s.length()+1);
		//      Serial.println(chars);
      	btSerial.write(chars); btSerial.write("\r\n");
        lcd.clear();
        lcd.setCursor(0,0);
      	lcd.print("Get names(10s):"); myDelay(10000);
        lcd.clear();
        lcd.setCursor(0,0);
      	lcd.print("BT Unit ");
      	lcd.print(i+1); lcd.print(": ");

      	while (btSerial.available()) {
        	int availableCount = btSerial.available();
        	if (availableCount > 0) {
         	// Create a char array (c-string) to hold the incomming data.
        	char text[availableCount];
           	// Read the incomming data. bufferSize refers to the buffer you are giving as an argument
           	btSerial.read(text, availableCount);
           	String myString = String(text);
        	// Print the incomming data to console
            lcd.setCursor(1,0);
            lcd.print("                ");
            lcd.setCursor(1,0);         
           	lcd.print(myString);
            delay(300);   
          }
       }  
    }

    if (noOfBTSlavesFound > 0) {
      	Serial.print("Type number of BT unit to pair (1 - ");
  		Serial.print(noOfBTSlavesFound); Serial.println("):");

      	while (!Serial.available()) { } // wait for user to type... 
  		char c = Serial.read(); delay(500);

      	while (Serial.available()) {  // wait for user to type... 
     		char temp = Serial.read();
      	}

		int id = c - '0';
		Serial.println(id);
		int i = id - 1;
		int c1 = stringArr[i].indexOf(':');
		int c2 = stringArr[i].indexOf(':', c1+1);
		int c3 = stringArr[i].indexOf(':', c2+1);
		int c4 = stringArr[i].indexOf(',', c3+1);  
		String s1 = stringArr[i].substring(c1+1,c2); 
		String s2 = stringArr[i].substring(c2+1,c3);
		String s3 = stringArr[i].substring(c3+1,c4);       
		//     Serial.println(stringArr[i]);
		//     Serial.println(s1+","+s2+","+s3);
		btSerial.write("AT+CMODE=0\r\n"); delay(100); readIfDataBT();
		String s = "AT+BIND="+s1+","+s2+","+s3;
		char chars[s.length()];
		s.toCharArray(chars,s.length()+1);
		Serial.println(chars);
		btSerial.write(chars); btSerial.write("\r\n");
		btSerial.write("AT+BIND?\r\n"); delay(100); echo();
		btSerial.write("AT+UART=9600,1,0\r\n"); delay(100); echo();
		btSerial.write("AT+UART?\r\n"); delay(100); echo();
		Serial.print("BT Unit "); 
		Serial.print(id); 
		Serial.println(" paired successfully. Please wait for it to connect."); 
		Serial.println("The red LED on the slave BT unit should stop blinking"); 
		digitalWrite(pwrPin,LOW);
		digitalWrite(keyPin,LOW);
		delay(1000);
		digitalWrite(pwrPin,HIGH);
		delay(1000);

		while (Serial.available()) {
			char c = Serial.read();
		} // wait for user to type... 

      	Serial.println("Type <enter> key to send test string STEPF to unit.");
		while (!Serial.available()) { } // wait for user to type... 

		c = Serial.read();
		btSerial.begin(9600);
		Serial.println("STEPF");     
		btSerial.write("STEPF\r\n");  
    }
} //end run()



void readIfDataBT() {
  int availableCount = btSerial.available();
  if (availableCount > 0) {
     // Create a char array (c-string) to hold the incomming data.
    char text[availableCount];
    // Read the incomming data. bufferSize refers to the buffer you are giving as an argument
    btSerial.read(text, availableCount);
    // Print the incomming data to console
 //   Serial.print("Received data: ");
 //   Serial.println(text);
  }
}

String readBT() {
  while(!btSerial.available()) {};
  int availableCount = btSerial.available();
  if (availableCount > 0) {
     // Create a char array (c-string) to hold the incomming data.
    char text[availableCount];
    for (int i=0; i < availableCount;i++) {text[i]=' ';};
    // Read the incomming data. bufferSize refers to the buffer you are giving as an argument
    btSerial.read(text, availableCount);
    // Print the incomming data to console
   // Serial.print("Received data: ");
   // Serial.println(text);
    return String(text);
  };
  return "";
}



