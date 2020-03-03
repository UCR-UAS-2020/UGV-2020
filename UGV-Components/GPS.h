#pragma once

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

void GPSsetup();
double calculateDistance(double, double);
double calculateDegree(double, double);

double getLon();
double getLat();

