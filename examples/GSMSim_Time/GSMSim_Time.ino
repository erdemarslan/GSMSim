#include <GSMSim.h>

#define RX 7
#define TX 8
#define RESET 2
#define BAUD 9600


GSMSim gsm;

/*
 * Also you can this types:
 * GSMSim gsm(RX, TX);
 * GSMSim gsm(RX, TX, RESET);
 * GSMSim gsm(RX, TX, RESET, LED_PIN, LED_FLAG);
 */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println("GSMSim Library - Get and Set (from Internet) Time Example");
  Serial.println("");
  delay(1000);

  gsm.start(); // baud default 9600
  //gsm.start(BAUD);

  // Get module date time
  Serial.print("Module DateTime: ");
  Serial.println(gsm.timeGetRaw());

  Serial.print("Connect to Internet: ");
  // For setting time from ntp servers, we must connect to internet!
  Serial.println(gsm.gprsConnectBearer());

  Serial.print("Set timezone to +3: ");
  Serial.println(gsm.timeSetServer(3)); // time server is: 202.120.2.101 (Shanghai Jiaotong University - China)
  //Serial.println(gsm.timeSetServer(3, "202.120.2.101")); // you can set server too :)

  Serial.print("Sync from server. Server response: ");
  Serial.println(gsm.timeSyncFromServer());

  Serial.print("Module DateTime after sync: ");
  Serial.println(gsm.timeGetRaw());

  // Close internet connection
  Serial.print("Close Internet: ");
  Serial.println(gsm.gprsCloseConn());

}

void loop() {
  // put your main code here, to run repeatedly:

}
