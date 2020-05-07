/*
 * GSMSim Library
 * 
 * This library written for SIMCOM modules. Tested on Sim800L. Library may worked on any SIMCOM and another GSM modules and GSM Shields. Some AT commands for only SIMCOM modules.
 *
 * Created 11.05.2017
 * By Erdem ARSLAN
 * Modified 06.05.2020
 *
 * Version: v.2.0.1
 *
 * Erdem ARSLAN
 * Science and Technology Teacher, an Arduino Lover =)
 * erdemsaid@gmail.com
 * https://www.erdemarslan.com/
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

#include "Arduino.h"
#include "GSMSimGPRS.h"
#include "GSMSimTime.h"

//////////////////////////////////////
//			TIME METHODS			//
//////////////////////////////////////

bool GSMSimTime::setServer(int timezone) {
	gsm.print("AT+CNTPCID=1\r");
	_readSerial();

	int zaman = 0;
	if (timezone <= -12) {
		zaman = -47;
	}
	if (timezone > 12) {
		zaman = 48;
	}
	if (timezone > -12 || timezone <= 12) {
		zaman = timezone * 4;
	}

	gsm.print(F("AT+CNTP=\"0.tr.pool.ntp.org\","));
	gsm.print(zaman);
	gsm.print(F("\r"));

	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}
bool GSMSimTime::setServer(int timezone, String server) {
	gsm.print("AT+CNTPCID=1\r");
	_readSerial();

	int zaman = 0;
	if (timezone <= -12) {
		zaman = -47;
	}
	if (timezone > 12) {
		zaman = 48;
	}
	if (timezone > -12 || timezone <= 12) {
		zaman = timezone * 4;
	}

	gsm.print(F("AT+CNTP=\""));
	gsm.print(server);
	gsm.print(F("\","));
	gsm.print(zaman);
	gsm.print(F("\r"));

	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}
String GSMSimTime::syncFromServer() {
	gsm.print("AT+CNTPCID=1\r");
	_readSerial();

	gsm.print(F("AT+CNTP\r"));
	_readSerial();
	//delay(50);
	_readSerial(30000);

	//return _buffer;

	if (_buffer.indexOf(F("+CNTP:")) != -1) {
		String kod = _buffer.substring(8);
		kod.trim();

		if (kod == "1") {
			return "TIME_SYNCHRONIZED_SUCCESS";
		}
		else if (kod == "61") {
			return "NETWORK_ERROR";
		}
		else if (kod == "62") {
			return "DNS_ERROR";
		}
		else if (kod == "63") {
			return "CONNECTION_ERROR";
		}
		else if (kod == "64") {
			return "SERVICE_RESPONSE_ERROR";
		}
		else if (kod == "65") {
			return "SERVICE_RESPONSE_TIMEOUT";
		}
		else {
			return "UNKNOWN_ERROR_" + kod;
		}
	}
	else {
		return "AT_COMMAND_ERROR";
		//return _buffer;
	}
}
String GSMSimTime::getRaw() {
	gsm.print("AT+CCLK?\r");
	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		String zaman = _buffer.substring(_buffer.indexOf(F("\"")) + 1, _buffer.lastIndexOf(F("\"")));
		return zaman;
	}
	else {
		return "ERROR:NOT_GET_DATETIME";
	}
}
void GSMSimTime::get(int *day, int *month, int *year, int *hour, int *minute, int *second) {
	gsm.print("AT+CCLK?\r");
	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		_buffer = _buffer.substring(_buffer.indexOf(F("\"")) + 1, _buffer.lastIndexOf(F("\"")) - 1);
		*year = (_buffer.substring(0, 2).toInt()) + 2000;
		*month = _buffer.substring(3, 5).toInt();
		*day = _buffer.substring(6, 8).toInt();
		*hour = _buffer.substring(9, 11).toInt();
		*minute = _buffer.substring(12, 14).toInt();
		*second = _buffer.substring(15, 17).toInt();
	}
}