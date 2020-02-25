#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

void GPSsetup();
double calculateDegree();
double calculateDistance();

#endif