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
#include "GSMSimDTMF.h"
#include "GSMSimCall.h"

//////////////////////////////////////
//			DTMF BÖLÜMÜ				//
//////////////////////////////////////

// DTMF yi ayarlar
bool GSMSimDTMF::setDTMF(bool active, unsigned int interval, bool reportTime, bool soundDetect) {
	int mode = active == true ? 1 : 0;
	int rtime = reportTime == true ? 1 : 0;
	int ssdet = soundDetect == true ? 1 : 0;

	gsm.print(F("AT+DDET="));
	gsm.print(mode);
	gsm.print(F(","));
	gsm.print(interval);
	gsm.print(F(","));
	gsm.print(rtime);
	gsm.print(F(","));
	gsm.print(ssdet);
	gsm.print(F("\r"));

	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}
// Serialden DTMF Yi okur ve karakter olarak geri döner!
String GSMSimDTMF::readDTMF(String serialRaw) {

	serialRaw.trim();
	
	if (serialRaw.indexOf(F("+DTMF:")) != -1) {
		//  var mı yok mu?
		String metin;
		if (serialRaw.indexOf(F(",")) != -1) {
			metin = serialRaw.substring(7, serialRaw.indexOf(","));
		}
		else {
			metin = serialRaw.substring(7);
		}

		return metin;
	}
	else {
		return "NOT_DTMF_RAW";
	}
}