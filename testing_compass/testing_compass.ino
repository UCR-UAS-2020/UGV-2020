#include <math.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!

// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
double degreeOriented = 0;
String facing = "null";
#define LSM9DS1_SCK A5  //SCL on  Teensy
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4  //SDA on Teensy
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5
// You can also use software SPI
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_SCK, LSM9DS1_MISO, LSM9DS1_MOSI, LSM9DS1_XGCS, LSM9DS1_MCS);
// Or hardware SPI! In this case, only CS pins are passed in
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_XGCS, LSM9DS1_MCS);


void setupSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
}


void setup() 
{
  Serial.begin(115200);

  while (!Serial) {
    delay(1); // will pause Zero, Leonardo, etc until serial console opens
  }
  
  Serial.println("LSM9DS1 data read demo");
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
  Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");

  // helper to just set the default scaling we want, see above!
  setupSensor();
}

void loop() 
{
  lsm.read();  /* ask it to read in the data */ 

  /* Get a new sensor event */ 
  sensors_event_t a, m, g, temp;

  lsm.getEvent(&a, &m, &g, &temp); 

//  Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s^2");
//  Serial.print("\tY: "); Serial.print(a.acceleration.y);     Serial.print(" m/s^2 ");
//  Serial.print("\tZ: "); Serial.print(a.acceleration.z);     Serial.println(" m/s^2 ");

  Serial.print("Mag X: "); Serial.print(m.magnetic.x);   Serial.print(" gauss");
  Serial.print("\tY: "); Serial.print(m.magnetic.y);     Serial.print(" gauss");
//  Serial.print("\tZ: "); Serial.print(m.magnetic.z);     Serial.println(" gauss");

//  Serial.print("Gyro X: "); Serial.print(g.gyro.x);   Serial.print(" dps");
//  Serial.print("\tY: "); Serial.print(g.gyro.y);      Serial.print(" dps");
//  Serial.print("\tZ: "); Serial.print(g.gyro.z);      Serial.println(" dps");
  
  
  if(m.magnetic.x != 0){
    degreeOriented = atan2 (m.magnetic.y, m.magnetic.x) * 180 / PI;
  }
  else{
    if(m.magnetic.y < 0)
      degreeOriented = 90;
    else
      degreeOriented = 0;
  }

    
  if(degreeOriented > 360)
  {
    degreeOriented -= 360;
  }
  else if (degreeOriented < 0)
  {
    degreeOriented += 360;
  }
  
  if(degreeOriented >=  337.25 || degreeOriented <= 22.5)
    facing = "North";
  else if(degreeOriented >  292.5 && degreeOriented < 337.25)
    facing = "North-West";
  else if(degreeOriented >=  247.5 && degreeOriented <= 292.5)
    facing = "West";
  else if(degreeOriented >  202.5 && degreeOriented < 247.5)
    facing = "South-West";
  else if(degreeOriented >=  157.5 && degreeOriented <= 202.5)
    facing = "South";
  else if(degreeOriented >  112.5 && degreeOriented < 157.5)
    facing = "South-East";
  else if(degreeOriented >=  67.5 && degreeOriented <= 112.5)
    facing = "East";
  else if(degreeOriented >  22.5 && degreeOriented < 67.5)
    facing = "North-East";
   
    
  Serial.print("DIRECTION: "); Serial.println(degreeOriented); Serial.print("\t"); Serial.print(facing);
  

  Serial.println();
  delay(200);
}
