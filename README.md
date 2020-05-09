# GSMSim Library for Arduino (for SimCom Modules like SIM800L, SIM900 etc)

This library for Arduino to use SimCom GSM Modules. This library tested on Sim800L (5V Module). Some methods may not work all SimCom Modules like FM Radio methods. This library may work other gsm shields or modules. But some AT Commands only work SimCom modules. So some functions may not work on other gsm shield or module (etc A6 Ai-Thinker)

This library use any Serial library like HardwareSerial, SoftwareSerial etc. If you use SoftwareSerial, please dont forget this library's limits.

### ChangeLog
#### v.2.0.2
* Little fix for incoming SMS from iPhone. Remove ???? chars at the begining of SMS.

#### v.2.0.1
* GSMSim now can be use with any serial interface. Hardware or Software
* added some new methods.
* Changed many method names. There is no backward compatibility.
* Fixed and added issue#26, issue#24, issue#19, issue#17,issue#14, issue#13, issue#12, issue#1
#### v.1.0.9
* First public release.
#### v.1.0.0
* First release for personal use.

## Connection & Pinouts
Arduino |   Sim800L   |    Notes  
-------------|-------------|------------
+5v| (3.8v)~(4.4v)!| Power supply input
RX_PIN | TX |  
TX_PIN | RX |
RESET_PIN | RST| Reset Pin
GND | GND |

DEFAULT RESET PIN => 2
DEFAULT LED PIN => 13
DEFAULT LED FLAG => true

## How I use this library?

Please visit the example pages.

## Recommendation
* If it possible, use HardwareSerial. SoftwareSerial a bit laggy.
* Use highiest baudrate as you can. (I test on HardwareSerial 115200 baudrate, SoftwareSerial 57600 baudrate)
* Please chooice good power supply. I recommend 5V power supply and 1A or over. (Sim800L EVB Board)
* If the signal is weak or the power supply is insufficient, the module can reset itself on load.


## Which methods can i use?

### GSMSim Methods
*All this methods can use with any GSMSim* classes. All GSMSim* classes inherit from GSMSim class.*

Method Name |Return   |Notes
:-------|:-------:|:-----------------------------------------------:|
init()|void|Method init reset pin, led pin and led flag.
reset()|void|Reset module with reset pin.
sendATCommand(char* command)|String|You can send AT commands manualy. Dont use \r char in command.
setPhoneFunc(int level)|bool|Set phone function. level can be 0,1 or 4. 0-minimum, 1-full, 4-disable
signalQuality()|integer|0-31 -> 0-poor, 31-full, 99-unknown
isRegistered()|bool|Is module connect to gsm operator?
isSimInserted()|bool|
pinStatus()|int|0-ready, 1-sim pin, 2-sim puk, 3-ph sim pin, 4-ph sim puk, 5-sim pin2, 6-sim puk2, 7-unknown
enterPinCode(char* pinCode)|bool|Enter pin code if module locked with pin code.
enablePinCode(char* pinCode)|bool| Enable pin lock.
disablePinCode(char* pinCode)|bool|Disable pin lock.
operatorName()|String|If modune not connected it returns NOT_CONNECTED.
operatorNameFromSim()|String|Only works on SimCom modules. If modune not connected it returns NOT_CONNECTED.
phoneStatus()|integer|Phone activity status 0-ready, 2-unknown, 3-ringing, 4-call in progress, 99-unkonown
echoOff()|bool|Echo off AT command on execute result. GSMSim class automaticly set echoOff on init.
echoOn()|bool|Echo AT command on execute result.
moduleManufacturer()|String|
moduleModel()|String|
moduleRevision()|String|
moduleIMEI()|String|
moduleIMEIChange(char* imeino)|bool|Change IMEI. Changing the IMEI is prohibited by the laws of some countries. In this regard, comply with the laws of your country. You've been warned.
moduleIMSI()|String|
moduleICCID()|String|
ringerVolume()|integer|
setRingerVolume(uint8_t level)|bool|true or false|level must between 0-100
speakerVolume()|integer|
setSpeakerVolume(uint8_t level)|bool|level must between 0-100
moduleDebug()|String|Print verbose.
saveSettingsToModule()|bool|Save some setting to module EEPROM.

### GSMSimCall Methods
*This class inherit from GSMSim class. You can use GSMSim class methods with this class.

Method Name |Return   |Notes
:-------|:-------:|:-----------------------------------------------:|
initCall()|bool|Init call function. If you use other methods correctly, add your code this method.
call(char* phone_number)|bool|
answer()|bool|
hangoff()|bool|
status()|integer|Return codes as same as phoneStatus function
setCLIP(bool active)|bool|
setCLIR(bool active)|bool|
setCOLP(bool active)|bool|
isCOLPActive()|bool|
showCurrentCall(bool active)|bool|
readCurrentCall(String serialRaw)|String|Return call status and number like STATUS:xxx\|NUMBER:xxx - This only read when you give raw serial data to this function. It not fetch the raw serial data!
setCallReject(bool rejectAll)|bool|Enable or disable reject call automaticly.


### GSMSimDTMF Methods
*This class inherit from GSMSimCall class. You can use GSMSim and GSMSimCall classes methods with this class.

Method Name |Return   |Notes
:-------|:-------:|:-----------------------------------------------:|
setDTMF(bool active, unsigned int interval, bool reportTime, bool soundDetect)|bool|Activate or deactivate DTMF tones.
readDTMF(String serialRaw)|String|Get pressed key info from DTMF on serial raw data.


### GSMSimUSSD Methods
*This class inherit from GSMSimCall class. You can use GSMSim and GSMSimCall classes methods with this class.

Method Name |Return   |Notes
:-------|:-------:|:-----------------------------------------------:|
sendUSSD(char* code)|bool|Send USSD command.
readUSSD(String serialRaw)|String|Read USSD response from serial raw value.


### GSMSimSMS Methods
*This class inherit from GSMSim class. You can use GSMSim class methods with this class.

Method Name |Return   |Notes
:-------|:-------:|:-----------------------------------------------:|
initSMS()|bool|Init sms function. If you use other methods correctly, add your code this method.
setTextMode(bool textModeON)|bool|Text Mode or PDU Mode. This class works with TEXT Mode.
setPreferredSMSStorage(char* mem1, char* mem2, char* mem3)|bool|"ME" (Module), "SM" (Sim), "ME_P" (Module Preferred), "SM_P" (Sim Preferred), "MT" (ME or SM, Sim Preferred). Class use "ME" for all memories.
setNewMessageIndication()|bool|
setCharset(char* charset)|bool|"IRA","GSM","UCS2","HEX","PCCP","PCDN","8859-1". Class use IRA charset.
send(char* number, char* message)|bool|
list(bool onlyUnread)|String|If no message found it returns NO_SMS else returns SMSIndexNo:x,y,z. If onlyUnread equal to false, method returns ALL messages.
read(unsigned int index)|String|Read message from given index.
read(unsigned int index, bool markRead)|String|Read message from given index. If markRead equal to false, message not set to READ.
getSenderNo(unsigned int index)|String|Give the message sender number.
readFromSerial(String serialRaw)|String|Read message from serial raw value.
indexFromSerial(String serialRaw)|integer|Get message index number from serial raw value.
readMessageCenter()|String|
changeMessageCenter(char* messageCenter)|bool|
deleteOne(unsigned int index)|bool|Delete sms in given index.
deleteAllRead()|bool|Delete all read messages.
deleteAll()|bool|Delete all messages.


### GSMSimFMRadio Methods
*This class inherit from GSMSim class. You can use GSMSim class methods with this class.

Method Name |Return   |Notes
:-------|:-------:|:-----------------------------------------------:|
fmOpen()|true or false|Open fm radio.
fmOpen(bool mainChannel)|true or false|Open fm radio. true is main channel, false is aux channel.
fmOpen(bool mainChannel, uint16_t freq)|true or false|Frequency must be between 875 and 1080. 875 is 87.5 MHz.
fmIsOpened()|true or false|Return radio status.
fmClose()|true or false|
fmGetFreq()|integer|
fmSetFreq(uint16_t freq)|true or false|875 to 1080
fmGetVolume()|integer|
fmSetVolume(uint16_t volume)|true or false|


### GSMSimGPRS Methods
*This class inherit from GSMSim class. You can use GSMSim class methods with this class.
When you call this class, it automaticly load default APN, USER and PWD values. If you want to change this values, use gprsInit() method for this.
DEFAULT APN => "internet"
DEFAULT USER => ""
DEFAULT PWD => ""

Method Name |Return   |Notes
:-------|:-------:|:-----------------------------------------------:|
gprsInit(String apn)|void|Set only APN value
gprsInit(String apn, String user, String password)|void|Set APN, USER and PWD values.
connect()|bool|Connect to GPRS
isConnected()|bool|
getIP()|String|Get IP v4 address.
closeConn()|bool|

### GSMSimHTTP Methods
*This class inherit from GSMSimGPRS class. You can use GSMSim and GSMSimGPRS classes methods with this class.

Method Name |Return   |Notes
:-------|:-------:|:-----------------------------------------------:|
get(String url)|String|Use GET method. It not returns web page response. Only HTTP method, status code and response lenght.
get(String url, bool read)|String|If read equal to true it returns HTTP method, status code, response lenght and web page response.
getWithSSL(String url)|String|Same as get(String url) but it connect to SSL sites.
getWithSSL(String url, bool read)|String|Same as get(String url, bool read ) but it connect to SSL sites.
post(String url, String data, String contentType)|String|Use POST method. It not returns web page response. Only HTTP method, status code and response lenght.
post(String url, String data, String contentType, bool read)|String|If read equal to true it returns HTTP method, status code, response lenght and web page response.
postWithSSL(String url, String data, String contentType)|String|Same as post(String url, String data, String contentType) but it connect to SSL sites.
postWithSSL(String url, String data, String contentType, bool read)|String|Same as post(String url, String data, String contentType) but it connect to SSL sites.
ping(String address)|String|Its to laggy. It returns raw AT response. Its experimental.


### GSMSimEmail Methods
*This class inherit from GSMSimGPRS class. You can use GSMSim and GSMSimGPRS classes methods with this class.
Note: All commands for SMTP. For Gmail please look at example files.

Method Name |Return   |Notes
:-------|:-------:|:-----------------------------------------------:|
setServer(String server, String port, bool useSSL)|bool|Set SMTP Server configuration.
auth(String username, String password)|bool|
auth(String username, String password, bool requireAuth)|bool|
gmail(String username, String password)|bool|If you want to send email over GMAIL, you can use this method for set server requirements.
write(String from, String to, String title, String message)|String|Return OK if success.
write(String from, String to, String title, String message, String fromName, String toName)|String|Return OK if success.
send()|String|Return SUCCESS:EMAIL_SEND if send status ok. Else return ERROR:Error_Type


### GSMSimTime Methods
*This class inherit from GSMSimGPRS class. You can use GSMSim and GSMSimGPRS classes methods with this class.


Method Name |Return   |Notes
:-------|:-------:|:-----------------------------------------------:|
setServer(int timezone)|bool|
setServer(int timezone, String server)|bool|
syncFromServer()|String|
getRaw()|String|
get(int *day, int *month, int *year, int *hour, int *minute, int *second)|void|



### Credits

Cristian Steib - Sim800l Arduino library - https://github.com/cristiansteib/Sim800l

Vittorio Esposito - Sim800L-Arduino-Library-revised - https://github.com/VittorioEsposito/Sim800L-Arduino-Library-revised

Thanks.


### Support or Contact

If you have any question about this library, please contact only on GitHub.

