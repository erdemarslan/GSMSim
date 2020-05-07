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

#ifndef __GSMSimHTTP_H__
#define __GSMSimHTTP_H__

#include <Arduino.h>
#include "GSMSimGPRS.h"


class GSMSimHTTP : public GSMSimGPRS {
  protected :


  public :

    // Sınıfı Başlatıcı...
    GSMSimHTTP(Stream& s) : GSMSimGPRS(s) {
      RESET_PIN = DEFAULT_RST_PIN;
      LED_PIN = DEFAULT_LED_PIN;
      LED_FLAG = DEFAULT_LED_FLAG;

      APN = DEFAULT_GPRS_APN;
      USER = DEFAULT_GPRS_USER;
      PWD = DEFAULT_GPRS_PWD;
    }

    GSMSimHTTP(Stream& s, unsigned int resetPin) : GSMSimGPRS(s, resetPin) {
      RESET_PIN = resetPin;
      LED_PIN = DEFAULT_LED_PIN;
      LED_FLAG = DEFAULT_LED_FLAG;

      APN = DEFAULT_GPRS_APN;
      USER = DEFAULT_GPRS_USER;
      PWD = DEFAULT_GPRS_PWD;
    }

    GSMSimHTTP(Stream& s, unsigned int resetPin, unsigned int ledPin, bool ledFlag) : GSMSimGPRS(s, resetPin, ledPin, ledFlag) {
      RESET_PIN = resetPin;
      LED_PIN = ledPin;
      LED_FLAG = ledFlag;

      APN = DEFAULT_GPRS_APN;
      USER = DEFAULT_GPRS_USER;
      PWD = DEFAULT_GPRS_PWD;
    }

    // HTTP Get
    String get(String url);
    // HTTP Get
    String get(String url, bool read);
    // HTTPS Get
    String getWithSSL(String url);
    // HTTPS Get
    String getWithSSL(String url, bool read);
    // HTTP Post
    String post(String url, String data, String contentType);
    // HTTP Post
    String post(String url, String data, String contentType, bool read);
    // HTTPS Post
    String postWithSSL(String url, String data, String contentType);
    // HTTPS Post
    String postWithSSL(String url, String data, String contentType, bool read);

    // PING Komutları
    String ping(String address);

};

#endif
