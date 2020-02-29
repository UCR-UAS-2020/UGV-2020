#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

void GPSsetup();
double calculateDegree(double, double);
double calculateDistance(double, double);
double getLon();
double getLat();

#endif