#include "Altimeter.h"
#include "GPS.h"
#include "Radio.h"
#include "ServoTimer2.h"
int state = 0;
double currentLat;
double currentLon;

float period = 50.0;
float DRIVE_PERIOD = 100;
long GROUND_LEVEL = 0.0; //change
long MIN_FLIGHT_LEVEL = 7.0; //meters //change and change sea level
long DETACT_PARA_HEIGHT = 1.524; //meters
static const double COURSE_LAT = 33.975849, COURSE_LON = -117.326131;  //destination

bool isFlying = true;
bool isDropping = false;
bool isGrounded = false;

enum UGVControlState {CONTROL_START, CONTROL_AUTO, CONTROL_MANUAL, CONTROL_STOP};
enum UGVManualState {COMMAND_STOP, COMMAND_FORWARD, COMMAND_BACK, COMMAND_TURNL, COMMAND_TURNR};
UGVControlState control_state;
char manual_state;
char currState;

//motor
#define LeftMotorOUTA 1
#define LeftMotorOUTB 2
#define LeftMotorINA 3    // c
#define LeftMotorINB 4    // cc
#define LeftEn 5
#define RightMotorOUTA 6  // c
#define RightMotorOUTB 7  // cc
#define RightMotorINA 8
#define RightMotorINB 9
#define RightEn 10

const unsigned MAXSPEED = 255;

void setup()
{
  //place component setup here
  altimeterSetup();
  GPSsetup();
  radioSetup();

  //motor setup
  Serial.begin(9600);
  /*    Left Motor Pins   */
  pinMode(LeftMotorOUTA, OUTPUT);   // M1
  pinMode(LeftMotorOUTB, OUTPUT);   // M2
  pinMode(LeftMotorINA, INPUT);     // OUTA     Reads out A pulses    clockwise (forward) - A first
  pinMode(LeftMotorINB, INPUT);     // OUTB     Reads out B pulses    counter clockwaise (backward) - B first
  pinMode(LeftEn, OUTPUT);          // Outputs into motor driver (left) the speed we want

  /*    Right Motor Pins    */
  pinMode(RightMotorOUTA, OUTPUT);    // M1
  pinMode(RightMotorOUTB, OUTPUT);    // M2
  pinMode(RightMotorINA, INPUT);      // OUTA     Reads out A pulses    clockwise (forward) - A first
  pinMode(RightMotorINB, INPUT);      // OUTB     Reads out B pulses    counter clockwaise (backward) - B first
  pinMode(RightEn, OUTPUT);           // Outputs into motor driver (right) the speed we want
}

void loop()
{
  switch (state) {
    case 0:
      {
        //flying state
        isFlying = detectIfAboveAlt(MIN_FLIGHT_LEVEL);

        if (!isFlying)
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
        if (isGrounded)
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

        manual_state = stateChange();
        if(manual_state == COMMAND_FORWARD)
        {
          state = 33;
        }
        else if(manual_state == COMMAND_TURNR)
        {
          state = 34;
        }
        else if(manual_state == COMMAND_TURNL)
        {
          state = 35;
        }

        motorStop();
        //motors are in a stop state
      }
    case 33: //manual drive
      {
         manual_state = stateChange();
         if(manual_state == COMMAND_STOP)
        {
          state = 32;
        }
        motorStraight();
        //forward
      }
    case 34: //manual drive
      {
        manual_state = stateChange();
         if(manual_state == COMMAND_STOP)
        {
          state = 32;
        }
        rTurnUGV();
        //turnRight
      }
    case 35: //manual drive
      {
        manual_state = stateChange();
         if(manual_state == COMMAND_STOP)
        {
          state = 32;
        }
        lTurnUGV();
        //turnLeft
      }

  }
}

void CommandStateMachine(char command_state)
{
  if (command_state == 0)
  {
    control_state = CONTROL_START;
  }
  else if (command_state == 1)
  {
    control_state = CONTROL_AUTO;
  }
  else if (command_state == 2)
  {
    control_state = CONTROL_MANUAL;
  }
  else if (command_state == 3)
  {
    control_state = CONTROL_STOP;
  }
}

void motorStop() 
{
  analogWrite(LeftEn, 0);
  analogWrite(RightEn, 0);
}

void motorStraight() 
{
  analogWrite(LeftEn, 100);
  analogWrite(RightEn, 100);
  digitalWrite(LeftMotorOUTA, HIGH);
  digitalWrite(LeftMotorOUTB, LOW);
  digitalWrite(RightMotorOUTA, HIGH);
  digitalWrite(RightMotorOUTB, LOW);
}

void lTurnUGV() {
  analogWrite(LeftEn, 100);
  analogWrite(RightEn, 100);
  digitalWrite(LeftMotorOUTA, LOW);
  digitalWrite(LeftMotorOUTB, HIGH);
  digitalWrite(RightMotorOUTA, HIGH);
  digitalWrite(RightMotorOUTB, LOW);
}
void rTurnUGV() {
  analogWrite(LeftEn, 100);
  analogWrite(RightEn, 100);
  digitalWrite(LeftMotorOUTA, HIGH);
  digitalWrite(LeftMotorOUTB, LOW);
  digitalWrite(RightMotorOUTA, LOW);
  digitalWrite(RightMotorOUTB, HIGH);
}
