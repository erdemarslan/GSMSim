/*
 * GSMSimCall Example
 * 
 * GSMSim_Call.ino
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


/*
	### Example Serial Output ###

	Set Phone Function... 1
	is Module Registered to Network?... 1
	Signal Quality... 13
	Operator Name... Turk Telekom
	Init Call... 1
	Call any number... 1
	1. STATUS:RINGING|NUMBER:xxxxxxxxxxxx
	2. STATUS:BUSY|NUMBER:xxxxxxxxxxxx

*/

#include <GSMSimCall.h>

// You can use any Serial interface. I recommended HardwareSerial. Please use the library with highiest baudrate.
// In examples, i used HardwareSerial. You can change it anymore.

#define RESET_PIN 10 // you can use any pin.

static volatile int num = 0;

GSMSimCall call(Serial1, RESET_PIN); // GSMSimCall inherit from GSMSim. You can use GSMSim methods with it.

void setup() {
	Serial1.begin(115200); // If you dont change module baudrate, it comes with auto baudrate.

	while(!Serial1) {
		; // wait for module for connect.
	}

	Serial.begin(115200); // Serial for debug...

	// Init module...
	call.init(); // use for init module. Use it if you dont have any valid reason.

	Serial.print("Set Phone Function... ");
	Serial.println(call.setPhoneFunc(1));
	delay(1000);

	Serial.print("is Module Registered to Network?... ");
	Serial.println(call.isRegistered());
	delay(1000);

	Serial.print("Signal Quality... ");
	Serial.println(call.signalQuality());
	delay(1000);

	Serial.print("Operator Name... ");
	Serial.println(call.operatorNameFromSim());
	delay(1000);

	Serial.print("Init Call... ");
	Serial.println(call.initCall()); // Its optional but highly recommended. Some function work with this function.
	delay(1000);
  
	Serial.print("Call any number... ");
	Serial.println(call.call("xxxxxxxxxxxxx"));
	//delay(1000);

	// For other methods please look at readme.txt file.

}

void loop() {
	
	// Use your Serial interface...
	if(Serial1.available()) {
	    String buffer = "";
	    buffer = Serial1.readString();
	    num = num + 1;
	    Serial.print(num);
	    Serial.print(". ");

	    // This example for how you catch incoming calls.
	    if(buffer.indexOf("+CLCC:") != -1) {
	    	Serial.println(call.readCurrentCall(buffer));
	    } else {
	    	Serial.println(buffer);
	    }
	}
	
	// put your main code here, to run repeatedly:
}