#include "Altimeter.h"
//#include "GPS.h"
//#include "Radio.h"
#include "ServoTimer2.h"
int state = 0;

float period = 50.0;
float DRIVE_PERIOD = 100;
long GROUND_LEVEL = 0.0; //change
long MIN_FLIGHT_LEVEL = 7.0; //meters //change and change sea level
long DETACT_PARA_HEIGHT = 1.524; //meters

bool isFlying = true;
bool isDropping = false;
bool isGrounded = false;
  
void setup() 
{
    //place component setup here 
    altimeterSetup();
    //GPSsetup();
    //radioSetup();
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
   }
}
