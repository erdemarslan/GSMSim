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
#include "GSMSimEmail.h"

//////////////////////////////////////
//			EMAIL METHODS			//
//////////////////////////////////////

bool GSMSimEmail::setServer(String server, String port, bool useSSL) {
	int ssl = useSSL == true ? 1 : 0;

	gsm.print(F("AT+EMAILSSL="));
	gsm.print(ssl);
	gsm.print(F("\r"));
	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {

		gsm.print(F("AT+EMAILCID=1\r"));
		_readSerial();

		if (_buffer.indexOf(F("OK")) != -1) {
			gsm.print(F("AT+EMAILTO=30\r"));
			_readSerial();

			if (_buffer.indexOf(F("OK")) != -1) {
				gsm.print(F("AT+SMTPSRV=\""));
				gsm.print(server);
				gsm.print(F("\",\""));
				gsm.print(port);
				gsm.print("\"\r");

				_readSerial();

				if (_buffer.indexOf(F("OK")) != -1) {
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
bool GSMSimEmail::auth(String username, String password) {
	gsm.print(F("AT+SMTPAUTH=1,\""));
	gsm.print(username);
	gsm.print(F("\",\""));
	gsm.print(password);
	gsm.print(F("\"\r"));

	_readSerial();
	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}
bool GSMSimEmail::auth(String username, String password, bool requireAuth) {
	int auth = requireAuth == true ? 1 : 0;
	gsm.print(F("AT+SMTPAUTH="));
	gsm.print(auth);
	gsm.print(F(",\""));
	gsm.print(username);
	gsm.print(F("\",\""));
	gsm.print(password);
	gsm.print(F("\"\r"));

	_readSerial();
	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}
bool GSMSimEmail::gmail(String username, String password) {
	bool conf = setServer("smtp.gmail.com", "465", true);
	if (conf) {
		return auth(username, password);
	}
	else {
		return false;
	}
}
String GSMSimEmail::write(String from, String to, String title, String message) {

	gsm.print(F("AT+SMTPFROM=\""));
	gsm.print(from);
	gsm.print("\"\r");
	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {

		gsm.print("AT+SMTPRCPT=0\r");
		_readSerial();
		delay(50);
		gsm.print(F("AT+SMTPRCPT=0,1,\""));
		gsm.print(to);
		gsm.print("\"\r");
		_readSerial();

		if (_buffer.indexOf(F("OK")) != -1) {

			gsm.print(F("AT+SMTPSUB=\""));
			gsm.print(title);
			gsm.print("\"\r");
			_readSerial();

			if (_buffer.indexOf(F("OK")) != -1) {

				uint16_t uzunluk = message.length();
				gsm.print(F("AT+SMTPBODY="));
				gsm.print(uzunluk);
				gsm.print("\r");

				delay(50);

				gsm.print(message);
				gsm.print(F("\""));
				//_buffer += _readSerial();
				_readSerial();

				if (_buffer.indexOf(F("OK")) != -1) {

					return "OK";
				}
				else {
					return "ERROR:BODY_NOT_SET";
				}

			}
			else {
				return "ERROR:TITLE_NOT_SET";
			}
		}
		else {
			return "ERROR:TO_NOT_SET";
		}
	}
	else {
		return "ERROR:FROM_NOT_SET";
	}
}
String GSMSimEmail::write(String from, String to, String title, String message, String fromName, String toName) {
	gsm.print(F("AT+SMTPFROM=\""));
	gsm.print(from);
	gsm.print("\",\"");
	gsm.print(fromName);
	gsm.print("\"\r");
	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {

		gsm.print("AT+SMTPRCPT=0\r");
		_readSerial();
		delay(50);
		gsm.print(F("AT+SMTPRCPT=0,1,\""));
		gsm.print(to);
		gsm.print("\",\"");
		gsm.print(toName);
		gsm.print("\"\r");
		_readSerial();

		if (_buffer.indexOf(F("OK")) != -1) {

			gsm.print(F("AT+SMTPSUB=\""));
			gsm.print(title);
			gsm.print("\"\r");
			_readSerial();

			if (_buffer.indexOf(F("OK")) != -1) {

				uint16_t uzunluk = message.length();
				gsm.print(F("AT+SMTPBODY="));
				gsm.print(uzunluk);
				gsm.print("\r");

				delay(50);

				gsm.print(message);
				gsm.print(F("\""));
				//_buffer += _readSerial();
				_readSerial();

				if (_buffer.indexOf(F("OK")) != -1) {

					return "OK";
				}
				else {
					return "ERROR:BODY_NOT_SET";
				}

			}
			else {
				return "ERROR:TITLE_NOT_SET";
			}
		}
		else {
			return "ERROR:TO_NOT_SET";
		}
	}
	else {
		return "ERROR:FROM_NOT_SET";
	}
}

String GSMSimEmail::send() {

	gsm.print("AT+SMTPSEND\r");
	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		delay(50);
		_readSerial(30000);
		if (_buffer.indexOf(F("+SMTPSEND:")) != -1) {
			String kod = _buffer.substring(12);
			kod.trim();

			if (kod == "1") {
				return "SUCCESS:EMAIL_SEND";
			}
			else if (kod == "61") {
				return "ERROR:NETWORK_ERROR";
			}
			else if (kod == "62") {
				return "ERROR:DNS_RESOLVE_ERROR";
			}
			else if (kod == "63") {
				return "ERROR:TCP_CONNECTION_ERROR";
			}
			else if (kod == "64") {
				return "ERROR:TIMEOUT_SMTP_RESPONSE";
			}
			else if (kod == "65") {
				return "ERROR:SMTP_RESPONSE_ERROR";
			}
			else if (kod == "66") {
				return "ERROR:NOT_AUTH";
			}
			else if (kod == "67") {
				return "ERROR:AUTH_FAILED";
			}
			else if (kod == "68") {
				return "ERROR:BAD_RECIPIENT";
			}
			else {
				return "ERROR:ERROR_NO_" + kod;
			}
		}
		else {
			delay(50);
			_readSerial(30000);

			if (_buffer.indexOf(F("+SMTPSEND:")) != -1) {
				String kod = _buffer.substring(12);
				kod.trim();

				if (kod == "1") {
					return "SUCCESS:EMAIL_SEND";
				}
				else if (kod == "61") {
					return "ERROR:NETWORK_ERROR";
				}
				else if (kod == "62") {
					return "ERROR:DNS_RESOLVE_ERROR";
				}
				else if (kod == "63") {
					return "ERROR:TCP_CONNECTION_ERROR";
				}
				else if (kod == "64") {
					return "ERROR:TIMEOUT_SMTP_RESPONSE";
				}
				else if (kod == "65") {
					return "ERROR:SMTP_RESPONSE_ERROR";
				}
				else if (kod == "66") {
					return "ERROR:NOT_AUTH";
				}
				else if (kod == "67") {
					return "ERROR:AUTH_FAILED";
				}
				else if (kod == "68") {
					return "ERROR:BAD_RECIPIENT";
				}
				else {
					return "ERROR:ERROR_NO_" + kod;
				}
			}
			else {
				return "ERROR:EMAIL_TIMEOUT_ERROR";
			}
		}
	}
	else {
		return "ERROR:EMAIL_SENDING_ERROR";
	}
}
