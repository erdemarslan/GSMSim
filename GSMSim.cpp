/*
	GSMSim Library

	This library written for SIMCOM Sim800L module. Library may worked on any SIMCOM modules
	and GSM Shields.

	Created 11.05.2017
	By Erdem ARSLAN
	Modified 30.08.2017

	Erdem ARSLAN
	Science and Technology Teacher, an Arduino Lover =)
	erdemsaid@gmail.com
	https://www.erdemarslan.com/
	http://www.robothane.net/
	http://www.eralabs.net/

*/

#include "Arduino.h"
#include "GSMSim.h"
#include <SoftwareSerial.h>

GSMSim::GSMSim(void) : SoftwareSerial(DEFAULT_RX_PIN, DEFAULT_TX_PIN)
{
	RX_PIN = DEFAULT_RX_PIN;
	TX_PIN = DEFAULT_TX_PIN;
	RESET_PIN = DEFAULT_RST_PIN;
	LED_PIN = DEFAULT_LED_PIN;
	LED_FLAG = DEFAULT_LED_FLAG;
}

GSMSim::GSMSim(uint8_t rx, uint8_t tx) : SoftwareSerial(rx, tx)
{
	RX_PIN = rx;
	TX_PIN = tx;
	RESET_PIN = DEFAULT_RST_PIN;
	LED_PIN = DEFAULT_LED_PIN;
	LED_FLAG = DEFAULT_LED_FLAG;
}

GSMSim::GSMSim(uint8_t rx, uint8_t tx, uint8_t rst) : SoftwareSerial(rx, tx)
{
	RX_PIN = rx;
	TX_PIN = tx;
	RESET_PIN = rst;
	LED_PIN = DEFAULT_LED_PIN;
	LED_FLAG = DEFAULT_LED_FLAG;
}

GSMSim::GSMSim(uint8_t rx, uint8_t tx, uint8_t rst, uint8_t led) : SoftwareSerial(rx, tx)
{
	RX_PIN = rx;
	TX_PIN = tx;
	RESET_PIN = rst;
	LED_PIN = led;
	LED_FLAG = DEFAULT_LED_FLAG;
}

GSMSim::GSMSim(uint8_t rx, uint8_t tx, uint8_t rst, uint8_t led, bool ledflag) : SoftwareSerial(rx, tx)
{
	RX_PIN = rx;
	TX_PIN = tx;
	RESET_PIN = rst;
	LED_PIN = led;
	LED_FLAG = ledflag;
}



// Start GSMSim
void GSMSim::start() {

	pinMode(RESET_PIN, OUTPUT);
	digitalWrite(RESET_PIN, HIGH);

	_baud = DEFAULT_BAUD_RATE;

	this->begin(_baud);

	if (LED_FLAG) {
		pinMode(LED_PIN, OUTPUT);
	}

	_buffer.reserve(BUFFER_RESERVE_MEMORY);
}
void GSMSim::start(uint32_t baud) {
	pinMode(RESET_PIN, OUTPUT);
	digitalWrite(RESET_PIN, HIGH);

	_baud = baud;

	this->begin(_baud);

	if (LED_FLAG) {
		pinMode(LED_PIN, OUTPUT);
	}

	_buffer.reserve(BUFFER_RESERVE_MEMORY);
}

// Reset GMS Module
void GSMSim::reset() {
	if (LED_FLAG) {
		digitalWrite(LED_PIN, HIGH);
	}

	digitalWrite(RESET_PIN, LOW);
	delay(1000);
	digitalWrite(RESET_PIN, HIGH);
	delay(1000);

	// Modul kendine geldi mi onu bekle
	this->print(F("AT\r"));
	while (_readSerial().indexOf("OK") == -1) {
		this->print(F("AT\r"));
	}

	if (LED_FLAG) {
		digitalWrite(LED_PIN, LOW);
	}
}



// SET PHONE FUNC
bool GSMSim::setPhoneFunc(uint8_t level = 1) {
	if(level != 0 || level != 1 || level != 4) {
		return false;
	}
	else {
		this->print(F("AT+CFUN="));
		this->print(String(level));
		this->print(F("\r"));

		_buffer = _readSerial();
		if( (_buffer.indexOf("OK") ) != -1)  {
			return true;
		}
		else {
			return false;
		}
	}
}

// SIGNAL QUALTY - 0-31 | 0-> poor | 31 - Full | 99 -> Unknown
uint8_t GSMSim::signalQuality() {
	this->print(F("AT+CSQ\r"));
	_buffer = _readSerial();

	if((_buffer.indexOf("+CSQ:")) != -1) {
		return _buffer.substring(_buffer.indexOf("+CSQ: ")+6, _buffer.indexOf(",")).toInt();
	} else {
		return 99;
	}
}


// IS Module connected to the operator?
bool GSMSim::isRegistered() {
	this->print(F("AT+CREG?\r"));
	_buffer = _readSerial();

	if( (_buffer.indexOf("+CREG: 0,1")) != -1 || (_buffer.indexOf("+CREG: 0,5")) != -1 || (_buffer.indexOf("+CREG: 1,1")) != -1 || (_buffer.indexOf("+CREG: 1,5")) != -1) {
		return true;
	} else {
		return false;
	}
}

// IS SIM Inserted?
bool GSMSim::isSimInserted() {
	this->print(F("AT+CSMINS?\r"));
	_buffer = _readSerial();
	if(_buffer.indexOf(",") != -1) {
		// bölelim
		String veri = _buffer.substring(_buffer.indexOf(","), _buffer.indexOf("OK"));
		veri.trim();
		if(veri == "1") {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

// Pin statüsü - AT+CPIN?
uint8_t GSMSim::pinStatus() {
	this->print(F("AT+CPIN?\r"));
	_buffer = _readSerial();

	if(_buffer.indexOf("READY") != -1)
	{
		return 0;
	}
	else if(_buffer.indexOf("SIM PIN") != -1)
	{
		return 1;
	}
	else if(_buffer.indexOf("SIM PUK") != -1)
	{
		return 2;
	}
	else if(_buffer.indexOf("PH_SIM PIN") != -1)
	{
		return 3;
	}
	else if(_buffer.indexOf("PH_SIM PUK") != -1)
	{
		return 4;
	}
	else if(_buffer.indexOf("SIM PIN2") != -1)
	{
		return 5;
	}
	else if(_buffer.indexOf("SIM PUK2") != -1)
	{
		return 6;
	}
	else {
		return 7;
	}
}


// OPERATOR NAME
String GSMSim::operatorName() {
	this->print(F("AT+COPS?\r"));
	_buffer = _readSerial();

	if(_buffer.indexOf(",") == -1) {
		return "NOT CONNECTED";
	}
	else {
		 return _buffer.substring(_buffer.indexOf(",\"")+2, _buffer.lastIndexOf("\""));
	}
}

// OPERATOR NAME FROM SIM
String GSMSim::operatorNameFromSim() {
	this->flush();
	this->print(F("AT+CSPN?\r"));
	_buffer = _readSerial();
	delay(250);
	_buffer = _readSerial();
	/*
	return _buffer;
	*/
	if(_buffer.indexOf("OK") != -1) {
		return _buffer.substring(_buffer.indexOf(" \"") + 2, _buffer.lastIndexOf("\""));
	}
	else {
		return "NOT CONNECTED";
	}

}

// PHONE STATUS
uint8_t GSMSim::phoneStatus() {
	this->print(F("AT+CPAS\r"));
	_buffer = _readSerial();

	if((_buffer.indexOf("+CPAS: ")) != -1)
	{
		return _buffer.substring(_buffer.indexOf("+CPAS: ")+7,_buffer.indexOf("+CPAS: ")+9).toInt();
	}
	else {
		return 99; // not read from module
	}
}

// ECHO OFF
bool GSMSim::echoOff() {
	this->print(F("ATE0\r"));
	_buffer = _readSerial();
	if ( (_buffer.indexOf("OK") )!=-1 ) {
   		return true;
   }
   else {
   	return false;
   }
}

// ECHO ON
bool GSMSim::echoOn() {
	this->print(F("ATE1\r"));
	_buffer = _readSerial();
	if ( (_buffer.indexOf("OK") )!=-1 ) {
   		return true;
   }
   else {
   	return false;
   }
}

// Modül Üreticisi
String GSMSim::moduleManufacturer() {
	this->print(F("AT+CGMI\r"));
	_buffer = _readSerial();
	String veri = _buffer.substring(8, _buffer.indexOf("OK"));
	veri.trim();
	veri.replace("_", " ");
	return veri;
}

// Modül Modeli
String GSMSim::moduleModel() {
	this->print(F("AT+CGMM\r"));
	_buffer = _readSerial();

	String veri = _buffer.substring(8, _buffer.indexOf("OK"));
	veri.trim();
	veri.replace("_", " ");
	return veri;
}

// Modül Revizyonu
String GSMSim::moduleRevision() {
	this->print(F("AT+CGMR\r"));
	_buffer = _readSerial();

	String veri = _buffer.substring(_buffer.indexOf(":")+1 , _buffer.indexOf("OK"));
	veri.trim();
	return veri;
}

// Modülün IMEI numarası
String GSMSim::moduleIMEI() {
	this->print(F("AT+CGSN\r"));
	_buffer = _readSerial();

	String veri = _buffer.substring(8, _buffer.indexOf("OK"));
	veri.trim();
	return veri;
}

// Modülün IMEI Numarasını değiştirir.
bool GSMSim::moduleIMEIChange(char* imeino) {
	return true;
}

// Modülün SIM Numarası
String GSMSim::moduleIMSI() {
	this->print(F("AT+CIMI\r"));
	_buffer = _readSerial();

	String veri = _buffer.substring(8, _buffer.indexOf("OK"));
	veri.trim();
	return veri;
}

// Sim Kart Seri Numarası
String GSMSim::moduleICCID() {
	this->print(F("AT+CCID\r"));
	_buffer = _readSerial();

	String veri = _buffer.substring(8, _buffer.indexOf("OK"));
	veri.trim();

	return veri;
}

// Çalma Sesi
uint8_t GSMSim::ringerVolume() {
	this->print(F("AT+CRSL?\r"));
	_buffer = _readSerial();

	String veri = _buffer.substring(7, _buffer.indexOf("OK"));
	veri.trim();

	return veri.toInt();
}

// Çalma sesini ayarla
bool GSMSim::setRingerVolume(uint8_t level) {
	if(level > 100) {
		level = 100;
	}

	this->print(F("AT+CRSL="));
	this->print(level);
	this->print(F("\r"));
	_buffer = _readSerial();

	if(_buffer.indexOf("OK") != -1) {
		return true;
	} else {
		return false;
	}
}

// Hoparlör sesi
uint8_t GSMSim::speakerVolume() {
	this->print(F("AT+CLVL?\r"));
	_buffer = _readSerial();

	String veri = _buffer.substring(7, _buffer.indexOf("OK"));
	veri.trim();

	return veri.toInt();
}

// Hoparlör sesini ayarla
bool GSMSim::setSpeakerVolume(uint8_t level) {
	if(level > 100) {
		level = 100;
	}

	this->print(F("AT+CLVL="));
	this->print(level);
	this->print(F("\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}

String GSMSim::moduleDebug() {
	this->print(F("AT&V\r"));

	return _readSerial();
}


//////////////////////////////////////
//			CALL	SECTION			//
//////////////////////////////////////

// Arama Yapar
bool GSMSim::call(char* phone_number) {

	bool sorgulamaYapma = callIsCOLPActive();
	_buffer = _readSerial();
	delay(100);

	this->print(F("ATD"));
	this->print(phone_number);
	this->print(";\r");

	if (sorgulamaYapma) {
		return true;
	}
	else {
		_buffer = _readSerial();

		if (_buffer.indexOf("OK") != -1)
		{
			return true;
		}
		else {
			return false;
		}
	}
}

// Gelen aramayı cevaplar
bool GSMSim::callAnswer() {
	this->print(F("ATA\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1)
	{
		return true;
	}
	else {
		false;
	}
}

// Aramayı reddeder veya görüşmeyi sonlandırır!
bool GSMSim::callHangoff() {
	this->print(F("ATH\r"));
	_buffer = _readSerial();

	if(_buffer.indexOf("OK") != -1)
	{
		return true;
	} else {
		false;
	}
}

// Arama durumunu belirtir
uint8_t GSMSim::callStatus() {
	/*
		values of return:
		0 Ready (MT allows commands from TA/TE)
		2 Unknown (MT is not guaranteed to respond to tructions)
		3 Ringing (MT is ready for commands from TA/TE, but the ringer is active)
		4 Call in progress
	*/
	this->print(F("AT+CPAS\r"));
	_buffer = _readSerial();
	return _buffer.substring(_buffer.indexOf("+CPAS: ") + 7, _buffer.indexOf("+CPAS: ") + 9).toInt();
}

// Connected Line Identification aktif veya kapalı
bool GSMSim::callSetCOLP(bool active) {
	int durum = active == true ? 1 : 0;
	this->print(F("AT+COLP="));
	this->print(durum);
	this->print("\r");

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1)
	{
		return true;
	}
	else {
		false;
	}
}

// COLP Aktif mi değil mi?
bool GSMSim::callIsCOLPActive() {
	this->print("AT+COLP?\r");
	_buffer = _readSerial();

	if (_buffer.indexOf("+COLP: 1") != -1) {
		return true;
	}
	else {
		false;
	}
}

// Arayanı söyleme aktif mi değil mi?
bool GSMSim::callActivateListCurrent(bool active) {
	int durum = active == true ? 1 : 0;
	this->print(F("AT+CLCC="));
	this->print(durum);
	this->print("\r");

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1)
	{
		return true;
	}
	else {
		false;
	}
}
// şimdi arayanı söyle
String GSMSim::callReadCurrentCall(String serialRaw) {

	String sonuc = "";
	if (serialRaw.indexOf("+CLCC:") != -1) {
		String durum = serialRaw.substring(11,13);
		String numara = serialRaw.substring(18, serialRaw.indexOf("\","));

		if (durum == "0") {
			durum = "STATUS:ACTIVE"; // Görüşme var
		}
		else if (durum == "1") {
			durum = "STATUS:HELD";
		}
		else if (durum == "2") {
			durum = "STATUS:DIALING"; // Çevriliyor
		}
		else if (durum == "3") {
			durum = "STATUS:ALERTING"; // Çalıyor
		}
		else if (durum == "4") {
			durum = "STATUS:INCOMING"; // Gelen arama
		}
		else if (durum == "5") {
			durum = "STATUS:WAITING"; // gelen arama bekliyor
		}
		else if (durum == "6") {
			durum = "STATUS:DISCONNECT"; // görüşme bitti
		}

		sonuc = durum + "|NUMBER:" + numara;
	}

	return sonuc;
}


//////////////////////////////////////
//			MESAJ BÖLÜMÜ			//
//////////////////////////////////////

// SMS i TEXT ya da PDU moduna alır.
bool GSMSim::smsTextMode(bool textModeON) {
	if (textModeON == true) {
		this->print(F("AT+CMGF=1\r"));
	}
	else {
		this->print(F("AT+CMGF=0\r"));
	}
	bool sonuc = false;
	_buffer = _readSerial();
	if (_buffer.indexOf("OK") != -1) {
		sonuc = true;
	}

	return sonuc;
}

// verilen numara ve mesajı gönderir!
bool GSMSim::smsSend(char* number, char* message) {
	this->print(F("AT+CMGS=\""));  // command to send sms
	this->print(number);
	this->print(F("\"\r"));
	_buffer = _readSerial();
	this->print(message);
	this->print("\r");
	//change delay 100 to readserial
	_buffer += _readSerial();
	this->print((char)26);

	_buffer += _readSerial();
	//expect CMGS:xxx   , where xxx is a number,for the sending sms.
	/*
	return _buffer;
	*/
	if (((_buffer.indexOf("AT+CMGS")) != -1)) {
		return true;
	}
	else {
		return false;
	}
}

// Belirtilen klasördeki smslerin indexlerini listeler!
String GSMSim::smsListUnread() {

	this->print(F("AT+CMGL=\"REC UNREAD\",1\r"));

	_buffer = _readSerial();

	String donus = "";

	if (_buffer.indexOf("ERROR") != -1) {
		donus = "ERROR";
	}


	if (_buffer.indexOf("+CMGL:") != -1) {

		String veri = _buffer;
		bool islem = false;
		donus = "";

		while (!islem) {
			if (veri.indexOf("+CMGL:") == -1) {
				islem = true;
				continue;
			}

			veri = veri.substring(veri.indexOf("+CMGL: ") + 7);
			String metin = veri.substring(0, veri.indexOf(","));
			metin.trim();

			if (donus == "") {
				donus += "SMSIndexNo:";
				donus += metin;
			}
			else {
				donus += ",";
				donus += metin;
			}

		}

	}
	else {
		if (donus != "ERROR") {
			donus = "NO_SMS";
		}
	}

	return donus;
}

// Indexi verilen mesajı okur. Anlaşılır hale getirir!
String GSMSim::smsRead(uint8_t index) {
	this->print("AT+CMGR=");
	this->print(index);
	this->print(",0\r");

	_buffer = _readSerial();

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
// Indexi verilen mesajı okur. Anlaşılır hale getirir!
String GSMSim::smsRead(uint8_t index, bool markRead) {
	this->print("AT+CMGR=");
	this->print(index);
	this->print(",");
	if (markRead == true) {
		this->print("0");
	}
	else {
		this->print("1");
	}
	this->print("\r");

	_buffer = _readSerial();

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

// Serialden Mesajı okur
String GSMSim::smsReadFromSerial(String serialRaw) {
	if (serialRaw.indexOf("+CMTI:") != -1) {
		String numara = serialRaw.substring(serialRaw.indexOf("\",") + 2);
		int no = numara.toInt();

		return smsRead(no, true);
	}
	else {
		return "RAW_DATA_NOT_READ";
	}
}

// serialden mesajın indexini alır
uint8_t GSMSim::smsIndexFromSerial(String serialRaw) {
	if (serialRaw.indexOf("+CMTI:") != -1) {
		String numara = serialRaw.substring(serialRaw.indexOf("\",") + 2);
		int no = numara.toInt();

		return no;
	}
	else {
		return -1;
	}
}

// mesaj merkez numasını getirir
String GSMSim::smsReadMessageCenter() {
	this->print("AT+CSCA?\r");
	_buffer = _readSerial();

	String sonuc = "";

	if (_buffer.indexOf("+CSCA:") != -1)
	{
		sonuc = _buffer.substring(_buffer.indexOf("+CSCA:")+8, _buffer.indexOf("\","));
	}

	return sonuc;
}

// mesaj merkez numarasını değiştirir
bool GSMSim::smsChangeMessageCenter(char* messageCenter) {
	this->print("AT+CSCA=\"");
	this->print(messageCenter);
	this->print("\"\r");

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}

// tek bir mesajı siler
bool GSMSim::smsDeleteOne(uint8_t index) {
	this->print(F("AT+CMGD="));
	this->print(index);
	this->print(F(",0\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}

}

// Tüm okunmuş mesajlaarı siler. Fakat gidene dokunmaz
bool GSMSim::smsDeleteAllRead() {
	this->print(F("AT+CMGD=1,1\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}

// okunmuş okunmamış ne varsa siler
bool GSMSim::smsDeleteAll() {
	this->print(F("AT+CMGD=1,4\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}


//////////////////////////////////////
//			DTMF BÖLÜMÜ				//
//////////////////////////////////////

// DTMF yi ayarlar
bool GSMSim::dtmfSet(bool active, uint8_t interval, bool reportTime, bool soundDetect) {
	int mode = active == true ? 1 : 0;
	int rtime = reportTime == true ? 1 : 0;
	int ssdet = soundDetect == true ? 1 : 0;

	this->print(F("AT+DDET="));
	this->print(mode);
	this->print(F(","));
	this->print(interval);
	this->print(F(","));
	this->print(rtime);
	this->print(F(","));
	this->print(ssdet);
	this->print(F("\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}
// Serialden DTMF Yi okur ve karakter olarak geri döner!
String GSMSim::dtmfRead(String serialRaw) {

	if (serialRaw.indexOf("+DTMF:") != -1) {
		//  var mı yok mu?
		String metin;
		if (serialRaw.indexOf(",") != -1) {
			metin = serialRaw.substring(7, serialRaw.indexOf(","));
		}
		else {
			metin = serialRaw.substring(7);
		}

		return metin;
	}
	else {
		return "?";
	}

}


//////////////////////////////////////
//			USSD SECTION			//
//////////////////////////////////////
// USSD kodu gönderir
bool GSMSim::ussdSend(char* code) {
	this->print(F("AT+CUSD=1,\""));
	this->print(code);
	this->print(F("\"\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}
// Raw datadan cevabı okur!
String GSMSim::ussdRead(String serialRaw) {
	if (serialRaw.indexOf("+CUSD:") != -1) {
		String metin = serialRaw.substring(serialRaw.indexOf(",\"") + 2, serialRaw.indexOf("\","));
		return metin;
	}
	else {
		return "NOT_USSD_RAW";
	}
}


//////////////////////////////////////
//			FM RADIO SECTION		//
//////////////////////////////////////

// SIM800L & SIM800H only

// FM RADIO Open
bool GSMSim::fmOpen() {
	this->print(F("AT+FMOPEN=0\r"));
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}
bool GSMSim::fmOpen(bool mainChannel) {
	uint8_t channel = mainChannel == true ? 1 : 0;
	this->print(F("AT+FMOPEN="));
	this->print(channel);
	this->print(F("\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}
bool GSMSim::fmOpen(bool mainChannel, uint8_t freq) {
	uint8_t channel = mainChannel == true ? 1 : 0;
	uint8_t frekans = 875;
	if (freq < 875) {
		frekans = 875;
	}
	if (freq > 1080) {
		frekans = 1080;
	}


	this->print(F("AT+FMOPEN="));
	this->print(channel);
	this->print(",");
	this->print(frekans);
	this->print(F("\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}

// FM IS OPENED?
bool GSMSim::fmIsOpened() {
	this->print(F("AT+FMOPEN?\r"));
	_buffer = _readSerial();

	if (_buffer.indexOf("+FMOPEN: 1") != -1) {
		return true;
	}
	else {
		return false;
	}
}

// FM RADIO CLOSE
bool GSMSim::fmClose() {
	this->print(F("AT+FMCLOSE\r"));
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}

// GET FM RADIO FREQ
uint8_t GSMSim::fmGetFreq() {
	this->print(F("AT+FMFREQ?\r"));
	_buffer = _readSerial();

	if (_buffer.indexOf("+FMFREQ:") != -1) {
		String sonuc = _buffer.substring(_buffer.indexOf("+FMFREQ:")+8);
		sonuc.trim();
		return sonuc.toInt();
	}
	else {
		return 0;
	}
}

// SET FM RADIO FREQ
bool GSMSim::fmSetFreq(uint8_t freq) {
	this->print(F("AT+FMFREQ="));
	uint8_t frekans = 875;
	if (freq < 875) {
		frekans = 875;
	}
	if (freq > 1080) {
		frekans = 1080;
	}
	this->print(frekans);
	this->print(F("\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}

// GET FM RADIO FREQ
uint8_t GSMSim::fmGetVolume() {
	this->print(F("AT+FMVOLUME?\r"));
	_buffer = _readSerial();

	if (_buffer.indexOf("+FMVOLUME:") != -1) {
		String sonuc = _buffer.substring(_buffer.indexOf("+FMVOLUME:")+10);
		sonuc.trim();
		return sonuc.toInt();
	}
	else {
		return 0;
	}
}

// SET FM RADIO FREQ
bool GSMSim::fmSetVolume(uint8_t volume) {
	this->print(F("AT+FMVOLUME="));
	uint8_t vol = 0;
	if (volume < 0) {
		vol = 0;
	}
	if (volume > 6) {
		vol = 6;
	}
	this->print(vol);
	this->print(F("\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}


//////////////////////////////////////
//			GPRS METHODS			//
//////////////////////////////////////
// Connect to GPRS Bearer




bool GSMSim::gprsConnectBearer() {
	this->print(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r"));
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {

		delay(100);
		this->print(F("AT+SAPBR=3,1,\"APN\",\"internet\"\r"));
		_buffer = _readSerial();

		if (_buffer.indexOf("OK") != -1) {
			delay(100);

			this->print(F("AT+SAPBR=3,1,\"USER\",\"\"\r"));
			_buffer = _readSerial();

			if (_buffer.indexOf("OK") != -1) {

				delay(100);

				this->print(F("AT+SAPBR=3,1,\"PWD\",\"\"\r"));
				_buffer = _readSerial();

				if (_buffer.indexOf("OK") != -1) {

					delay(100);
					this->print("AT+SAPBR=1,1\r");
					_buffer = _readSerial();
					delay(50);
					_buffer += _readSerial();
					if (_buffer.indexOf("OK") != -1) {

						this->print("AT+SAPBR=2,1\r");
						_buffer = _readSerial();

						if (_buffer.indexOf("\"0.0.0.0\"") != -1 || _buffer.indexOf("ERR") != -1) {
							return false;
						}
						else {
							return true;
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
bool GSMSim::gprsConnectBearer(String apn) {
	this->print(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r"));
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {

		delay(100);
		this->print(F("AT+SAPBR=3,1,\"APN\",\""));
		this->print(apn);
		this->print(F("\"\r"));
		_buffer = _readSerial();

		if (_buffer.indexOf("OK") != -1) {
			delay(100);

			this->print(F("AT+SAPBR=3,1,\"USER\",\"\"\r"));
			_buffer = _readSerial();

			if (_buffer.indexOf("OK") != -1) {

				delay(100);

				this->print(F("AT+SAPBR=3,1,\"PWD\",\"\"\r"));
				_buffer = _readSerial();

				if (_buffer.indexOf("OK") != -1) {

					delay(100);
					this->print("AT+SAPBR=1,1\r");
					_buffer = _readSerial();
					delay(50);
					_buffer += _readSerial();
					if (_buffer.indexOf("OK") != -1) {

						this->print("AT+SAPBR=2,1\r");
						_buffer = _readSerial();

						if (_buffer.indexOf("\"0.0.0.0\"") != -1 || _buffer.indexOf("ERR") != -1) {
							return false;
						}
						else {
							return true;
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
bool GSMSim::gprsConnectBearer(String apn, String user, String password) {
	this->print(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r"));
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {

		delay(100);
		this->print(F("AT+SAPBR=3,1,\"APN\",\""));
		this->print(apn);
		this->print(F("\"\r"));
		_buffer = _readSerial();

		if (_buffer.indexOf("OK") != -1) {
			delay(100);

			this->print(F("AT+SAPBR=3,1,\"USER\",\""));
			this->print(user);
			this->print(F("\"\r"));
			_buffer = _readSerial();

			if (_buffer.indexOf("OK") != -1) {

				delay(100);

				this->print(F("AT+SAPBR=3,1,\"PWD\",\""));
				this->print(password);
				this->print(F("\"\r"));
				_buffer = _readSerial();

				if (_buffer.indexOf("OK") != -1) {

					delay(100);
					this->print("AT+SAPBR=1,1\r");
					_buffer = _readSerial();
					delay(50);
					_buffer += _readSerial();
					if (_buffer.indexOf("OK") != -1) {

						this->print("AT+SAPBR=2,1\r");
						_buffer = _readSerial();

						if (_buffer.indexOf("\"0.0.0.0\"") != -1 || _buffer.indexOf("ERR") != -1) {
							return false;
						}
						else {
							return true;
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
bool GSMSim::gprsIsConnected() {
	this->print(F("AT+SAPBR=2,1\r"));
	_buffer = _readSerial();
	delay(50);
	_buffer += _readSerial();

	if (_buffer.indexOf("ERR") != -1 || _buffer.indexOf("\"0.0.0.0\"") != -1) {
		return false;
	}
	else {
		return true;
	}
}
// GET IP Address
String GSMSim::gprsGetIP() {
	this->print(F("AT+SAPBR=2,1\r\n"));
	_buffer = _readSerial();
	delay(50);
	_buffer += _readSerial();

	//return _buffer;

	if (_buffer.indexOf("ERR") != -1 || _buffer.indexOf("\"0.0.0.0\"") != -1) {
		return "ERROR:NO_IP";
	}
	else {
		if (_buffer.indexOf("+SAPBR:") != -1) {
			String veri = _buffer.substring(_buffer.indexOf(",\"")+2, _buffer.lastIndexOf("\""));
			veri.trim();
			return veri;
		}
		else {
			"ERROR:NO_IP_FETCH";
		}
	}
}

bool GSMSim::gprsCloseConn() {
	this->print(F("AT+SAPBR=0,1\r"));
	_buffer = _readSerial();
	delay(50);
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}

String GSMSim::gprsHTTPGet(String url) {
	if (gprsIsConnected()) {
		// Terminate http connection, if it opened before!
		this->print(F("AT+HTTPTERM\r"));
		_buffer = _readSerial();

		this->print(F("AT+HTTPINIT\r"));
		_buffer = _readSerial();
		if (_buffer.indexOf("OK") != -1) {
			this->print(F("AT+HTTPPARA=\"CID\",1\r"));
			_buffer = _readSerial();
			if (_buffer.indexOf("OK") != -1) {
				this->print(F("AT+HTTPPARA=\"URL\",\""));
				this->print(url);
				this->print("\"\r");
				_buffer = _readSerial();

				if (_buffer.indexOf("OK") != -1) {
					this->print(F("AT+HTTPACTION=0\r"));
					_buffer = _readSerial();
					if (_buffer.indexOf("OK") != -1) {
						delay(100);
						_buffer = _readSerial(10000);
						if (_buffer.indexOf("+HTTPACTION: 0,") != -1) {
							String kod = _buffer.substring(_buffer.indexOf(",")+1, _buffer.lastIndexOf(","));
							String uzunluk = _buffer.substring(_buffer.lastIndexOf(",")+1);

							String sonuc = "METHOD:GET|HTTPCODE:";
							sonuc += kod;
							sonuc += "|LENGTH:";
							sonuc += uzunluk;

							// Bağlantıyı kapat!
							this->print(F("AT+HTTPTERM\r"));
							_buffer = _readSerial();

							sonuc.trim();

							return sonuc;
						}
						else {
							return "HTTP_ACTION_READ_ERROR";
						}
					}
					else {
						return "HTTP_ACTION_ERROR";
					}
				}
				else {
					return "HTTP_PARAMETER_ERROR";
				}

			}
			else {
				return "HTTP_PARAMETER_ERROR";
			}
		}
		else {
			return "HTTP_INIT_ERROR";
		}
	}
	else {
		return "GPRS_NOT_CONNECTED";
	}
}
String GSMSim::gprsHTTPGet(String url, bool read) {
	if (gprsIsConnected()) {
		// Terminate http connection, if it opened before!
		this->print(F("AT+HTTPTERM\r"));
		_buffer = _readSerial();

		this->print(F("AT+HTTPINIT\r\n"));
		_buffer = _readSerial();

		//return _buffer;
		if (_buffer.indexOf("OK") != -1) {
			this->print(F("AT+HTTPPARA=\"CID\",1\r"));
			_buffer = _readSerial();
			if (_buffer.indexOf("OK") != -1) {
				this->print(F("AT+HTTPPARA=\"URL\",\""));
				this->print(url);
				this->print(F("\"\r"));
				_buffer = _readSerial();

				if (_buffer.indexOf("OK") != -1) {
					this->print(F("AT+HTTPACTION=0\r"));
					_buffer = _readSerial();
					if (_buffer.indexOf("OK") != -1) {
						delay(100);
						_buffer = _readSerial(10000);
						if (_buffer.indexOf("+HTTPACTION: 0,") != -1) {
							String kod = _buffer.substring(_buffer.indexOf(",") + 1, _buffer.lastIndexOf(","));
							String uzunluk = _buffer.substring(_buffer.lastIndexOf(",") + 1);
							kod.trim();
							uzunluk.trim();

							this->print(F("AT+HTTPREAD\r"));
							_buffer = _readSerial(10000);

							String okuma = "";

							if (_buffer.indexOf("+HTTPREAD:") != -1) {

								String kriter = "+HTTPREAD: " + uzunluk;
								String veri = _buffer.substring(_buffer.indexOf(kriter) + kriter.length(), _buffer.lastIndexOf("OK"));
								okuma = veri;
							}
							else {
								return "ERROR:HTTP_READ_ERROR";
							}

							String sonuc = "METHOD:GET|HTTPCODE:";
							sonuc += kod;
							sonuc += "|LENGTH:";
							sonuc += uzunluk;
							sonuc += "|DATA:";
							okuma.trim();
							sonuc += okuma;

							this->print(F("AT+HTTPTERM\r"));
							_buffer = _readSerial();

							sonuc.trim();

							return sonuc;
						}
						else {
							return "ERROR:HTTP_ACTION_READ_ERROR";
						}
					}
					else {
						return "ERROR:HTTP_ACTION_ERROR";
					}
				}
				else {
					return "ERROR:HTTP_PARAMETER_ERROR";
				}

			}
			else {
				return "ERROR:HTTP_PARAMETER_ERROR";
			}
		}
		else {
			return "ERROR:HTTP_INIT_ERROR";
		}
	}
	else {
		return "ERROR:GPRS_NOT_CONNECTED";
	}
}



//////////////////////////////////////
//			TIME METHODS			//
//////////////////////////////////////

bool GSMSim::timeSetServer(int timezone) {
	this->print("AT+CNTPCID=1\r");
	_buffer = _readSerial();

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

	this->print(F("AT+CNTP=\"202.120.2.101\","));
	this->print(zaman);
	this->print(F("\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}
bool GSMSim::timeSetServer(int timezone, String server) {
	this->print("AT+CNTPCID=1\r");
	_buffer = _readSerial();

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

	this->print(F("AT+CNTP=\""));
	this->print(server);
	this->print(F("\","));
	this->print(zaman);
	this->print(F("\r"));

	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}
String GSMSim::timeSyncFromServer() {

	this->print(F("AT+CNTP\r"));
	_buffer = _readSerial();
	//delay(50);
	_buffer = _readSerial(20000);


	if (_buffer.indexOf("+CNTP:") != -1) {
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

String GSMSim::timeGetRaw() {
	this->print("AT+CCLK?\r");
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		String zaman = _buffer.substring(_buffer.indexOf("\"") + 1, _buffer.lastIndexOf("\""));
		return zaman;
	}
	else {
		return "ERROR:NOT_GET_DATETIME";
	}
}
void GSMSim::timeGet(int *day, int *month, int *year, int *hour, int *minute, int *second) {
	this->print("AT+CCLK?\r");
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		_buffer = _buffer.substring(_buffer.indexOf("\"") + 1, _buffer.lastIndexOf("\"") - 1);
		*year = (_buffer.substring(0, 2).toInt()) + 2000;
		*month = _buffer.substring(3, 5).toInt();
		*day = _buffer.substring(6, 8).toInt();
		*hour = _buffer.substring(9, 11).toInt();
		*minute = _buffer.substring(12, 14).toInt();
		*second = _buffer.substring(15, 17).toInt();
	}
}


//////////////////////////////////////
//			EMAIL METHODS			//
//////////////////////////////////////

bool GSMSim::emailSMTPConf(String server, String port, bool useSSL) {
	int ssl = useSSL == true ? 1 : 0;

	this->print(F("AT+EMAILSSL="));
	this->print(ssl);
	this->print(F("\r"));
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {

		this->print(F("AT+EMAILCID=1\r"));
		_buffer = _readSerial();

		if (_buffer.indexOf("OK") != -1) {
			this->print(F("AT+EMAILTO=30\r"));
			_buffer = _readSerial();

			if (_buffer.indexOf("OK") != -1) {
				this->print(F("AT+SMTPSRV=\""));
				this->print(server);
				this->print(F("\",\""));
				this->print(port);
				this->print("\"\r");

				_buffer = _readSerial();

				if (_buffer.indexOf("OK") != -1) {
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
bool GSMSim::emailSMTPAuth(String username, String password) {
	this->print(F("AT+SMTPAUTH=1,\""));
	this->print(username);
	this->print(F("\",\""));
	this->print(password);
	this->print(F("\"\r"));

	_buffer = _readSerial();
	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}
bool GSMSim::emailSMTPAuth(String username, String password, bool requireAuth) {
	int auth = requireAuth == true ? 1 : 0;
	this->print(F("AT+SMTPAUTH="));
	this->print(auth);
	this->print(F(",\""));
	this->print(username);
	this->print(F("\",\""));
	this->print(password);
	this->print(F("\"\r"));

	_buffer = _readSerial();
	if (_buffer.indexOf("OK") != -1) {
		return true;
	}
	else {
		return false;
	}
}
bool GSMSim::emailSMTPGmail(String username, String password) {
	bool conf = emailSMTPConf("smtp.gmail.com", "465", true);
	if (conf) {
		return emailSMTPAuth(username, password);
	}
	else {
		return false;
	}
}
String GSMSim::emailSMTPWrite(String from, String to, String title, String message) {

	this->print(F("AT+SMTPFROM=\""));
	this->print(from);
	this->print("\"\r");
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {

		this->print("AT+SMTPRCPT=0\r");
		_buffer = _readSerial();
		delay(50);
		this->print(F("AT+SMTPRCPT=0,1,\""));
		this->print(to);
		this->print("\"\r");
		_buffer = _readSerial();

		if (_buffer.indexOf("OK") != -1) {

			this->print(F("AT+SMTPSUB=\""));
			this->print(title);
			this->print("\"\r");
			_buffer = _readSerial();

			if (_buffer.indexOf("OK") != -1) {

				uint8_t uzunluk = message.length();
				this->print(F("AT+SMTPBODY="));
				this->print(uzunluk);
				this->print("\r");

				delay(50);

				this->print(message);
				this->print(F("\""));
				_buffer += _readSerial();

				if (_buffer.indexOf("OK") != -1) {

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
String GSMSim::emailSMTPWrite(String from, String to, String title, String message, String fromName, String toName) {
	this->print(F("AT+SMTPFROM=\""));
	this->print(from);
	this->print("\",\"");
	this->print(fromName);
	this->print("\"\r");
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {

		this->print("AT+SMTPRCPT=0\r");
		_buffer = _readSerial();
		delay(50);
		this->print(F("AT+SMTPRCPT=0,1,\""));
		this->print(to);
		this->print("\",\"");
		this->print(toName);
		this->print("\"\r");
		_buffer = _readSerial();

		if (_buffer.indexOf("OK") != -1) {

			this->print(F("AT+SMTPSUB=\""));
			this->print(title);
			this->print("\"\r");
			_buffer = _readSerial();

			if (_buffer.indexOf("OK") != -1) {

				uint8_t uzunluk = message.length();
				this->print(F("AT+SMTPBODY="));
				this->print(uzunluk);
				this->print("\r");

				delay(50);

				this->print(message);
				this->print(F("\""));
				_buffer += _readSerial();

				if (_buffer.indexOf("OK") != -1) {

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

String GSMSim::emailSMTPSend() {

	this->print("AT+SMTPSEND\r");
	_buffer = _readSerial();

	if (_buffer.indexOf("OK") != -1) {
		delay(50);
		_buffer = _readSerial(30000);
		if (_buffer.indexOf("+SMTPSEND:") != -1) {
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
			_buffer = _readSerial(30000);

			if (_buffer.indexOf("+SMTPSEND:") != -1) {
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


//////////////////////////////////////
//			PRIVATE METHODS			//
//////////////////////////////////////

// READ FROM SERIAL
String GSMSim::_readSerial() {

	uint64_t timeOld = millis();

	while (!this->available() && !(millis() > timeOld + TIME_OUT_READ_SERIAL))
	{
		delay(13);
	}

	String str = "";

	while (this->available())
	{
		if (this->available())
		{
			str += (char) this->read();
		}
	}

	return str;
}

String GSMSim::_readSerial(uint32_t timeout) {

	uint64_t timeOld = millis();

	while (!this->available() && !(millis() > timeOld + timeout))
	{
		delay(13);
	}

	String str = "";

	while (this->available())
	{
		if (this->available())
		{
			str += (char) this->read();
		}
	}

	return str;
}
