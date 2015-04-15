
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

short sov=0,vask=0,spis=0;

int lastError = 0;
const int MAX_SPEED = 400;
// Set constants for PID control
const float KP = 0.5;  // Proportional constant
const float KD = 6;    // Derivative constant
const int SV = 2500; // Set-value for position (in the middle of sensors)
int svingeRetning=0;


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
ZumoReflectanceSensorArray reflectanceSensors;
Pushbutton button(ZUMO_BUTTON);

long long timeClock;

void setup() {
    // put your setup code here, to run once:
    mcp.begin(mcp_addr);
    mcp.pinMode(3, OUTPUT); // Blue led for testing

    btSerial.begin(9600); // BT baud rate

    timeClock=millis(); // Initial time
    calibrate();
}

void calibrate()
{
reflectanceSensors.init();
button.waitForButton();
delay(1000);
  int i;
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
      
    reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0,0);
  
  button.waitForButton();
}

void loop() {
    // put your main code here, to run repeatedly:
    updateBehov();
    NewTone(3, 440, 100);
    mcp.digitalWrite(3, HIGH);
    delay(100);
    mcp.digitalWrite(3, LOW);
    delay(300);
    
    switch(state)
    {
      case ST_NORMAL:
      vanilla();
      break;
      case ST_SEARCH_LINE:
      find_line();
      break;
      case ST_SEARCH_AREA:
      check_zones();
      follow_line();

    }
    
}

void updateBehov()
{
  double deltaTime=(millis()-timeClock)/1000.0;
  sov+= 4*deltaTime;
  vask+= 3*deltaTime;
  spis+= 6*deltaTime;
  timeClock=millis();
  
}


void pollBluetooth(){
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
  
sov=0;
delay(5000);
}

void behov_spis(){
spis=0;
delay(3000);
  
}

void behov_vask(){
vask=0;
delay(2500);
}

void follow_line()
{

  unsigned int sensors[6];

  int pv = reflectanceSensors.readLine(sensors);
  int error = pv - SV; 
  int speedDifference = KP*error + KD * (error - lastError);
 
  lastError = error;
  int m1Speed = MAX_SPEED + speedDifference;
  int m2Speed = MAX_SPEED - speedDifference;

  if (m1Speed < 0)
    m1Speed = 0;
  if (m2Speed < 0)
    m2Speed = 0;
  if (m1Speed > MAX_SPEED)
    m1Speed = MAX_SPEED;
  if (m2Speed > MAX_SPEED)
    m2Speed = MAX_SPEED;

  if(m1Speed<m2Speed)
     svingeRetning--;
  else if(m1Speed>m2Speed)
     svingeRetning++;

  motors.setSpeeds(m1Speed, m2Speed);
}

void find_line()
{
unsigned int sensors[6];
motors.setSpeeds(MAX_SPEED,MAX_SPEED);
reflectanceSensors.readLine(sensors);

for(int i=0;i<6;i++)
  {  
    if(sensors[i]<200)
      state=ST_SEARCH_AREA;
  }

}

void check_zones()
{
  if(zone==ZONE_SLEEP&&sov>=100)
    behov_sov();
  else if(zone==ZONE_EAT&&spis>=100)
    behov_spis();
  else if(zone==ZONE_WASH&&vask>=100)
    behov_vask();
    
  if(sov<100&&spis<100&&vask<100)
    {
      if(svingeRetning>0)
       motors.setSpeeds(MAX_SPEED,MAX_SPEED/2);
       else
       motors.setSpeeds(MAX_SPEED/2,MAX_SPEED);
       
       delay(2000);
       state=ST_NORMAL;
       motors.setSpeeds(0,0);
    }  
    
}
int currentSpeed1=MAX_SPEED,currentSpeed2=MAX_SPEED;
void vanilla()
{
   unsigned int sensors[6];
   reflectanceSensors.readLine(sensors); 
   
   for(int i=0;i<6;i++)
  {  
    if(sensors[i]<200)
      {
        motors.setSpeeds(MAX_SPEED,-MAX_SPEED);
        delay(400);
        motors.setSpeeds(MAX_SPEED,MAX_SPEED);
        delay(500);//for at den ikke skal se sensorene igjen med en gang
      }
  }
  if(sov>=100||spis>=100||vask>=100)
    state=ST_SEARCH_LINE;
   
}
