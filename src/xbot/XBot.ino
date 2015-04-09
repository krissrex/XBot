
#include "PLab_ZumoMotors.h"
#include "Pushbutton.h"
#include "QTRSensors.h"
#include "ZumoBuzzer.h"
#include "ZumoMotors.h"
#include "ZumoReflectanceSensorArray.h"

#include <SoftwareSerial.h>
#include "PLabBTCallback.h"
#include "PLabBTSerial.h"

#include "utility/twi.h"
#include "Wire.h"
#include <Adafruit_MCP23008.h>

#include "NewTone.h"


// STATES :^)
#define ST_NORMAL 		1
#define ST_SEARCH_LINE  2
#define ST_SEARCH_AREA	3

// Current state
int state = ST_NORMAL;

// port expander i2c address
const int mcp_addr = 0;
// port expander
Adafruit_MCP23008 mcp;

void setup() {
    // put your setup code here, to run once:
    mcp.begin(mcp_addr);
    mcp.pinMode(3, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    NewTone(3, 440, 100);
    mcp.digitalWrite(3, HIGH);
    delay(100);
    mcp.digitalWrite(3, LOW);
    delay(300);
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