#pragma once
#include "Altimeter.h"
//comment
//demonstation
#define cs_pin 10    // chip (slave) select
#define sdo_pin 12   // MISO
#define sdi_pin 11  // MOSI
#define sck_pin 13   // SPI Clock Pin
#define sealvl 1013.25  // 1013.25 mbar (101.325 kPa; 29.921 inHg; 760.00 mmHg) 

double seaToGround = 281;
double bound = 1.524;

//Adafruit_BMP3XX bmpCreate() {
Adafruit_BMP3XX bmp(cs_pin);
//	return bmp;
//}

void altimeterSetup()
{
	//Adafruit_BMP3XX bmp(cs_pin);

	Serial.begin(115200);

	while (!Serial);
	Serial.begin(115200);

	// check if BMP 388 sensor is connected
	if (!bmp.begin()) {
		Serial.println("Altimeter Sensor not connected.");
		while (true);
	}
}

double altimeterReturn()
{
	if (!bmp.performReading()) {
		Serial.println("Could not read data");
		return -1.0;
	}

	Serial.println("Altitude Reading: ");
	double altimeter_data = bmp.readAltitude(sealvl) - seaToGround;
	Serial.print(altimeter_data);
	
	if (altimeter_data <= bound) { //test purposes
		Serial.println("Within 5 feet.");
	}

	return altimeter_data;
}

bool detectIfAboveAlt(double bound)
{
	int count = 0;
	double altimeter_data = altimeterReturn();
	for (int i = 0; i < 5; i++) {
		if (altimeter_data <= bound) { //take count of how many of 5 tests is within bound
			count++;
		}
	}
	return (count <= 4);
}


