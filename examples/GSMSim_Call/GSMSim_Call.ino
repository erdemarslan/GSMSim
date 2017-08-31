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

  Serial.println("GSMSim Library - Call Example");
  Serial.println("");
  delay(1000);

  gsm.start(); // baud default 9600
  //gsm.start(BAUD);

  char* phone_no = "+905123456789";

  Serial.println("Calling the number " + String(phone_no));
  Serial.println(gsm.call(phone_no));
  delay(10000);
  Serial.println("Call hang off");
  gsm.callHangoff();

  

  
}

void loop() {
  // put your main code here, to run repeatedly:
}
