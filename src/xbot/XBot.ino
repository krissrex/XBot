
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

// Current state
STATE state = ST_NORMAL;

// port expander i2c address
const int mcp_addr = 0;
// port expander
Adafruit_MCP23008 mcp;
long long timeClock;

void setup() {
    // put your setup code here, to run once:
    mcp.begin(mcp_addr);
    mcp.pinMode(3, OUTPUT);
    timeClock=millis();
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
