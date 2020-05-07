/*
 * GSMSimHTTP Example
 * 
 * GSMSim_HTTP.ino
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
  Get... METHOD:GET|HTTPCODE:200|LENGTH:30
  Get with SSL and read returned data... METHOD:GET|HTTPCODE:200|LENGTH:30|DATA:This test data return from GET
  Post... METHOD:POST|HTTPCODE:200|LENGTH:54
  Post with SSL and read returned data... METHOD:POST|HTTPCODE:200|LENGTH:54|DATA:This test data return from POST.<br>Hello Erdem Arslan
  Close GPRS... 1

*/

#include <GSMSimHTTP.h>

// You can use any Serial interface. I recommended HardwareSerial. Please use the library with highiest baudrate.
// In examples, i used HardwareSerial. You can change it anymore.

#define RESET_PIN 10 // you can use any pin.

static volatile int num = 0;

GSMSimHTTP http(Serial1, RESET_PIN); // GSMSimHTTP inherit from GSMSimGPRS. You can use GSMSim and GSMSimGPRS methods with it.

void setup() {
  Serial1.begin(115200); // If you dont change module baudrate, it comes with auto baudrate.

  while(!Serial1) {
    ; // wait for module for connect.
  }

  Serial.begin(115200); // Serial for debug...

  // Init module...
  http.init(); // use for init module. Use it if you dont have any valid reason.

  Serial.print("Set Phone Function... ");
  Serial.println(http.setPhoneFunc(1));
  //delay(1000);

  Serial.print("is Module Registered to Network?... ");
  Serial.println(http.isRegistered());
  //delay(1000);

  Serial.print("Signal Quality... ");
  Serial.println(http.signalQuality());
  //delay(1000);

  Serial.print("Operator Name... ");
  Serial.println(http.operatorNameFromSim());
  //delay(1000);


  //Serial.print("GPRS Init... ");
  //Serial.println(http.gprsInit("internet")); // Its optional. You can set apn, user and password with this method. Default APN: "internet" Default USER: "" Default PWD: ""
  //delay(1000);


  Serial.print("Connect GPRS... ");
  Serial.println(http.connect());
  //delay(1000);

  Serial.print("Get IP Address... ");
  Serial.println(http.getIP());
  delay(1000);
  
 
  Serial.print("Get... ");
  Serial.println(http.get("sera.erdemarslan.com/test.php"));
  delay(1000);

  Serial.print("Get with SSL and read returned data... ");
  Serial.println(http.getWithSSL("erdemarslan.com/test.php", true));
  delay(1000);

  Serial.print("Post... ");
  Serial.println(http.post("sera.erdemarslan.com/test.php", "name=Erdem&surname=Arslan", "application/x-www-form-urlencoded"));
  delay(1000);

  Serial.print("Post with SSL and read returned data... ");
  Serial.println(http.post("erdemarslan.com/test.php", "name=Erdem&surname=Arslan", "application/x-www-form-urlencoded", true));
  delay(1000);

  Serial.print("Close GPRS... ");
  Serial.println(http.closeConn());
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