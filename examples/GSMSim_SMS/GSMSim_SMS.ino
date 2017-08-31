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

  Serial.println("GSMSim Library - SMS Example");
  Serial.println("");
  delay(1000);

  gsm.start(); // baud default 9600
  //gsm.start(BAUD);

  Serial.println("Changing to text mode.");
  gsm.smsTextMode(true); // TEXT or PDU mode. TEXT is readable :)

  char* number = "+905123456789";
  char* message = "Hi my friend. How are you?"; // message lenght must be <= 160. Only english characters.

  Serial.println("Sending Message --->");
  Serial.println(gsm.smsSend(number, message)); // if success it returns true (1) else false (0)
  delay(2000);

  Serial.println("Listing unread message(s).");
  Serial.println(gsm.smsListUnread()); // if not unread messages have it returns "NO_SMS"

  Serial.println("Read SMS on index no = 1");
  Serial.println(gsm.smsRead(1)); // if no message in that index, it returns IXDEX_NO_ERROR
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
