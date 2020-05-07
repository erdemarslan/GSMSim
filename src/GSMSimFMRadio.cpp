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
#include "GSMSimFMRadio.h"

//////////////////////////////////////
//			FM RADIO SECTION		//
//////////////////////////////////////

// SIM800L & SIM800H only

// FM RADIO Open
bool GSMSimFMRadio::fmOpen() {
	gsm.print(F("AT+FMOPEN=0\r"));
	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}
bool GSMSimFMRadio::fmOpen(bool mainChannel) {
	uint8_t channel = mainChannel == true ? 1 : 0;
	gsm.print(F("AT+FMOPEN="));
	gsm.print(channel);
	gsm.print(F("\r"));

	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}
bool GSMSimFMRadio::fmOpen(bool mainChannel, uint16_t freq) {
	uint8_t channel = mainChannel == true ? 1 : 0;
	
	if (freq < 875) {
		freq = 875;
	}
	if (freq > 1080) {
		freq = 1080;
	}


	gsm.print(F("AT+FMOPEN="));
	gsm.print(channel);
	gsm.print(",");
	gsm.print(freq);
	gsm.print(F("\r"));

	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}
// FM IS OPENED?
bool GSMSimFMRadio::fmIsOpened() {
	gsm.print(F("AT+FMOPEN?\r"));
	_readSerial();

	if (_buffer.indexOf(F("+FMOPEN: 1")) != -1) {
		return true;
	}
	else {
		return false;
	}
}
// FM RADIO CLOSE
bool GSMSimFMRadio::fmClose() {
	gsm.print(F("AT+FMCLOSE\r"));
	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}
// GET FM RADIO FREQ
uint16_t GSMSimFMRadio::fmGetFreq() {
	gsm.print(F("AT+FMFREQ?\r"));
	_readSerial();

	if (_buffer.indexOf(F("+FMFREQ:")) != -1) {
		String sonuc = _buffer.substring(_buffer.indexOf(F("+FMFREQ:"))+8);
		sonuc.trim();
		return sonuc.toInt();
	}
	else {
		return 0;
	}
}
// SET FM RADIO FREQ
bool GSMSimFMRadio::fmSetFreq(uint16_t freq) {
	gsm.print(F("AT+FMFREQ="));
	
	if (freq < 875) {
		freq = 875;
	}
	if (freq > 1080) {
		freq = 1080;
	}
	gsm.print(freq);
	gsm.print(F("\r"));

	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}
// GET FM RADIO Volume
uint8_t GSMSimFMRadio::fmGetVolume() {
	gsm.print(F("AT+FMVOLUME?\r"));
	_readSerial();

	if (_buffer.indexOf(F("+FMVOLUME:")) != -1) {
		String sonuc = _buffer.substring(_buffer.indexOf(F("+FMVOLUME:"))+10);
		sonuc.trim();
		return sonuc.toInt();
	}
	else {
		return 0;
	}
}
// SET FM RADIO Volume
bool GSMSimFMRadio::fmSetVolume(uint8_t volume) {
	gsm.print(F("AT+FMVOLUME="));
	uint8_t vol = 0;
	if (volume < 0) {
		vol = 0;
	}
	if (volume > 6) {
		vol = 6;
	}
	gsm.print(vol);
	gsm.print(F("\r"));

	_readSerial();

	if (_buffer.indexOf(F("OK")) != -1) {
		return true;
	}
	else {
		return false;
	}
}