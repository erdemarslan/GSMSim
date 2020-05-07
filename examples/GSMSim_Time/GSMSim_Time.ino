/*
 * GSMSimTime Example
 * 
 * GSMSim_Time.ino
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
  Signal Quality... 14
  Operator Name... Turk Telekom
  Connect GPRS... 1
  Get IP Address... xxx.xxx.xxx.xxx
  Get module date time... 04/01/01,01:16:04+00
  Set timezone and time server... 1
  Sync date time from server... TIME_SYNCHRONIZED_SUCCESS
  Get module date time after sycn... 20/05/07,16:20:59+00
  Close GPRS... 1

*/

#include <GSMSimTime.h>

// You can use any Serial interface. I recommended HardwareSerial. Please use the library with highiest baudrate.
// In examples, i used HardwareSerial. You can change it anymore.

#define RESET_PIN 10 // you can use any pin.

static volatile int num = 0;

GSMSimTime dateTime(Serial1, RESET_PIN); // GSMSimTime inherit from GSMSimGPRS. You can use GSMSim and GSMSimGPRS methods with it.

void setup() {
  Serial1.begin(115200); // If you dont change module baudrate, it comes with auto baudrate.

  while(!Serial1) {
    ; // wait for module for connect.
  }

  Serial.begin(115200); // Serial for debug...

  // Init module...
  dateTime.init(); // use for init module. Use it if you dont have any valid reason.

  Serial.print("Set Phone Function... ");
  Serial.println(dateTime.setPhoneFunc(1));
  //delay(1000);

  Serial.print("is Module Registered to Network?... ");
  Serial.println(dateTime.isRegistered());
  //delay(1000);

  Serial.print("Signal Quality... ");
  Serial.println(dateTime.signalQuality());
  //delay(1000);

  Serial.print("Operator Name... ");
  Serial.println(dateTime.operatorNameFromSim());
  //delay(1000);


  //Serial.print("GPRS Init... ");
  //Serial.println(dateTime.gprsInit("internet")); // Its optional. You can set apn, user and password with this method. Default APN: "internet" Default USER: "" Default PWD: ""
  //delay(1000);


  Serial.print("Connect GPRS... ");
  Serial.println(dateTime.connect());
  //delay(1000);

  Serial.print("Get IP Address... ");
  Serial.println(dateTime.getIP());
  delay(1000);


  Serial.print("Get module date time... ");
  Serial.println(dateTime.getRaw());
  delay(1000);

  Serial.print("Set timezone and time server... ");
  Serial.println(dateTime.setServer(3, "0.tr.pool.ntp.org")); // timezone +3
  delay(1000);

  Serial.print("Sync date time from server... ");
  Serial.println(dateTime.syncFromServer());
  delay(1000);

  Serial.print("Get module date time after sycn... ");
  Serial.println(dateTime.getRaw());
  delay(1000);

  Serial.print("Close GPRS... ");
  Serial.println(dateTime.closeConn());
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
      Serial.println(buffer);
  }
  
  // put your main code here, to run repeatedly:
}