/*
 * GSMSim Example
 * 
 * GSMSim_Module_Info.ino
 *
 * By Erdem ARSLAN
 * Version: v.2.0.1
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

#include <GSMSim.h>

// You can use any Serial interface. I recommended HardwareSerial. Please use the library with highiest baudrate.
// In examples, i used HardwareSerial. You can change it anymore.

#define RESET_PIN 10 // you can use any pin.
GSMSim gsm(Serial1, RESET_PIN);

void setup() {
	Serial1.begin(115200); // If you dont change module baudrate, it comes with auto baudrate.

	while(!Serial1) {
		; // wait for module for connect.
	}

	Serial.begin(115200); // Serial for debug...

	// Init module...
	gsm.init(); // use for reseting module. Use it if you dont have any valid reason.

	// Module Manifacturer
	Serial.print("Module Manifacturer... ");
	Serial.println(gsm.moduleManufacturer());
	delay(1000);

	// Module Manifacturer
	Serial.print("Module Model... ");
	Serial.println(gsm.moduleModel());
	delay(1000);

	// Module Manifacturer
	Serial.print("Module Revision... ");
	Serial.println(gsm.moduleRevision());
	delay(1000);

	// Module Manifacturer
	Serial.print("Module IMEI... ");
	Serial.println(gsm.moduleIMEI());
	delay(1000);

	// For other methods please look at readme.txt file.

}

void loop() {
	
	// Use your Serial interface...
	if(Serial1.available()) {
		Serial.println(Serial1.readString());
	}
	
	// put your main code here, to run repeatedly:
}