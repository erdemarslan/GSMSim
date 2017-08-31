#include <GSMSim.h>
#include <SoftwareSerial.h>

#define RX 7
#define TX 8
#define RESET 2
#define BAUD 9600


GSMSim gsm(RX, TX, RESET);

/*
 * Also you can this types:
 * GSMSim gsm;
 * GSMSim gsm(RX, TX);
 * GSMSim gsm(RX, TX, RESET, LED_PIN, LED_FLAG);
 */


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  Serial.println("GSMSim Library - Module Information");
  Serial.println("");
  delay(1000);

  //gsm.start(); // baud default 9600
  gsm.start(BAUD);
  
  
  Serial.print("Manufacturer:\t\t");
  Serial.println(gsm.moduleManufacturer());
  delay(250);
  Serial.print("Model:\t\t\t");
  Serial.println(gsm.moduleModel());
  delay(250);
  Serial.print("Revision:\t\t");
  Serial.println(gsm.moduleRevision());
  delay(250);
  Serial.print("IMEI:\t\t\t");
  Serial.println(gsm.moduleIMEI());
  delay(250);
  Serial.print("IMSI:\t\t\t");
  Serial.println(gsm.moduleIMSI());
  delay(250);
  Serial.print("ICCID:\t\t\t");
  Serial.println(gsm.moduleICCID());
  delay(250);
  Serial.print("Is Connected?:\t\t");
  Serial.println(gsm.isRegistered());
  delay(250);
  Serial.print("Signal Quality:\t\t");
  Serial.println(gsm.signalQuality());
  delay(250);
  Serial.print("Operator:\t\t");
  Serial.println(gsm.operatorName());
  delay(250);
  Serial.print("Operator From Sim:\t");
  Serial.println(gsm.operatorNameFromSim());
  delay(250);
}

void loop() {
  // put your main code here, to run repeatedly:

}
