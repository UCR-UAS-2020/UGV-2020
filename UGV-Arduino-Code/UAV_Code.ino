#include "Altimeter.h"
#include "GPS.h"
#include "Radio.h"
#include "ServoTimer2.h"
int state = 0;
double currentLat();
double currentLon();

float period = 50.0;
float DRIVE_PERIOD = 100;
long GROUND_LEVEL = 0.0; //change
long MIN_FLIGHT_LEVEL = 7.0; //meters //change and change sea level
long DETACT_PARA_HEIGHT = 1.524; //meters
static const double COURSE_LAT = 33.975849, COURSE_LON = -117.326131;  //destination

bool isFlying = true;
bool isDropping = false;
bool isGrounded = false;

enum UGVControlState{CONTROL_START, CONTROL_AUTO, CONTROL_MANUAL, CONTROL_STOP};
enum UGVManualState{COMMAND_STOP, COMMAND_FORWARD, COMMAND_BACK, COMMAND_TURNL, COMMAND_TURNR};
UGVControlState control_state;
UGVManualState manual_state;
char currState;
  
void setup() 
{
    //place component setup here 
    altimeterSetup();
    GPSsetup();
    radioSetup();
}

void loop() 
{
   switch(state){
    case 0:
    {
      //flying state
      isFlying = detectIfAboveAlt(MIN_FLIGHT_LEVEL);
      
      if(!isFlying)
      {
        Serial.println("\nState 1");
        state = 1;
      }
      Serial.println("\nStill state 0");
      delay(500);
      break;
    }
    case 1:
    {
      Serial.println("\nWe are here now");
      //dropping state
      if(isGrounded)
      {
        state = 2;
      }
      delay(500);
      break;
    }
    case 2:
    {
      //reorient
      break;
    }
    case 32: //manual drive
    {
      //stand-by
      currentLat = getLat();
      currentLon = getLon();

      ManualStateMachine(stateChange());
      //motors are in a stop state
    }
    case 33: //manual drive
    {
      //forward
    }
    case 34: //manual drive
    {
      //turnRight
    }
    case 35: //manual drive
    {
      //turnLeft
    }
    
   }
}

void CommandStateMachine(char command_state)
{
  if(command_state == 0)
  {
    control_state = CONTROL_START;
  }
  else if(command_state == 1)
  {
    control_state = CONTROL_AUTO;
  }
  else if(command_state == 2)
  {
    control_state = CONTROL_MANUAL;
  }
  else if(command_state == 3)
  {
    control_state = CONTROL_STOP;
  }
}

void ManualStateMachine(char manual_state)
{
  if(command_state == 0)
  {
    manual_state = COMMAND_STOP;
  }
  else if(command_state == 1)
  {
    manual_state = COMMAND_FORWARD;
  }
  else if(command_state == 2)
  {
    manual_state = COMMAND_BACK;
  }
  else if(command_state == 3)
  {
    manual_state = COMMAND_TURNL;
  }
  else if(command_state == 4)
  {
    manual_state = COMMAND_TURNR;
  }
}

 
