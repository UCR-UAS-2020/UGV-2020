#include "Radio.h"

/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 915.0

// Where to send packets to!
#define DEST_ADDRESS   1
// change addresses for each client board, any number :)
#define MY_ADDRESS     2


#if defined (__AVR_ATmega32U4__) // Feather 32u4 w/Radio
#define RFM69_CS      8
#define RFM69_INT     7
#define RFM69_RST     4
#define LED           13
#endif

#if defined(ADAFRUIT_FEATHER_M0) // Feather M0 w/Radio
#define RFM69_CS      8
#define RFM69_INT     3
#define RFM69_RST     4
#define LED           13
#endif

#if defined (__AVR_ATmega328P__)  // Feather 328P w/wing
#define RFM69_INT     3  // 
#define RFM69_CS      4  //
#define RFM69_RST     2  // "A"
#define LED           13
#endif

#if defined(ESP8266)    // ESP8266 feather w/wing
#define RFM69_CS      2    // "E"
#define RFM69_IRQ     15   // "B"
#define RFM69_RST     16   // "D"
#define LED           0
#endif

#if defined(ESP32)    // ESP32 feather w/wing
#define RFM69_RST     13   // same as LED
#define RFM69_CS      33   // "B"
#define RFM69_INT     27   // "A"
#define LED           13
#endif

String radiopacketInput;
int16_t packetnum = 0;

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);

void radioSetup()
{
	Serial.begin(115200);

	pinMode(LED, OUTPUT);
	pinMode(RFM69_RST, OUTPUT);
	digitalWrite(RFM69_RST, LOW);

	Serial.println("Feather Addressed RFM69 TX Test!");
	Serial.println();

	// manual reset
	digitalWrite(RFM69_RST, HIGH);
	delay(10);
	digitalWrite(RFM69_RST, LOW);
	delay(10);

	if (!rf69_manager.init()) {
		Serial.println("RFM69 radio init failed");
		while (1);
	}
	Serial.println("RFM69 radio init OK!");
	// Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
	// No encryption
	if (!rf69.setFrequency(RF69_FREQ)) {
		Serial.println("setFrequency failed");
	}

	// If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
	// ishighpowermodule flag set like this:
	rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

	// The encryption key has to be the same as the one in the server
	uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
					  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
	rf69.setEncryptionKey(key);

	pinMode(LED, OUTPUT);

	Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
}

/*void ping()
{
	delay(1000);  // Wait 1 second between transmits, could also 'sleep' here!

	char radiopacket[20] = "Ping to Ground ";
	itoa(packetnum++, radiopacket + 13, 10);
	Serial.print("Sending "); 
	Serial.println(radiopacket);

	// Send a message!
	rf69.send((uint8_t*)radiopacket, strlen(radiopacket));
	rf69.waitPacketSent();

	// Now wait for a reply
	uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(buf);

	if (rf69.waitAvailableTimeout(500)) {
		// Should be a reply message for us now   
		if (rf69.recv(buf, &len)) {
			Serial.print("Got a reply: ");
			Serial.println((char*)buf);
			Blink(LED, 50, 3); //blink LED 3 times, 50ms between blinks
		}
		else {
			Serial.println("Receive failed");
		}
	}
	else {
		Serial.println("No reply, is another RFM69 listening?");
	}
}*/

char stateChange()  //rx recieving 
{
	char stateChanger;

	if (rf69.available()) {
		// Should be a message for us now   
		uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
		uint8_t len = sizeof(buf);
		if (rf69.recv(buf, &len)) {
			if (!len) return;
			buf[len] = 0;
			Serial.print("Received [");
			Serial.print(len);
			Serial.print("]: ");

			stateChanger = len;
			if (strstr((char*)buf, "0")) {
				// Send start mode
				uint8_t data[] = "start or stop";
				rf69.send(data, sizeof(data));
				rf69.waitPacketSent();
				Serial.println("Sent update");
				stateChanger = 0x30;  //ascii 0
			}
			if (strstr((char*)buf, "1")) {
				// Send auto mode
				uint8_t data[] = "auto or for";
				rf69.send(data, sizeof(data));
				rf69.waitPacketSent();
				Serial.println("Sent update");
				stateChanger = 0x31;  //ascii 1
			}
			if (strstr((char*)buf, "2")) {
				// Send manual mode
				uint8_t data[] = "manual or back";
				rf69.send(data, sizeof(data));
				rf69.waitPacketSent();
				Serial.println("Sent update");
				stateChanger = 0x32;  //ascii 2
			}
			if (strstr((char*)buf, "3")) {
				// Send stop mode
				uint8_t data[] = "stop or turnl";
				rf69.send(data, sizeof(data));
				rf69.waitPacketSent();
				Serial.println("Sent update");
				stateChanger = 0x33;  //ascii 3
			}
			if (strstr((char*)buf, "4")) {
				// Send stop mode
				uint8_t data[] = "turnr";
				rf69.send(data, sizeof(data));
				rf69.waitPacketSent();
				Serial.println("Sent update");
				stateChanger = 0x34;  //ascii 4
			}
		}
		else {
			Serial.println("Receive failed");
		}
	}
	return stateChanger;
}