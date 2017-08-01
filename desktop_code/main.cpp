#include <iostream>
#include <thread>
#include <cmath>
#include <chrono>

#include <stdio.h>
#include <fcntl.h>

#include <SerialStream.h>
#include "../Arduino/sketch_jul17b/config.h"
#include "joystick.h"

using namespace LibSerial;

// TODO set deadzones
// TODO put curves on joystick axis

template<typename T, typename B>
T clamp(T x, B a, B b) {
	if (x < a) x = a;
	if (x > b) x = b;
	return x;
}
template<typename T, typename B>
T step(T x, B a) {
	if (x > a) x = 1;
	else x = 0;
	return x;
}

int main() {
	char indices[4] = {0,1,2,3};
	short data[4];
	for (auto& d : data)
		d = CHANNEL_DEFAULT_VALUE;

	SerialStream arduino("/dev/ttyUSB0", std::ios::out);
	arduino.SetBaudRate(SerialStreamBuf::BAUD_9600);

	Joystick js("/dev/input/js1");
	JoystickEvent jse;

	while (1) {

		for (unsigned char i = 0; i < 4; i=i+1) {
			arduino.write(indices + i, 1);
			arduino.write((char*)(data + i), 2);
		}

		// Get joystick values
		while (js.sample(&jse)) {

			// We're only sending the sticks
			if (jse.isAxis()) {

				int n = jse.number;
				float invert = 1.f;
				float shift = 0.;
				float scale = 1.;

				// Remap numbers
				     if (n == 0) {n = 1; shift = .08; }              // left stick vertical
				else if (n == 1) {n = 0; shift = .03; invert = -1; } // left stick horizontal
				else if (n == 3) {n = 3; shift = .025; }             // right stick horizontal
				else if (n == 4) {n = 2; invert = -1; }              // right stick vertical
				else n = 4;

				if (n < 4) {
					double range = scale * invert * (jse.value/float(jse.MAX_AXES_VALUE) + shift);

					range = range * step(fabs(range), .09);
					int sgn = range > 0 ? 1 : -1;
					// range = 2.*std::tanh(std::tanh(std::tanh(std::tanh(std::max(0.,fabs(range) - .09)*sgn))));
					range = std::max(0.,fabs(range) - .09)*sgn;
					range = range*range*range;

					data[n] = CHANNEL_DEFAULT_VALUE + (CHANNEL_MAXIMUM_VALUE - CHANNEL_MINIMUM_VALUE) / 2 * range;
					data[n] = clamp(data[n], CHANNEL_MINIMUM_VALUE, CHANNEL_MAXIMUM_VALUE);
				}
			}
		}

		// no need to write too often
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
	}

	return 0;


	// Measure latency

	int x;
	std::cin >> x;

	js.sample(&jse);
	data[0] = CHANNEL_MAXIMUM_VALUE;
	while (1) {
		arduino.write(indices, 1);
		arduino.write((char*)(data), 2);
	}

	return 0;
}
