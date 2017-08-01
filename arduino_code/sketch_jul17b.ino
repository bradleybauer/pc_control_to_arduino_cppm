#include "cppm.h"

const int out = 8;
uint16_t data[4];

void setup() {

	for (auto& d : data)
		d = CHANNEL_DEFAULT_VALUE;

	pinMode(out, OUTPUT);

	Serial.begin(9600);

	CPPM::instance().init();
}

void loop() {

	// If there is atleast 5 bytes available, then there is gauranteed to be atleast 1 packet available.
	// A packet is:
	//	1 byte  = channel index. unsigned char in range [0,3]
	//  2 bytes = channel value. short int in range [CHANNEL_MINIMUM_VALUE, CHANNEL_MAXIMUM_VALUE]

	// Values sent from the PC are already in the correct range
	if (Serial.available() >= 5) {

		uint8_t index;
		// find an index
		while(Serial.available()) {
			index = Serial.read();
			if (index < 4)
				break;
		}
		if (index < 4) {

			uint8_t* data_bytes = (uint8_t*)(data+index);
			data_bytes[0] = Serial.read();
			data_bytes[1] = Serial.read();

			if (data[index] > CHANNEL_MAXIMUM_VALUE || data[index] < CHANNEL_MINIMUM_VALUE)
				data[index] = CHANNEL_DEFAULT_VALUE;

			// generate ppm signal representing data
			// CPPM utilizes interupts and so is independent of loop()
			CPPM::instance().copy(data);

		}
	}

	// Test this
	delay(1);
}

