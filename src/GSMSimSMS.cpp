/*
 * GSMSim Library
 * 
 * This library written for SIMCOM modules. Tested on Sim800L. Library may worked on any SIMCOM and another GSM modules and GSM Shields. Some AT commands for only SIMCOM modules.
 *
 * Created 11.05.2017
 * By Erdem ARSLAN
 * Modified 06.05.2020
 *
 * Version: v.2.0.2
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
#include "GSMSimSMS.h"


//////////////////////////////////////
//			MESAJ BÖLÜMÜ			//
//////////////////////////////////////

bool GSMSimSMS::initSMS() {
	if(setTextMode(true)) {
		if(setPreferredSMSStorage("ME", "ME", "ME")) {
			if(setNewMessageIndication()) {
				if(setCharset("IRA")) {
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

// SMS i TEXT ya da PDU moduna alır. +
bool GSMSimSMS::setTextMode(bool textModeON) {
	if (textModeON == true) {
		gsm.print(F("AT+CMGF=1\r"));
	}
	else {
		gsm.print(F("AT+CMGF=0\r"));
	}
	_readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	} else {
		return false;
	}
}
// tercih edilen sms kayıt yeri +
bool GSMSimSMS::setPreferredSMSStorage(char* mem1, char* mem2, char* mem3) {
	gsm.print(F("AT+CPMS=\""));
	gsm.print(mem1);
	gsm.print(F("\",\""));
	gsm.print(mem2);
	gsm.print(F("\",\""));
	gsm.print(mem3);
	gsm.print(F("\"\r"));

	_readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	} else {
		return false;
	}
}
// yeni mesajı <mem>,<smsid> şeklinde geri dönmesi için ayarlar... +
bool GSMSimSMS::setNewMessageIndication() {
	gsm.print(F("AT+CNMI=2,1\r"));
	_readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	} else {
		return false;
	}
}

// charseti ayarlar
bool GSMSimSMS::setCharset(char* charset) {
	gsm.print(F("AT+CSCS=\""));
	gsm.print(charset);
	gsm.print(F("\"\r"));
	_readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	} else {
		return false;
	}
}

// verilen numara ve mesajı gönderir! +
bool GSMSimSMS::send(char* number, char* message) {

	String str = "";
	gsm.print(F("AT+CMGS=\""));  // command to send sms
	gsm.print(number);
	gsm.print(F("\"\r"));
	_readSerial();
	str += _buffer;
	gsm.print(message);
	gsm.print("\r");
	//change delay 100 to readserial
	//_buffer += _readSerial();
	_readSerial();
	str += _buffer;
	gsm.print((char)26);

	//_buffer += _readSerial();
	_readSerial();
	str += _buffer;
	//expect CMGS:xxx   , where xxx is a number,for the sending sms.
	
	return str;
	/**/
	if (str.indexOf("+CMGS:") != -1) {
		return true;
	}
	else {
		return false;
	}
}

// Belirtilen klasördeki smslerin indexlerini listeler! +
String GSMSimSMS::list(bool onlyUnread) {

	if(onlyUnread) {
		gsm.print(F("AT+CMGL=\"REC UNREAD\",1\r"));
	} else {
		// hepsi
		gsm.print(F("AT+CMGL=\"ALL\",1\r"));
	}

	_readSerial(30000);

	//return _buffer;

	String returnData = "";

	if(_buffer.indexOf("ERROR") != -1) {
		returnData = "ERROR";
	} else {
		// +CMGL: varsa döngüye girelim. yoksa sadece OK dönmüştür. O zaman NO_SMS diyelim
		if(_buffer.indexOf("+CMGL:") != -1) {
			String data = _buffer;
			bool quitLoop = false;
			returnData = "";

			while(!quitLoop) {
				if(data.indexOf("+CMGL:") == -1) {
					quitLoop = true;
					continue;
				}

				data = data.substring(data.indexOf("+CMGL: ") + 7);
				String metin = data.substring(0, data.indexOf(","));
				metin.trim();

				if(returnData == "") {
					returnData += "SMSIndexNo:";
					returnData += metin;
				} else {
					returnData += ",";
					returnData += metin;
				}

			}
		} else {
			returnData = "NO_SMS";
		}
	}

	return returnData;
}

// Indexi verilen mesajı okur. Anlaşılır hale getirir! +
String GSMSimSMS::read(unsigned int index) {
	gsm.print("AT+CMGR=");
	gsm.print(index);
	gsm.print(",0\r");

	_readSerial(30000);

	//return _buffer;

	String durum = "INDEX_NO_ERROR";

	if (_buffer.indexOf("+CMGR:") != -1) {

		String klasor, okundumu, telno, zaman, mesaj;

		klasor = "UNKNOWN";
		okundumu = "UNKNOWN";

		if (_buffer.indexOf("REC UNREAD") != -1) {
			klasor = "INCOMING";
			okundumu = "UNREAD";
		}
		if (_buffer.indexOf("REC READ") != -1) {
			klasor = "INCOMING";
			okundumu = "READ";
		}
		if (_buffer.indexOf("STO UNSENT") != -1) {
			klasor = "OUTGOING";
			okundumu = "UNSENT";
		}
		if (_buffer.indexOf("STO SENT") != -1) {
			klasor = "OUTGOING";
			okundumu = "SENT";
		}

		String telno_bol1 = _buffer.substring(_buffer.indexOf("\",\"") + 3);
		telno = telno_bol1.substring(0, telno_bol1.indexOf("\",\"")); // telefon numarası tamam

		String tarih_bol = telno_bol1.substring(telno_bol1.lastIndexOf("\",\"") + 3);

		zaman = tarih_bol.substring(0, tarih_bol.indexOf("\"")); // zamanı da aldık. Bir tek mesaj kaldı!

		mesaj = tarih_bol.substring(tarih_bol.indexOf("\"") + 1, tarih_bol.lastIndexOf("OK"));

		mesaj.trim();

		// Little Fix for incoming messaged from iPhone
		String messageHex = "";
		for(int i = 0; i < mesaj.length(); i++) {
			messageHex += String(mesaj[i], HEX);
		}

		if(messageHex.indexOf("ffa5ffa4ffa3ffa3") != -1) {
			mesaj = mesaj.substring(4);
		}

		durum = "FOLDER:";
		durum += klasor;
		durum += "|STATUS:";
		durum += okundumu;
		durum += "|PHONENO:";
		durum += telno;
		durum += "|DATETIME:";
		durum += zaman;
		durum += "|MESSAGE:";
		durum += mesaj;
	}

	return durum;
}
// Indexi verilen mesajı okur. Anlaşılır hale getirir! +
String GSMSimSMS::read(unsigned int index, bool markRead) {
	gsm.print("AT+CMGR=");
	gsm.print(index);
	gsm.print(",");
	if (markRead == true) {
		gsm.print("0");
	}
	else {
		gsm.print("1");
	}
	gsm.print("\r");

	_readSerial(30000);

	//return _buffer;

	String durum = "INDEX_NO_ERROR";

	if (_buffer.indexOf("+CMGR:") != -1) {

		String klasor, okundumu, telno, zaman, mesaj;

		klasor = "UNKNOWN";
		okundumu = "UNKNOWN";

		if (_buffer.indexOf("REC UNREAD") != -1) {
			klasor = "INCOMING";
			okundumu = "UNREAD";
		}
		if (_buffer.indexOf("REC READ") != -1) {
			klasor = "INCOMING";
			okundumu = "READ";
		}
		if (_buffer.indexOf("STO UNSENT") != -1) {
			klasor = "OUTGOING";
			okundumu = "UNSENT";
		}
		if (_buffer.indexOf("STO SENT") != -1) {
			klasor = "OUTGOING";
			okundumu = "SENT";
		}

		String telno_bol1 = _buffer.substring(_buffer.indexOf("\",\"") + 3);
		telno = telno_bol1.substring(0, telno_bol1.indexOf("\",\"")); // telefon numarası tamam

		String tarih_bol = telno_bol1.substring(telno_bol1.lastIndexOf("\",\"") + 3);

		zaman = tarih_bol.substring(0, tarih_bol.indexOf("\"")); // zamanı da aldık. Bir tek mesaj kaldı!

		mesaj = tarih_bol.substring(tarih_bol.indexOf("\"")+1, tarih_bol.lastIndexOf("OK"));

		mesaj.trim();

		
		// Little Fix for incoming messaged from iPhone
		String messageHex = "";
		for(int i = 0; i < mesaj.length(); i++) {
			messageHex += String(mesaj[i], HEX);
		}

		if(messageHex.indexOf("ffa5ffa4ffa3ffa3") != -1) {
			mesaj = mesaj.substring(4);
		}

		durum = "FOLDER:";
		durum += klasor;
		durum += "|STATUS:";
		durum += okundumu;
		durum += "|PHONENO:";
		durum += telno;
		durum += "|DATETIME:";
		durum += zaman;
		durum += "|MESSAGE:";
		durum += mesaj;
	}

	return durum;
}

// Verilen indexten mesajı gönderen kişiyi bulur. +
String GSMSimSMS::getSenderNo(unsigned int index) {
	String message = read(index, false);

	if(message.indexOf(F("PHONENO:")) == -1) {
		return "NO MESSAGE";
	} else {
		String no = message.substring(message.indexOf("PHONENO:")+8, message.indexOf("|DATETIME:"));
		no.trim();
		return no;
	}
}

// Serialden Mesajı okur +
String GSMSimSMS::readFromSerial(String serialRaw) {
	if (serialRaw.indexOf("+CMTI:") != -1) {
		String numara = serialRaw.substring(serialRaw.indexOf("\",") + 2);
		numara.trim();
		int no = numara.toInt();

		return read(no, true);
	}
	else {
		return "RAW_DATA_NOT_READ";
	}
}

// serialden mesajın indexini alır +
unsigned int GSMSimSMS::indexFromSerial(String serialRaw) {
	if (serialRaw.indexOf("+CMTI:") != -1) {
		String numara = serialRaw.substring(serialRaw.indexOf("\",") + 2);
		numara.trim();
		int no = numara.toInt();

		return no;
	}
	else {
		return -1;
	}
}

// mesaj merkez numasını getirir +
String GSMSimSMS::readMessageCenter() {
	gsm.print("AT+CSCA?\r");
	_readSerial();

	String sonuc = "";

	if (_buffer.indexOf("+CSCA:") != -1)
	{
		sonuc = _buffer.substring(_buffer.indexOf("+CSCA:")+8, _buffer.indexOf("\","));
	}

	return sonuc;
}

// mesaj merkez numarasını değiştirir +
bool GSMSimSMS::changeMessageCenter(char* messageCenter) {
	gsm.print("AT+CSCA=\"");
	gsm.print(messageCenter);
	gsm.print("\"\r");

	_readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}

// tek bir mesajı siler +
bool GSMSimSMS::deleteOne(unsigned int index) {
	gsm.print(F("AT+CMGD="));
	gsm.print(index);
	gsm.print(F(",0\r"));

	_readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}

// Tüm okunmuş mesajlaarı siler. Fakat gidene dokunmaz +
bool GSMSimSMS::deleteAllRead() {
	gsm.print(F("AT+CMGD=1,1\r"));

	_readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}

// okunmuş okunmamış ne varsa siler +
bool GSMSimSMS::deleteAll() {
	gsm.print(F("AT+CMGD=1,4\r"));

	_readSerial(30000);

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}