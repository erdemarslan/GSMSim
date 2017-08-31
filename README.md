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

Name|Return|Notes
:-------|:-------:|:-----------------------------------------------:|
start()|none|Begin the library with 9600 baud rate
start(uint8_t baud_rate)|none|Begin the library with given baud rate
reset()|none|Reset the module



### Support or Contact

If you have any question about this library, please contact only on GitHub.
