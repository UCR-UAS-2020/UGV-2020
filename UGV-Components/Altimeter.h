#pragma once

#include <SPI.h>
#include <SoftwareSerial.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>
#include <Adafruit_BMP3XX.h>
#include <bmp3.h>
#include <bmp3_defs.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>


void altimeterSetup();

double altimeterReturn();

bool detectIfAboveAlt(double);
