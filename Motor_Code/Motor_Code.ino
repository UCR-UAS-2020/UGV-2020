//#include "Motor.h"
//#include "/* Compass */"



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
//unsigned motorSpeed = 0;
int keyIn = 0;
int currDrive = 0;

void setup() {
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

int motorState = 3;
double locAngleFromOrientation = 0.0;

void loop() {
  switch (motorState) {
    case 0:   // stop
      motorStop();
      // check if the stop is due to reorientation or new destination
      break;
    case 1:   // straight
      motorStraight();
      // check Orientation
      break;
    case 2:   // turning UGV L if neg angle, R if pos angle
//      turnUGV(/* locAngle, UGV_Angle */);
      // check Orientation, return true if orientation matches destination
      break;
    case 3:
      UGVManual();
      break;
  }
}

// Constantly check orientation
// If orientation is not correct (after 5 checks to make sure it's not a misread), stop and readjust
// Keep checking until destination angle and UGV front angle match, then go straight

void motorStop() {
  analogWrite(LeftEn, 0);
  analogWrite(RightEn, 0);
}
void motorStraight() {
  analogWrite(LeftEn, 100);
  analogWrite(RightEn, 100);
  digitalWrite(LeftMotorOUTA, HIGH);
  digitalWrite(LeftMotorOUTB, LOW);
  digitalWrite(RightMotorOUTA, HIGH);
  digitalWrite(RightMotorOUTB, LOW);
}
void turnUGV(const double& locAngle, const double& UGV_Angle) {
  /*
   *
   * if (    )  // always points north, get locAngle and UGV angle to equal each other to stop turning
   *            // figure out which way to turn based on angle
   *
   */
   analogWrite(LeftEn, 100);
   analogWrite(RightEn, 100);
//   once done turning, motorState = straight
/* Right Turn */

   digitalWrite(LeftMotorOUTA, HIGH);
   digitalWrite(LeftMotorOUTB, LOW);
   digitalWrite(RightMotorOUTA, LOW);
   digitalWrite(RightMotorOUTB, HIGH);

/* Left Turn */
   digitalWrite(LeftMotorOUTA, LOW);
   digitalWrite(LeftMotorOUTB, HIGH);
   digitalWrite(RightMotorOUTA, HIGH);
   digitalWrite(RightMotorOUTB, LOW);
}

//void checkOrientatoin(const double& locAngle, const double& UGV_Angle) {
//  if ((fabs(locAngle - UGV_Angle)) >= 0.001) { // if not oriented correctly
//    for (unsigned i = 0; i < 5; i++ ) {
//      if ((fabs(locAngle - UGV_Angle)) <= 0.001) { // correct orientation -> misread
//        for (unsigned j = 0; j < 3; j++) {  // check if the "misread" is a misread
//          if ((fabs(locAngle - UGV_Angle)) >= 0.001) {  // if the "misread" is a misread, break out of this loop
//            i = 0;
//            break;
//          }
//          else if (j == 2) {  // if this runs, that means all "second check" is correct and the "misread" is an actual misread
//            return;
//          }
//        }
//      }
//    }
//
//  }
//
//}

/* void checkOrientation () { // EDIT
 *  if locAngle and UGVAngle don't match,
 *    for (i = 0; i < 5; i++) {check 5 times if it still doesn't match. return if it does match}
 *    switch machine to stop
 *
 *
 *
 *  }
 */

void UGVManual () {
  if (Serial.available() > 0) {
    keyIn = Serial.read();
  }
  else {return;}
  
  if (keyIn == 'p') {
    Serial.println("Exiting manual");
    motorState = 0; 
    return;
  }
  if (keyIn == 'w') {
    if (currDrive == 'w') {
      currDrive = 0;
      Serial.println("Stop Forward");
      motorStop();
    }
    else {
      Serial.println("Going Forward");
      currDrive = 'w';
      motorStraight();
    }
  } else if (keyIn == 'a') {
    if (currDrive == 'a') {
      currDrive = 0;
      Serial.println("Not Lefting");
      motorStop();
    }
    else {
      Serial.println("Turning Left");
      currDrive = 'a';
      analogWrite(LeftEn, 100);
      analogWrite(RightEn, 100);
      digitalWrite(LeftMotorOUTA, LOW);
      digitalWrite(LeftMotorOUTB, HIGH);
      digitalWrite(RightMotorOUTA, HIGH);
      digitalWrite(RightMotorOUTB, LOW);
    }
  } else if (keyIn == 'd') {
    if (currDrive == 'd') {
      currDrive = 0;
      Serial.println("Not Righting");
      motorStop();
    }
    else {
      Serial.println("Turning Right");
      currDrive = 'd';
      analogWrite(LeftEn, 100);
      analogWrite(RightEn, 100);
      digitalWrite(LeftMotorOUTA, HIGH);
      digitalWrite(LeftMotorOUTB, LOW);
      digitalWrite(RightMotorOUTA, LOW);
      digitalWrite(RightMotorOUTB, HIGH);
    }
  } else {return;}
}
