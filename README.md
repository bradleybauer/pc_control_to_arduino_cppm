This project enables your computer to control the sticks on an RC transmitter that accepts PPM input, such as the Taranis QX7.

My setup:

	Pc connects to arduino through standard usb cable

	One end of a 3.5mm audio cable is cut and then wired into the arduino digital out pin and ground pin

	The other of the 3.5mm audio cable is plugged into the RC transmitter

For CPPM generation on an Arduino Uno

https://github.com/xythobuz/Saitek-X52-PPM

LibSerial for easy serial communication with the arduino

http://libserial.sourceforge.net/

Super easy linux joystick api borrowed from

https://github.com/legacy-roboime/ann-camera-calibration/
