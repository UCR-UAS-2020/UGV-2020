#pragma once
#include "GPS.h"

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// For stats that happen every 5 seconds
unsigned long last = 0UL;

void GPSsetup()
{
	Serial.begin(115200);
	ss.begin(GPSBaud);
	Serial.print("Starting GPS");
}

double calculateDistance(double lat, double lon)
{
	while (ss.available() > 0)
	{
		gps.encode(ss.read());
	}

	if (millis() - last > 5000)
	{
		Serial.println();
		if (gps.location.isValid())
		{
			double distanceToFinish = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), lat, lon);

			Serial.print(F("Finish     Distance="));
			Serial.print(distanceToFinish / 1000, 6);
			Serial.print(F(" km Course-to="));

			return distanceToFinish;
		}
		if (gps.charsProcessed() < 10)
		{
		Serial.println("WARNING: No GPS data.  Check wiring.");
		return -1.0;
		}

		last = millis();
		Serial.println();
	}
}

double calculateDegree(double lat, double lon)
{
	while (ss.available() > 0)
	{
		gps.encode(ss.read());
	}

	if (millis() - last > 5000)
	{
		Serial.println();
		if (gps.location.isValid())
		{
			double courseToFinish = TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), lat, lon;

			Serial.print(courseToFinish, 6);
			Serial.print(F(" degrees ["));
			Serial.print(TinyGPSPlus::cardinal(courseToFinish));
			Serial.println(F("]"));
			
			return courseToFinish;
		}
		if (gps.charsProcessed() < 10)
		{
			Serial.println("WARNING: No GPS data.  Check wiring.");
			return -1.0;
		}

		last = millis();
		Serial.println();
	}
}

double getLon()
{
	return gps.location.lng();
}

double getLat()
{
	return gps.location.lat();
}