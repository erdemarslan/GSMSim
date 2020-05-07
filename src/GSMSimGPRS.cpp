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

void GSMSimGPRS::gprsInit(String apn) {
	APN = apn;
}
void GSMSimGPRS::gprsInit(String apn, String user, String password) {
	APN = apn;
	USER = user;
	PWD = password;
}
// Connect to GPRS Bearer
bool GSMSimGPRS::connect() {
	gsm.print(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r"));
	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		gsm.print(F("AT+SAPBR=3,1,\"APN\",\""));
		gsm.print(APN);
		gsm.print(F("\"\r"));
		_readSerial();
		if (_buffer.indexOf(F("OK")) != -1) {
			gsm.print(F("AT+SAPBR=3,1,\"USER\",\""));
			gsm.print(USER);
			gsm.print(F("\"\r"));
			_readSerial();
			if (_buffer.indexOf(F("OK")) != -1) {
				gsm.print(F("AT+SAPBR=3,1,\"PWD\",\""));
				gsm.print(PWD);
				gsm.print(F("\"\r"));
				_readSerial();
				if (_buffer.indexOf(F("OK")) != -1) {
					gsm.print("AT+SAPBR=1,1\r");
					_readSerial();
					if (_buffer.indexOf(F("OK")) != -1) {
						gsm.print("AT+SAPBR=2,1\r");
						_readSerial();
						if (_buffer.indexOf(F("\"0.0.0.0\"")) == -1 || _buffer.indexOf(F("ERR")) == -1) {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
// Check is GPRS connected?
bool GSMSimGPRS::isConnected() {
	gsm.print(F("AT+SAPBR=2,1\r"));
	_readSerial();
	if (_buffer.indexOf(F("ERROR")) != -1 || _buffer.indexOf(F("\"0.0.0.0\"")) != -1) {
		return false;
	}
	else {
		return true;
	}
}
// GET IP Address
String GSMSimGPRS::getIP() {
	gsm.print(F("AT+SAPBR=2,1\r\n"));
	_readSerial();
	if (_buffer.indexOf(F("ERROR")) != -1 || _buffer.indexOf(F("\"0.0.0.0\"")) != -1) {
		return "ERROR:NO_IP";
	}
	else {
		if (_buffer.indexOf("+SAPBR:") != -1) {
			String veri = _buffer.substring(_buffer.indexOf(F(",\""))+2, _buffer.lastIndexOf(F("\"")));
			veri.trim();
			return veri;
		}
		else {
			"ERROR:NO_IP_FETCH";
		}
	}
}
// Close GPRS Connection
bool GSMSimGPRS::closeConn() {
	gsm.print(F("AT+SAPBR=0,1\r"));
	_readSerial();
	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}