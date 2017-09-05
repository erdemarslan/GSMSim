/*
	GSMSim Library

	This library written for SIMCOM Sim800L module. Library may worked on any SIMCOM modules
	and GSM Shields.

	Created 11.05.2017
	By Erdem ARSLAN
	Modified 25.08.2017

	Erdem ARSLAN
	Science and Technology Teacher, an Arduino Lover =)
	erdemsaid@gmail.com
	https://www.erdemarslan.com/
	http://www.robothane.net/
	http://www.eralabs.net/

*/


#ifndef GSMSim_h
#define GSMSim_h
#include <SoftwareSerial.h>
#include "Arduino.h"

// DEFAULT PIN DEFINATIONS IN HERE
#define DEFAULT_RX_PIN 7
#define DEFAULT_TX_PIN 8
#define DEFAULT_RST_PIN 2

#define DEFAULT_LED_FLAG true
#define DEFAULT_LED_PIN 13

#define DEFAULT_BAUD_RATE 9600
#define BUFFER_RESERVE_MEMORY	255
#define TIME_OUT_READ_SERIAL	5000


// PIN (PERSONAL IDENTIFICATION NUMBER) STATUS
#define PIN_READY 0
#define SIM_PIN 1
#define SIM_PUK 2
#define PH_SIM_PIN 3
#define PH_SIM_PUK 4
#define SIM_PIN2 5
#define SIM_PUK2 6
#define PIN_STATUS_UNKNOWN 7

// CALL STATUS
#define READY 0
#define UNKNOWN 2
#define RINGING 3
#define IN_CALL 4
#define NOT_READ 99


// CLASS BEGIN IN HERE
class GSMSim : public SoftwareSerial
{
	private:
		uint32_t _baud;
		uint8_t _timeout;
		String _buffer;
		String _readSerial();
		String _readSerial(uint32_t timeout);

	public:

		uint8_t	RX_PIN;
		uint8_t TX_PIN;
		uint8_t RESET_PIN;
		uint8_t LED_PIN;
		bool	LED_FLAG;
		uint32_t BAUDRATE;


		GSMSim(void);
		GSMSim(uint8_t rx, uint8_t tx);
		GSMSim(uint8_t rx, uint8_t tx, uint8_t rst);
		GSMSim(uint8_t rx, uint8_t tx, uint8_t rst, uint8_t led);
		GSMSim(uint8_t rx, uint8_t tx, uint8_t rst, uint8_t led, bool ledflag);

		// Başlatıcı Fonksiyon
		void start();
		void start(uint32_t baud);
		// Reset Fonksiyonu
		void reset();

		// Kontrol Metotları

		// Telefon Fonksiyonunu ayarlar
		bool setPhoneFunc(uint8_t level);
		// Sinyal kalitesi
		uint8_t signalQuality();
		// Operatöre bağlı mı?
		bool isRegistered();
		// Sim kart takılımı
		bool isSimInserted();
		// pin durumu
		uint8_t pinStatus();
		// operatör adı
		String operatorName();
		// sim karttaki operatör adı
		String operatorNameFromSim();
		// telefon durumu
		uint8_t phoneStatus();
		// echo kapalı
		bool echoOff();
		// echo açıkı
		bool echoOn();
		// modül üreticisi
		String moduleManufacturer();
		// modül modeli
		String moduleModel();
		// modül revizyon
		String moduleRevision();
		// modül imei
		String moduleIMEI();
		// modül imei değiştirme
		bool moduleIMEIChange(char* imeino);
		// modül sim no
		String moduleIMSI();
		// modül sim operatör no
		String moduleICCID();
		// zil volümü
		uint8_t ringerVolume();
		// zil seviyesini ayarlar
		bool setRingerVolume(uint8_t level);
		// speaker düzeyi
		uint8_t speakerVolume();
		// speaker düzeyini ayarla
		bool setSpeakerVolume(uint8_t level);
		// debug modu - verbose mode
		String moduleDebug();





		// Arama Fonksiyonları
		// arama yapar
		bool call(char* phone_number);
		// arama cevaplar
		bool callAnswer();
		// aramayı sonlandırır
		bool callHangoff();
		// arama durumu
		uint8_t callStatus();
		// COLP u aktif veya pasif yapar
		bool callSetCOLP(bool active);
		// COLP aktif mi?
		bool callIsCOLPActive();
		// Arayanı söyleme aktif mi değil mi?
		bool callActivateListCurrent(bool active);
		// şimdi arayanı söyle
		String callReadCurrentCall(String serialRaw);


		// SMS Fonksiyonları
		// sms i text yada pdu moda döndürür
		bool smsTextMode(bool textModeON);
		// sms gönderir
		bool smsSend(char* number, char* message);
		// okunmamış mesaj listesi
		String smsListUnread();
		// indexi verilen mesajı oku
		String smsRead(uint8_t index);
		// indexi verilen mesajı oku
		String smsRead(uint8_t index, bool markRead);
		// serialden direk mesajı oku -> serialden gelen veri verilmeli
		String smsReadFromSerial(String serialRaw);
		// serialden gelen sms bilgisinin indexini ver
		uint8_t smsIndexFromSerial(String serialRaw);
		// mesaj merkezini öğren
		String smsReadMessageCenter();
		// mesaj merkezini değiştir
		bool smsChangeMessageCenter(char* messageCenter);
		// mesajı sil
		bool smsDeleteOne(uint8_t index);
		// tüm okunmuşları sil
		bool smsDeleteAllRead();
		// tüm mesajları sil
		bool smsDeleteAll();


		// DTMF Fonksiyonları
		// DTMF yi ayarlar
		bool dtmfSet(bool active, uint8_t interval, bool reportTime, bool soundDetect);
		// DTMF yi serialden okur!
		String dtmfRead(String serialRaw);


		// USSD Kodları
		// USSD kodu gönderir
		bool ussdSend(char* code);
		// Raw datadan cevabı okur!
		String ussdRead(String serialRaw);

		// Radyo Kodları
		bool fmOpen();
		bool fmOpen(bool mainChannel);
		bool fmOpen(bool mainChannel, uint8_t freq);
		bool fmIsOpened();
		bool fmClose();
		uint8_t fmGetFreq();
		bool fmSetFreq(uint8_t freq);
		uint8_t fmGetVolume();
		bool fmSetVolume(uint8_t volume);

		// GPRS Kodları
		// Connect to Bearer
		bool gprsConnectBearer();
		//String gprsConnectBearerT();
		bool gprsConnectBearer(String apn);
		bool gprsConnectBearer(String apn, String user, String password);

		// Check connection
		bool gprsIsConnected();
		// get ip address
		String gprsGetIP();
		// close gprs bearer connection
		bool gprsCloseConn();
		String gprsHTTPGet(String url);
		String gprsHTTPGet(String url, bool read);


		// NTP Komutları
		bool timeSetServer(int timezone);
		bool timeSetServer(int timezone, String server);
		String timeSyncFromServer();
		String timeGetRaw();
		void timeGet(int *day, int *month, int *year, int *hour, int *minute, int *second);

		// Email Komutları
		bool emailSMTPConf(String server, String port, bool useSSL);
		bool emailSMTPAuth(String username, String password);
		bool emailSMTPAuth(String username, String password, bool requireAuth);
		bool emailSMTPGmail(String username, String password);
		String emailSMTPWrite(String from, String to, String title, String message);
		String emailSMTPWrite(String from, String to, String title, String message, String fromName, String toName);
		String emailSMTPSend();

};

#endif
