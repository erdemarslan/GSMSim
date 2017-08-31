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

  Serial.println("GSMSim Library - GPRS Connection and HTTP Get Example");
  Serial.println("");
  delay(1000);

  gsm.start(); // baud default 9600
  //gsm.start(BAUD);

  /*
   * gsm.gprsConnectBearer() APN: "internet", User: "", Password: ""
   * gsm.gprsConnectBearer(String apn) User: "", Password: ""
   * gsm.gprsConnectBearer(String apn, String user, String password)
   * 
   * Returns boolean true/false. Is connect to baerer return true, else false
   * 
   */
  Serial.println(gsm.gprsConnectBearer());

  //delay(1000);
  
  Serial.println(gsm.gprsGetIP()); // String ip address.

  /*
   * gsm.gprsHTTPGet("www.google.com") // only returns method and response code like METHOD:GET|HTTPCODE:xxx
   * gsm.gprsHTTPGet("www.google.com", true) // returns method, response code, data lenght and DATA like METHOD:GET|HTTPCODE:xxx|LENGHT:x|DATA:......
   * 
   * if an error occured it returns error messages like this after ERROR: tag:
   * HTTP_READ_ERROR
   * HTTP_ACTION_READ_ERROR
   * HTTP_ACTION_ERROR
   * HTTP_PARAMETER_ERROR
   * HTTP_INIT_ERROR
   * GPRS_NOT_CONNECTED
   * 
   * This function automaticly terminate HTTP request at the end but it doesn't close the gprs connection!
   * 
   */

  Serial.println(gsm.gprsHTTPGet("iot.robothane.net/test.php", true));

  
  // After this command, if you make another HTTP request you must re connect to bearer like gprsConnectBearer() function.
  // If you make multiple http request in one loop, dont close connection, make http requests then close the connection!
  // You can check the connection is alive with gprsIsConnected() function.
  Serial.println(gsm.gprsCloseConn()); 
} 

  

void loop() {
  // put your main code here, to run repeatedly:

}
