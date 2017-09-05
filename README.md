## GSMSim Library for Arduino (for SimCom Modules like SIM800L, SIM900 etc)

This library for Arduino to use SimCom GSM Modules. This library tested on Sim800L (5V Module). Some functions may not work other SimCom Modules like FM Radio functions. This library may work other gsm shields or modules. But some AT Commands only work SimCom modules. So some functions may not work on other gsm shield or module (etc A6 Ai-Thinker)

This library use SoftwareSerial library. So please dont forget this library's limits. Dont use this library on Arduino's hardware serial ports 0 and 1!

### Connection & Pinouts
Arduino Uno  |   Sim800L   |    Notes  
-------------|-------------|------------
+5v| (3.8v)~(4.4v)!| Power supply input
7 RX_PIN | TX |  
8 TX_PIN | RX |
2   RESET_PIN | RST| Reset Pin
GND | GND |

### How I use this library?

Firstly inport this library on your Arduino IDE. Then you can use this:

```markdown
#import <GSMSim.h>

#define RX 7
#define TX 8
#define RESET 2
#define BAUD 9600

/*
 * Defaults => RX: 7 | TX: 8 | Reset: 2 | LED Pin: 13 | LED Flag: true | Baud Rate: 9600
 * GSMSim gsm;
 * GSMSim gsm(RX, TX);
 * GSMSim gsm(RX, TX, RESET);
 * GSMSim gsm(RX, TX, RESET, LED_PIN, LED_FLAG);
 */

GSMSim gsm(RX, TX, RESET);

void setup() {
  gsm.start();
  //gsm.start(9600);
}

```

For more details see Examples.

### Which functions i can use?

#### Module Functions

Name|Return|Notes
:-------|:-------:|:-----------------------------------------------:|
start()|none|Begin the library with 9600 baud rate
start(uint8_t baud_rate)|none|Begin the library with given baud rate
reset()|none|Reset the module
setPhoneFunc(uint8_t level)|true or false|level can be take 0,1 or 4 0-minimum, 1-full, 4-disable
signalQuality()|integer|0-31 -> 0-poor, 31-full, 99-unknown
isRegistered()|true or false|Is module connected to gsm operator?
isSimInserted()|true or false|
pinStatus()|integer|0-ready, 1-sim pin, 2-sim puk, 3-ph sim pin, 4-ph sim puk, 5-sim pin2, 6-sim puk2, 7-unknown
operatorName()|String|returns NOT_CONNECTED if not connected!
operatorNameFromSim()|String|Only works on simcom modules. returns NOT_CONNECTED if not connected!
phoneStatus()|integer|Phone activity status 0-ready, 2-unknown, 3-ringing, 4-call in progress, 99-unkonown
echoOff()|true or false|Echo mode off (for module)
echoOn()|true or false|Echo mode on (for module)
moduleManufacturer()|String|
moduleModel()|String|
moduleRevision()|String|
moduleIMEI()|String|
moduleIMSI()|String|
moduleICCID()|String|
ringerVolume()|integer|
setRingerVolume(uint8_t level)|true or false|level must between 0-100
speakerVolume()|integer|
setSpeakerVolume(uint8_t level)|true or false|level must between 0-100
moduleDebug()|String|print verbose


#### Call functions
Name|Return|Notes
:-------|:-------:|:-----------------------------------------------:|
call(char* phone_number)|true or false|If colp active, it always return true.
callAnswer()|true or false|
callHangoff()|true of false|
callStatus()|integer|Return codes as same as phoneStatus function
callSetCOLP(bool active)|true or false|If active is true, when call status change, module send call details to SoftwareSerial
callIsCOLPActive|true or false|True COLP is active else passive
callActivateListCurrent(bool active)|true or false|say the caller :)
callReadCurrentCall(String serialRaw)|String|Return call status and number like STATUS:xxx\|NUMBER:xxx - This only read when you give raw serial data to this function. It not fetch the raw serial data!


#### SMS functions
Name|Return|Notes
:-------|:-------:|:-----------------------------------------------:|
smsTextMode(bool textModeON)|true or false|TEXT mode or PDU mode.
smsSend(char* number, char* message)|true or false|Message must be in 160 characters and in text mode only use ascii characters.
smsListUnread()|String|If no message found it returns NO_SMS else returns SMSIndexNo:x,y,z. If you have a lot of un read messages, return only SMSIndexNo:
smsRead(uint8_t index)|String|If not message on given index, it return INDEX_NO_ERROR else a string about message.
smsRead(uint8_t index, bool markRead)|String|This function can mark message read or unread when opened.
smsReadFromSerial(String serialRaw)|String|Read sms from serial raw data.
smsIndexFromSerial(String serialRaw)|integer|Get sms index number from serial raw.
smsReadMessageCenter()|String|Get SMS Message Center number.
smsChangeMessageCenter(char* messageCenter)|true or false|Change SMS Message Center number.
smsDeleteOne(uint8_t index)|true or false|Delete sms in given index.
smsDeleteAllRead()|true or false|Delete all read messages.
smsDeleteAll()|true or false|Delete all messages.

#### DTMF functions
Name|Return|Notes
:-------|:-------:|:-----------------------------------------------:|
dtmfSet(bool active, uint8_t interval, bool reportTime, bool soundDetect)|true or false|Activate or deactivate DTMF tones.
dtmfRead(String serialRaw)|String|Get pressed key info from DTMF on serial raw data.


#### USSD functions
Name|Return|Notes
:-------|:-------:|:-----------------------------------------------:|
ussdSend(char* code)|true or false|Send USSD command.
ussdRead(String serialRaw)|String|

#### FM Radio functions
Name|Return|Notes
:-------|:-------:|:-----------------------------------------------:|
fmOpen()|true or false|Open fm radio.
fmOpen(bool mainChannel)|true or false|Open fm radio. true is main channel, false is aux channel.
fmOpen(bool mainChannel, uint8_t freq)|true or false|Frequency must be between 875 and 1080. 875 is 87.5 MHz.
fmIsOpened()|true or false|Return radio status.
fmClose()|true or false|
fmGetFreq()|integer|
fmSetFreq(uint8_t freq)|true or false|875 to 1080
fmGetVolume()|integer|
fmSetVolume(uint8_t volume)|true or false|

#### GPRS functions
Name|Return|Notes
:-------|:-------:|:-----------------------------------------------:|
gprsConnectBearer()|true or false|APN=internet, USER="", PASS=""
gprsConnectBearer(String apn)|true or false|
gprsConnectBearer(String apn, String user, String password)|true or false|
gprsIsConnected()|true or false|Return gprs connection status.
gprsGetIP()|String|Return IP Address
gprsCloseConn()|true or false|
gprsHTTPGet(String url)|String|Only get about response info, http code etc.
gprsHTTPGet(String url, bool read)|String|Get with response.


#### Time functions
Name|Return|Notes
:-------|:-------:|:-----------------------------------------------:|
timeSetServer(int timezone)|true or false|Require active internet connection
timeSetServer(int timezone, String server)|true or false|
timeSyncFromServer()|true or false|Sync time from server. Require an active internet connection.
timeGetRaw()|String|Get raw time.
timeGet(int *day, int *month, int *year, int *hour, int *minute, int *second)|void|


#### Time functions
Name|Return|Notes
:-------|:-------:|:-----------------------------------------------:|
emailSMTPConf(String server, String port, bool useSSL)|true or false|Set SMTP Server configuration.
emailSMTPAuth(String username, String password)|true or false|
emailSMTPAuth(String username, String password, bool requireAuth)|true or false|
emailSMTPGmail(String username, String password)|true or false|If you want to send email over GMAIL, you can use this function for set server requirements.
emailSMTPWrite(String from, String to, String title, String message)|String|Return OK if success.
emailSMTPWrite(String from, String to, String title, String message, String fromName, String toName)|String|Return OK if success.
emailSMTPSend()|String|Return SUCCESS:EMAIL_SEND if send status ok. Else return ERROR:Error_Type


### Credits

Cristian Steib - Sim800l Arduino library - https://github.com/cristiansteib/Sim800l

Vittorio Esposito - Sim800L-Arduino-Library-revised - https://github.com/VittorioEsposito/Sim800L-Arduino-Library-revised

Thanks.


### Support or Contact

If you have any question about this library, please contact only on GitHub.
