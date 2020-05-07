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
#include "GSMSim.h"
#include "stdint.h"


void GSMSim::init() {
	pinMode(RESET_PIN, OUTPUT);
	digitalWrite(RESET_PIN, HIGH);

	if (LED_FLAG) {
		pinMode(LED_PIN, OUTPUT);
	}

	_buffer.reserve(BUFFER_RESERVE_MEMORY);

	echoOff();
}


void GSMSim::reset() {
	if (LED_FLAG) {
		digitalWrite(LED_PIN, HIGH);
	}

	digitalWrite(RESET_PIN, LOW);
	delay(1000);
	digitalWrite(RESET_PIN, HIGH);
	delay(1000);

	// Modul kendine geldi mi onu bekle
	gsm.print(F("AT\r"));
	_readSerial();
	while (_buffer.indexOf(F("OK")) == -1) {
		gsm.print(F("AT\r"));
		_readSerial();
	}

	if (LED_FLAG) {
		digitalWrite(LED_PIN, LOW);
	}
}

// send AT Command to module
String GSMSim::sendATCommand(char* command) {
	gsm.print(command);
	gsm.print("\r");
	_readSerial(10000);
	return _buffer;
}

// SET PHONE FUNC +
bool GSMSim::setPhoneFunc(int level = 1) {

	if(level == 0 || level == 1 || level == 4) {
		gsm.print(F("AT+CFUN="));
		gsm.print(level);
		gsm.print(F("\r"));

		_readSerial();

		if((_buffer.indexOf(F("OK"))) != -1)  {
			return true;
		}
		else {
			return false;
		}
	} else {
		return false;
	}
}

// SIGNAL QUALTY - 0-31 | 0-> poor | 31 - Full | 99 -> Unknown +
unsigned int GSMSim::signalQuality() {
	gsm.print(F("AT+CSQ\r"));
	_readSerial(5000);

	if((_buffer.indexOf(F("+CSQ:"))) != -1) {
		return _buffer.substring(_buffer.indexOf(F("+CSQ: "))+6, _buffer.indexOf(F(","))).toInt();
	} else {
		return 99;
	}
}

// IS Module connected to the operator? +
bool GSMSim::isRegistered() {
	gsm.print(F("AT+CREG?\r"));
	_readSerial();

	if( (_buffer.indexOf(F("+CREG: 0,1"))) != -1 || (_buffer.indexOf(F("+CREG: 0,5"))) != -1 || (_buffer.indexOf(F("+CREG: 1,1"))) != -1 || (_buffer.indexOf(F("+CREG: 1,5"))) != -1) {
		return true;
	} else {
		return false;
	}
}

// IS SIM Inserted? +
bool GSMSim::isSimInserted() {
	gsm.print(F("AT+CSMINS?\r"));
	_readSerial();
	if(_buffer.indexOf(",") != -1) {
		// bölelim
		String veri = _buffer.substring(_buffer.indexOf(F(","))+1, _buffer.indexOf(F("OK")));
		veri.trim();
		//return veri;
		if(veri == "1") {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

// Pin statüsü - AT+CPIN? +
unsigned int GSMSim::pinStatus() {
	gsm.print(F("AT+CPIN?\r"));
	_readSerial();

	if(_buffer.indexOf(F("READY")) != -1)
	{
		return 0;
	}
	else if(_buffer.indexOf(F("SIM PIN")) != -1)
	{
		return 1;
	}
	else if(_buffer.indexOf(F("SIM PUK")) != -1)
	{
		return 2;
	}
	else if(_buffer.indexOf(F("PH_SIM PIN")) != -1)
	{
		return 3;
	}
	else if(_buffer.indexOf(F("PH_SIM PUK")) != -1)
	{
		return 4;
	}
	else if(_buffer.indexOf(F("SIM PIN2")) != -1)
	{
		return 5;
	}
	else if(_buffer.indexOf(F("SIM PUK2")) != -1)
	{
		return 6;
	}
	else {
		return 7;
	}
}

// Unlock the pin code +
bool GSMSim::enterPinCode(char* pinCode) {
	gsm.print(F("AT+CPIN=\""));
	gsm.print(pinCode);
	gsm.print(F("\"\r"));
	_readSerial(6000);

	if(_buffer.indexOf(F("ERROR")) != -1) {
		return false;
	} else {
		return true;
	}
}

// enable pin code... +
bool GSMSim::enablePinCode(char* pinCode) {
	gsm.print(F("AT+CLCK=\"SC\",1,\""));
	gsm.print(pinCode);
	gsm.print(F("\"\r"));
	_readSerial(6000);
	if(_buffer.indexOf(F("ERROR")) != -1) {
		return false;
	} else {
		reset();
		return true;
	}
}

// disable pin code +
bool GSMSim::disablePinCode(char* pinCode) {
	gsm.print(F("AT+CLCK=\"SC\",0,\""));
	gsm.print(pinCode);
	gsm.print(F("\"\r"));
	_readSerial(6000);
	if(_buffer.indexOf(F("ERROR")) != -1) {
		return false;
	} else {
		reset();
		return true;
	}
}

// OPERATOR NAME +
String GSMSim::operatorName() {
	gsm.print(F("AT+COPS?\r"));
	_readSerial();

	if(_buffer.indexOf(F(",")) == -1) {
		return "NOT CONNECTED";
	}
	else {
		 return _buffer.substring(_buffer.indexOf(F(",\""))+2, _buffer.lastIndexOf(F("\"")));
	}
}

// OPERATOR NAME FROM SIM +
String GSMSim::operatorNameFromSim() {
	gsm.flush();
	gsm.print(F("AT+CSPN?\r"));
	_readSerial();
	//delay(250);
	//_readSerial();
	/*
	return _buffer;
	*/
	if(_buffer.indexOf(F("OK")) != -1) {
		return _buffer.substring(_buffer.indexOf(F(" \"")) + 2, _buffer.lastIndexOf(F("\"")));
	}
	else {
		return "NOT CONNECTED";
	}
}

// PHONE STATUS +
unsigned int GSMSim::phoneStatus() {
	gsm.print(F("AT+CPAS\r"));
	_readSerial();

	if((_buffer.indexOf("+CPAS: ")) != -1)
	{
		return _buffer.substring(_buffer.indexOf(F("+CPAS: "))+7,_buffer.indexOf(F("+CPAS: "))+9).toInt();
	}
	else {
		return 99; // not read from module
	}
}

// ECHO OFF +
bool GSMSim::echoOff() {
	gsm.print(F("ATE0\r"));
	_readSerial();
	if ( (_buffer.indexOf(F("OK")) )!=-1 ) {
   		return true;
   }
   else {
   	return false;
   }
}

// ECHO ON +
bool GSMSim::echoOn() {
	gsm.print(F("ATE1\r"));
	_readSerial();
	if ( (_buffer.indexOf(F("OK")) )!=-1 ) {
   		return true;
   }
   else {
   	return false;
   }
}

// Modül Üreticisi +
String GSMSim::moduleManufacturer() {
	gsm.print(F("AT+CGMI\r"));
	_readSerial();
	String veri = _buffer.substring(8, _buffer.indexOf(F("OK")));
	veri.trim();
	veri.replace("_", " ");
	return veri;
}

// Modül Modeli +
String GSMSim::moduleModel() {
	gsm.print(F("AT+CGMM\r"));
	_readSerial();

	String veri = _buffer.substring(8, _buffer.indexOf(F("OK")));
	veri.trim();
	veri.replace("_", " ");
	return veri;
}

// Modül Revizyonu + 
String GSMSim::moduleRevision() {
	gsm.print(F("AT+CGMR\r"));
	_readSerial();

	String veri = _buffer.substring(_buffer.indexOf(F(":"))+1 , _buffer.indexOf(F("OK")));
	veri.trim();
	return veri;
}

// Modülün IMEI numarası +
String GSMSim::moduleIMEI() {
	gsm.print(F("AT+CGSN\r"));
	_readSerial();

	String veri = _buffer.substring(8, _buffer.indexOf(F("OK")));
	veri.trim();
	return veri;
}

// Modülün IMEI Numarasını değiştirir. +
bool GSMSim::moduleIMEIChange(char* imeino) {
	gsm.print(F("AT+SIMEI="));
	gsm.print(imeino);
	gsm.print("\r");

	_readSerial();

	if ( (_buffer.indexOf(F("OK")) )!=-1 ) {
   		return true;
   	}
	else
	{
		return false;
	}
}

// Modülün SIM Numarası +
String GSMSim::moduleIMSI() {
	gsm.print(F("AT+CIMI\r"));
	_readSerial();

	String veri = _buffer.substring(8, _buffer.indexOf(F("OK")));
	veri.trim();
	return veri;
}

// Sim Kart Seri Numarası +
String GSMSim::moduleICCID() {
	gsm.print(F("AT+CCID\r"));
	_readSerial();

	String veri = _buffer.substring(8, _buffer.indexOf(F("OK")));
	veri.trim();

	return veri;
}

// Çalma Sesi +
unsigned int GSMSim::ringerVolume() {
	gsm.print(F("AT+CRSL?\r"));
	_readSerial();

	String veri = _buffer.substring(7, _buffer.indexOf(F("OK")));
	veri.trim();

	return veri.toInt();
}

// Çalma sesini ayarla +
bool GSMSim::setRingerVolume(unsigned int level) {
	if(level > 100) {
		level = 100;
	}

	gsm.print(F("AT+CRSL="));
	gsm.print(level);
	gsm.print(F("\r"));
	_readSerial();

	if(_buffer.indexOf(F("OK")) != -1) {
		return true;
	} else {
		return false;
	}
}

// Hoparlör sesi +
unsigned int GSMSim::speakerVolume() {
	gsm.print(F("AT+CLVL?\r"));
	_readSerial();

	String veri = _buffer.substring(7, _buffer.indexOf(F("OK")));
	veri.trim();

	return veri.toInt();
}

// Hoparlör sesini ayarla +
bool GSMSim::setSpeakerVolume(unsigned int level) {
	if(level > 100) {
		level = 100;
	}

	gsm.print(F("AT+CLVL="));
	gsm.print(level);
	gsm.print(F("\r"));

	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}

// Modül Debug
String GSMSim::moduleDebug() {
	gsm.print(F("AT&V\r"));
	_readSerial(60000);

	return _buffer;
}
// Bazı fonksiyonların modül üzerine kaydedilmesini sağlar...
bool GSMSim::saveSettingsToModule() {
	gsm.print(F("AT&W\r"));
	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}





//////////////////////////////////////
//			PRIVATE METHODS			//
//////////////////////////////////////

// READ FROM SERIAL
void GSMSim::_readSerial() {

	_buffer = "";
	uint64_t timeOld = millis();
	// Bir süre bekle...
	while (!gsm.available() && !(millis() > timeOld + TIME_OUT_READ_SERIAL)) { ; }
	// beklemeden çıkınca ya da süre dolunca varsa seriali oku, yoksa çık git...
	if(gsm.available()) { _buffer = gsm.readString(); }
}

void GSMSim::_readSerial(uint32_t timeout) {
	_buffer = "";
	uint64_t timeOld = millis();
	// Bir süre bekle...
	while (!gsm.available() && !(millis() > timeOld + timeout)) { ; }
	// beklemeden çıkınca ya da süre dolunca varsa seriali oku, yoksa çık git...
	if(gsm.available()) { _buffer = gsm.readString(); }
}
