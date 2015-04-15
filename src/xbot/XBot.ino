
#include "PLab_ZumoMotors.h"
#include "Pushbutton.h"
#include "QTRSensors.h"
#include "ZumoBuzzer.h"
#include "ZumoMotors.h"
//#include "ZumoReflectanceSensorArray.h"

#include <SoftwareSerial.h>
#include "PLabBTCallback.h"
#include "PLabBTSerial.h"

#include "utility/twi.h"
#include "Wire.h"
#include <Adafruit_MCP23008.h>

#include "NewTone.h"

short sov=0,vask=0,spis=0;


// STATES :^)

enum STATE {ST_NORMAL,ST_SEARCH_LINE,ST_SEARCH_AREA};

// ZONES
#define ZONE_NONE 0
#define ZONE_SLEEP 1
#define ZONE_EAT 2
#define ZONE_WASH 3

// Current state
STATE state = ST_NORMAL;

// Current zone
int zone = ZONE_NONE;

/*-------------Pins-----------*/
// port expander i2c address
const int mcp_addr = 0;
// Bluetooth receiver pins
const int bt_rx = 2;
const int bt_tx = 6;

/*-------------Objects-----------*/
// port expander
Adafruit_MCP23008 mcp;
// Bluetooth unit
PLabBTSerial btSerial(bt_tx, bt_rx);
// Motors
ZumoMotors motors;

long long timeClock;

void setup() {
    // put your setup code here, to run once:
    mcp.begin(mcp_addr);
    mcp.pinMode(3, OUTPUT); // Blue led for testing

    btSerial.begin(9600); // BT baud rate

    timeClock=millis(); // Initial time
}

void loop() {
    // put your main code here, to run repeatedly:
    NewTone(3, 440, 100);
    mcp.digitalWrite(3, HIGH);
    delay(100);
    mcp.digitalWrite(3, LOW);
    delay(300);
    
    switch(state)
    {
      case ST_NORMAL:
      break;
      case ST_SEARCH_LINE:
      break;
      case ST_SEARCH_AREA:
      break;

    }
    
}

void updateBehov()
{
  double timeNow=(millis()-timeClock)/1000.0;
//  sov+=
  
}


int pollBluetooth(){
	int result = 0;

	int availableCount = btSerial.available();
  	if (availableCount > 0) {
    	char text[availableCount];
    	btSerial.read(text, availableCount); 
    	readCommand(text);
  	}
}

void readCommand (char *text) {
	if (0 == strcmp("1", text)) {
    	zone = ZONE_SLEEP;
  	} else if (0 == strcmp("2", text)) {
    	zone = ZONE_EAT;
  	} else if (0 == strcmp("3", text)){
  		zone = ZONE_WASH;
  	} else {
  		// Blink led for unknown
  		mcp.digitalWrite(3, HIGH);
  		delay(100);
  		mcp.digitalWrite(3, LOW);
  		zone = ZONE_NONE;
  	}
}



void kos(){

}

void sint(){

}

void behov_sov(){

}

void behov_spis(){

}

void behov_vask(){

}
