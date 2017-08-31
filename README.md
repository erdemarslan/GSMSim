## GSMSim Library for Arduino (for SimCom Modules like SIM800L, SIM900 etc)

This library for Arduino to use SimCom GSM Modules. This library tested on Sim800L (5V Module). Some functions may not work other SimCom Modules like FM Radio functions. This library may work other gsm shields or modules. But some AT Commands only work SimCom modules. So some functions may not work on other gsm shield or module (etc A6 Ai-Thinker)

This library use SoftwareSerial library. So please dont forget this library's limits. Dont use this library on Arduino's hardware serial ports 0 and 1!

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
moduleChangeIMEI(char* new_imei)|true or false|Reserved for future
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
callReadCurrentCall(String serialRaw)|String|Return call status and number like STATUS:xxx|NUMBER:xxx - This only read when you give raw serial data to this function. It not fetch the raw serial data!


#### SMS functions
Name|Return|Notes
:-------|:-------:|:-----------------------------------------------:|






### Support or Contact

If you have any question about this library, please contact only on GitHub.
