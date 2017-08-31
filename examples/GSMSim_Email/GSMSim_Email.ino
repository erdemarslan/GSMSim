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

  Serial.println("GSMSim Library - Send Email Example");
  Serial.println("");
  delay(1000);

  gsm.start(); // baud default 9600
  //gsm.start(BAUD);

  Serial.print("Connect to Internet: ");
  // For sending email, we must connect to internet!
  Serial.println(gsm.gprsConnectBearer());

  // Wait for ip config on internet.
  delay(5000);
  
  // Set Gmail Account
  // If you take an error when sending email with google, please check display unlock captcha setting on google
  // please use this url for this.
  // https://www.google.com/accounts/DisplayUnlockCaptcha
  // for gmail, dont use @gmail.com on your username.
  // at the last, if you take an authendication error, please check 2 way verification settings on your account.
  Serial.print("Set gmail smtp configuration: ");
  Serial.println(gsm.emailSMTPGmail("your_gmail_username_Without_@gmail", "your_gmail_password"));

  // Write Email
  Serial.print("Write email to someone: ");
  // In body, if you want to use break, use \b, for tab use \t on your message body string. Only use Ascii characters on title and body, otherwise your mail not send.
  Serial.println(gsm.emailSMTPWrite("your_email_address@gmail.com", "who_you_want_to_send_email", "Hello my friend", "This is my email body. Thanks for watching!\b :)"));

  // A little wait
  delay(1000);
  // Send Email
  Serial.print("Send email... : ");
  Serial.println(gsm.emailSMTPSend());
  
  // Close internet connection
  Serial.print("Close Internet: ");
  Serial.println(gsm.gprsCloseConn());
}

void loop() {
  // put your main code here, to run repeatedly:
}
