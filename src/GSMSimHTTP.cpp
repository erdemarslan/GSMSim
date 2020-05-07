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
#include "GSMSimHTTP.h"
#include "GSMSimGPRS.h"

// HTTP Get Method - No return web server response
String GSMSimHTTP::get(String url) {

	if (isConnected()) {
		// Terminate http connection, if it opened before! Otherwise method not run correctly.
		gsm.print(F("AT+HTTPTERM\r"));
		_readSerial();

		gsm.print(F("AT+HTTPINIT\r"));
		_readSerial();
		if (_buffer.indexOf(F("OK")) != -1) {
			gsm.print(F("AT+HTTPPARA=\"CID\",1\r"));
			_readSerial();
			if (_buffer.indexOf(F("OK")) != -1) {
				gsm.print(F("AT+HTTPPARA=\"URL\",\""));
				gsm.print(url);
				gsm.print("\"\r");
				_readSerial();

				if (_buffer.indexOf(F("OK")) != -1) {
					gsm.print(F("AT+HTTPACTION=0\r"));
					_readSerial();
					if (_buffer.indexOf(F("OK")) != -1) {
						_readSerial(10000);
						if (_buffer.indexOf(F("+HTTPACTION: 0,")) != -1) {
							String kod = _buffer.substring(_buffer.indexOf(F(","))+1, _buffer.lastIndexOf(F(",")));
							String uzunluk = _buffer.substring(_buffer.lastIndexOf(F(","))+1);

							String sonuc = "METHOD:GET|HTTPCODE:";
							sonuc += kod;
							sonuc += "|LENGTH:";
							sonuc += uzunluk;

							// Bağlantıyı kapat!
							gsm.print(F("AT+HTTPTERM\r"));
							_readSerial();

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
// HTTP Get Method - Return web server response
String GSMSimHTTP::get(String url, bool read) {
	if(read) {
		if (isConnected()) {
			// Terminate http connection, if it opened before!
			gsm.print(F("AT+HTTPTERM\r"));
			_readSerial();

			gsm.print(F("AT+HTTPINIT\r\n"));
			_readSerial();
			if (_buffer.indexOf(F("OK")) != -1) {
				gsm.print(F("AT+HTTPPARA=\"CID\",1\r"));
				_readSerial();
				if (_buffer.indexOf(F("OK")) != -1) {
					gsm.print(F("AT+HTTPPARA=\"URL\",\""));
					gsm.print(url);
					gsm.print(F("\"\r"));
					_readSerial();
					if (_buffer.indexOf(F("OK")) != -1) {
						gsm.print(F("AT+HTTPACTION=0\r"));
						_readSerial();
						if (_buffer.indexOf(F("OK")) != -1) {
							_readSerial(10000);
							if (_buffer.indexOf(F("+HTTPACTION: 0,")) != -1) {
								String kod = _buffer.substring(_buffer.indexOf(F(",")) + 1, _buffer.lastIndexOf(F(",")));
								String uzunluk = _buffer.substring(_buffer.lastIndexOf(F(",")) + 1);
								kod.trim();
								uzunluk.trim();

								gsm.print(F("AT+HTTPREAD\r"));
								_readSerial(10000);

								String okuma = "";

								if (_buffer.indexOf(F("+HTTPREAD:")) != -1) {
									String kriter = "+HTTPREAD: " + uzunluk;
									String veri = _buffer.substring(_buffer.indexOf(kriter) + kriter.length(), _buffer.lastIndexOf(F("OK")));
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

								gsm.print(F("AT+HTTPTERM\r"));
								_readSerial();

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
	} else {
		get(url);
	}	
}
// HTTP Get Method with SSL - No return web server response
String GSMSimHTTP::getWithSSL(String url) {
	if (isConnected()) {
		// Terminate http connection, if it opened before! Otherwise method not run correctly.
		gsm.print(F("AT+HTTPTERM\r"));
		_readSerial();

		gsm.print(F("AT+HTTPINIT\r"));
		_readSerial();
		if (_buffer.indexOf(F("OK")) != -1) {
			gsm.print(F("AT+HTTPPARA=\"CID\",1\r"));
			_readSerial();
			if (_buffer.indexOf(F("OK")) != -1) {
				gsm.print(F("AT+HTTPPARA=\"URL\",\""));
				gsm.print(url);
				gsm.print("\"\r");
				_readSerial();
				if (_buffer.indexOf(F("OK")) != -1) {
					gsm.print(F("AT+HTTPPARA=\"REDIR\",1\r"));
					_readSerial();
					if (_buffer.indexOf(F("OK")) != -1) {
						gsm.print(F("AT+HTTPSSL=1\r"));
						_readSerial();
						if (_buffer.indexOf(F("OK")) != -1) {
							gsm.print(F("AT+HTTPACTION=0\r"));
							_readSerial();
							if (_buffer.indexOf(F("OK")) != -1) {
								_readSerial(30000);
								if (_buffer.indexOf(F("+HTTPACTION: 0,")) != -1) {
									String kod = _buffer.substring(_buffer.indexOf(F(","))+1, _buffer.lastIndexOf(F(",")));
									String uzunluk = _buffer.substring(_buffer.lastIndexOf(F(","))+1);

									String sonuc = "METHOD:GET|HTTPCODE:";
									sonuc += kod;
									sonuc += "|LENGTH:";
									sonuc += uzunluk;

									// Bağlantıyı kapat!
									gsm.print(F("AT+HTTPTERM\r"));
									_readSerial();

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
						} else {
							return "HTTP_SSL_ERROR";
						}
					} else {
						return "HTTP_PARAMETER_ERROR";
					}

					//
					/*
					
					*/
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
// HTTP Get Method with SSL - Return web server response
String GSMSimHTTP::getWithSSL(String url, bool read) {
	if(read) {
		if (isConnected()) {
			// Terminate http connection, if it opened before!
			gsm.print(F("AT+HTTPTERM\r"));
			_readSerial();

			gsm.print(F("AT+HTTPINIT\r\n"));
			_readSerial();
			if (_buffer.indexOf(F("OK")) != -1) {
				gsm.print(F("AT+HTTPPARA=\"CID\",1\r"));
				_readSerial();
				if (_buffer.indexOf(F("OK")) != -1) {
					gsm.print(F("AT+HTTPPARA=\"URL\",\""));
					gsm.print(url);
					gsm.print(F("\"\r"));
					_readSerial();
					if (_buffer.indexOf(F("OK")) != -1) {
						gsm.print(F("AT+HTTPPARA=\"REDIR\",1\r"));
						_readSerial();
						if (_buffer.indexOf(F("OK")) != -1) {
							gsm.print(F("AT+HTTPSSL=1\r"));
							_readSerial();
							if (_buffer.indexOf(F("OK")) != -1) {
								gsm.print(F("AT+HTTPACTION=0\r"));
								_readSerial();
								if (_buffer.indexOf(F("OK")) != -1) {
									_readSerial(30000);
									if (_buffer.indexOf(F("+HTTPACTION: 0,")) != -1) {
										String kod = _buffer.substring(_buffer.indexOf(F(",")) + 1, _buffer.lastIndexOf(F(",")));
										String uzunluk = _buffer.substring(_buffer.lastIndexOf(F(",")) + 1);
										kod.trim();
										uzunluk.trim();

										gsm.print(F("AT+HTTPREAD\r"));
										_readSerial(30000);

										String okuma = "";

										if (_buffer.indexOf(F("+HTTPREAD:")) != -1) {
											String kriter = "+HTTPREAD: " + uzunluk;
											String veri = _buffer.substring(_buffer.indexOf(kriter) + kriter.length(), _buffer.lastIndexOf(F("OK")));
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

										gsm.print(F("AT+HTTPTERM\r"));
										_readSerial();

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
							} else {
								return "ERROR:HTTP_SSL_ERROR";
							}
						} else {
							return "ERROR:HTTP_PARAMETER_ERROR";
						}
						/*
						
						*/
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
	} else {
		getWithSSL(url);
	}
}

// HTTP POST Method - No return web server response
String GSMSimHTTP::post(String url, String data, String contentType) {
	// önce internet bağlı olsun...
	if(isConnected()) {
		// Terminate http connection, if it opened before!
		gsm.print(F("AT+HTTPTERM\r"));
		_readSerial();

		// şimdi başlıyoruz...
		gsm.print(F("AT+HTTPINIT\r\n"));
		_readSerial();
		if (_buffer.indexOf(F("OK")) != -1) {
			// init tamam şimdi parametreleri girelim...
			gsm.print(F("AT+HTTPPARA=\"CID\",1\r"));
			_readSerial();
			if (_buffer.indexOf(F("OK")) != -1) {
				gsm.print(F("AT+HTTPPARA=\"URL\",\""));
				gsm.print(url);
				gsm.print(F("\"\r"));
				_readSerial();
				if (_buffer.indexOf(F("OK")) != -1) {
					gsm.print(F("AT+HTTPPARA=\"CONTENT\",\""));
					gsm.print(contentType);
					gsm.print(F("\"\r"));
					_readSerial();
					if (_buffer.indexOf(F("OK")) != -1) {
						// download modunu açalım
						// veri uzunluğunu bulalım
						unsigned int length = data.length();
						gsm.print(F("AT+HTTPDATA="));
						gsm.print(length);
						gsm.print(F(","));
						gsm.print(30000);
						gsm.print("\r");
						_readSerial(30000);
						if (_buffer.indexOf(F("DOWNLOAD")) != -1) {
							gsm.print(data);
							gsm.print(F("\r"));
							_readSerial(30000);
							if (_buffer.indexOf(F("OK")) != -1) {
								gsm.print(F("AT+HTTPACTION=1\r"));
								_readSerial();
								if (_buffer.indexOf(F("OK")) != -1) {
									_readSerial(3000);
									if (_buffer.indexOf(F("+HTTPACTION: 1,")) != -1) {
										String kod = _buffer.substring(_buffer.indexOf(F(","))+1, _buffer.lastIndexOf(F(",")));
										String uzunluk = _buffer.substring(_buffer.lastIndexOf(F(","))+1);

										String sonuc = "METHOD:POST|HTTPCODE:";
										sonuc += kod;
										sonuc += "|LENGTH:";
										sonuc += uzunluk;

										// Bağlantıyı kapat!
										gsm.print(F("AT+HTTPTERM\r"));
										_readSerial();

										sonuc.trim();

										return sonuc;
									}
									else {
										return "HTTP_ACTION_READ_ERROR";
									}
								} else {
									return "ERROR:HTTP_ACTION_ERROR";
								}
							} else {
								return "ERROR:HTTP_DATADOWNLOAD_ERROR";
							}
						} else {
							return "ERROR:HTTP_DATA_ERROR";
						}
					}else {
						return "ERROR:HTTP_PARAMETER_ERROR";
					}
				} else {
					return "ERROR:HTTP_PARAMETER_ERROR";
				}
			} else {
				return "ERROR:HTTP_PARAMETER_ERROR";
			}
		} else {
			return "ERROR:HTTP_INIT_ERROR";
		}
	} else {
		return "ERROR:GPRS_NOT_CONNECTED";
	}
}

// HTTP POST Method - Return web server response
String GSMSimHTTP::post(String url, String data, String contentType, bool read) {
	// okuma istenmiyorsa bir önceki fonksiyonu çağırsın...
	if(read) {
		// önce internet bağlı olsun...
		if(isConnected()) {
			// Terminate http connection, if it opened before!
			gsm.print(F("AT+HTTPTERM\r"));
			_readSerial();

			// şimdi başlıyoruz...
			gsm.print(F("AT+HTTPINIT\r\n"));
			_readSerial();
			if (_buffer.indexOf(F("OK")) != -1) {
				// init tamam şimdi parametreleri girelim...
				gsm.print(F("AT+HTTPPARA=\"CID\",1\r"));
				_readSerial();
				if (_buffer.indexOf(F("OK")) != -1) {
					gsm.print(F("AT+HTTPPARA=\"URL\",\""));
					gsm.print(url);
					gsm.print(F("\"\r"));
					_readSerial();
					if (_buffer.indexOf(F("OK")) != -1) {
						gsm.print(F("AT+HTTPPARA=\"CONTENT\",\""));
						gsm.print(contentType);
						gsm.print(F("\"\r"));
						_readSerial();
						if (_buffer.indexOf(F("OK")) != -1) {
							// download modunu açalım
							// veri uzunluğunu bulalım
							unsigned int length = data.length();
							gsm.print(F("AT+HTTPDATA="));
							gsm.print(length);
							gsm.print(F(","));
							gsm.print(30000);
							gsm.print("\r");
							_readSerial(30000);
							if (_buffer.indexOf(F("DOWNLOAD")) != -1) {
								gsm.print(data);
								gsm.print(F("\r"));
								_readSerial(30000);
								if (_buffer.indexOf(F("OK")) != -1) {
									gsm.print(F("AT+HTTPACTION=1\r"));
									_readSerial();
									if (_buffer.indexOf(F("OK")) != -1) {
										_readSerial(3000);
										if (_buffer.indexOf(F("+HTTPACTION: 1,")) != -1) {
											String kod = _buffer.substring(_buffer.indexOf(F(","))+1, _buffer.lastIndexOf(F(",")));
											String uzunluk = _buffer.substring(_buffer.lastIndexOf(F(","))+1);
											kod.trim();
											uzunluk.trim();

											gsm.print(F("AT+HTTPREAD\r"));
											_readSerial(30000);

											String okuma = "";

											if (_buffer.indexOf(F("+HTTPREAD:")) != -1) {
												String kriter = "+HTTPREAD: " + uzunluk;
												String veri = _buffer.substring(_buffer.indexOf(kriter) + kriter.length(), _buffer.lastIndexOf(F("OK")));
												okuma = veri;
											}
											else {
												return "ERROR:HTTP_READ_ERROR";
											}

											String sonuc = "METHOD:POST|HTTPCODE:";
											sonuc += kod;
											sonuc += "|LENGTH:";
											sonuc += uzunluk;
											sonuc += "|DATA:";
											okuma.trim();
											sonuc += okuma;

											gsm.print(F("AT+HTTPTERM\r"));
											_readSerial();

											sonuc.trim();

											return sonuc;
										}
										else {
											return "HTTP_ACTION_READ_ERROR";
										}
									} else {
										return "ERROR:HTTP_ACTION_ERROR";
									}
								} else {
									return "ERROR:HTTP_DATADOWNLOAD_ERROR";
								}
							} else {
								return "ERROR:HTTP_DATA_ERROR";
							}
						}else {
							return "ERROR:HTTP_PARAMETER_ERROR";
						}
					} else {
						return "ERROR:HTTP_PARAMETER_ERROR";
					}
				} else {
					return "ERROR:HTTP_PARAMETER_ERROR";
				}
			} else {
				return "ERROR:HTTP_INIT_ERROR";
			}
		} else {
			return "ERROR:GPRS_NOT_CONNECTED";
		}
	} else {
		post(url, data, contentType);
	}
}

// HTTP POST Method with SSL - No return web server response
String GSMSimHTTP::postWithSSL(String url, String data, String contentType) {
	// önce internet bağlı olsun...
	if(isConnected()) {
		// Terminate http connection, if it opened before!
		gsm.print(F("AT+HTTPTERM\r"));
		_readSerial();

		// şimdi başlıyoruz...
		gsm.print(F("AT+HTTPINIT\r\n"));
		_readSerial();
		if (_buffer.indexOf(F("OK")) != -1) {
			// init tamam şimdi parametreleri girelim...
			gsm.print(F("AT+HTTPPARA=\"CID\",1\r"));
			_readSerial();
			if (_buffer.indexOf(F("OK")) != -1) {
				gsm.print(F("AT+HTTPPARA=\"URL\",\""));
				gsm.print(url);
				gsm.print(F("\"\r"));
				_readSerial();
				if (_buffer.indexOf(F("OK")) != -1) {
					gsm.print(F("AT+HTTPPARA=\"CONTENT\",\""));
					gsm.print(contentType);
					gsm.print(F("\"\r"));
					_readSerial();
					if (_buffer.indexOf(F("OK")) != -1) {
						gsm.print("AT+HTTPSSL=1\r");
						_readSerial();
						if(_buffer.indexOf(F("OK")) != -1) {
							// download modunu açalım
							// veri uzunluğunu bulalım
							unsigned int length = data.length();
							gsm.print(F("AT+HTTPDATA="));
							gsm.print(length);
							gsm.print(F(","));
							gsm.print(30000);
							gsm.print("\r");
							_readSerial(30000);
							if (_buffer.indexOf(F("DOWNLOAD")) != -1) {
								gsm.print(data);
								gsm.print(F("\r"));
								_readSerial(30000);
								if (_buffer.indexOf(F("OK")) != -1) {
									gsm.print(F("AT+HTTPACTION=1\r"));
									_readSerial();
									if (_buffer.indexOf(F("OK")) != -1) {
										_readSerial(3000);
										if (_buffer.indexOf(F("+HTTPACTION: 1,")) != -1) {
											String kod = _buffer.substring(_buffer.indexOf(F(","))+1, _buffer.lastIndexOf(F(",")));
											String uzunluk = _buffer.substring(_buffer.lastIndexOf(F(","))+1);

											String sonuc = "METHOD:POST|HTTPCODE:";
											sonuc += kod;
											sonuc += "|LENGTH:";
											sonuc += uzunluk;

											// Bağlantıyı kapat!
											gsm.print(F("AT+HTTPTERM\r"));
											_readSerial();

											sonuc.trim();

											return sonuc;
										}
										else {
											return "HTTP_ACTION_READ_ERROR";
										}
									} else {
										return "ERROR:HTTP_ACTION_ERROR";
									}
								} else {
									return "ERROR:HTTP_DATADOWNLOAD_ERROR";
								}
							} else {
								return "ERROR:HTTP_DATA_ERROR";
							}
						} else {
							return "ERROR:HTTP_SSL_ERROR";
						}
					}else {
						return "ERROR:HTTP_PARAMETER_ERROR";
					}
				} else {
					return "ERROR:HTTP_PARAMETER_ERROR";
				}
			} else {
				return "ERROR:HTTP_PARAMETER_ERROR";
			}
		} else {
			return "ERROR:HTTP_INIT_ERROR";
		}
	} else {
		return "ERROR:GPRS_NOT_CONNECTED";
	}
}

// HTTP POST Method with SSL - Return web server response
String GSMSimHTTP::postWithSSL(String url, String data, String contentType, bool read) {
	// okuma istenmiyorsa bir önceki fonksiyonu çağırsın...
	if(read) {
		// önce internet bağlı olsun...
		if(isConnected()) {
			// Terminate http connection, if it opened before!
			gsm.print(F("AT+HTTPTERM\r"));
			_readSerial();

			// şimdi başlıyoruz...
			gsm.print(F("AT+HTTPINIT\r\n"));
			_readSerial();
			if (_buffer.indexOf(F("OK")) != -1) {
				// init tamam şimdi parametreleri girelim...
				gsm.print(F("AT+HTTPPARA=\"CID\",1\r"));
				_readSerial();
				if (_buffer.indexOf(F("OK")) != -1) {
					gsm.print(F("AT+HTTPPARA=\"URL\",\""));
					gsm.print(url);
					gsm.print(F("\"\r"));
					_readSerial();
					if (_buffer.indexOf(F("OK")) != -1) {
						gsm.print(F("AT+HTTPPARA=\"CONTENT\",\""));
						gsm.print(contentType);
						gsm.print(F("\"\r"));
						_readSerial();
						if (_buffer.indexOf(F("OK")) != -1) {
							gsm.print("AT+HTTPSSL=1\r");
							_readSerial();
							if(_buffer.indexOf(F("OK")) != -1) {
								// download modunu açalım
								// veri uzunluğunu bulalım
								unsigned int length = data.length();
								gsm.print(F("AT+HTTPDATA="));
								gsm.print(length);
								gsm.print(F(","));
								gsm.print(30000);
								gsm.print("\r");
								_readSerial(30000);
								if (_buffer.indexOf(F("DOWNLOAD")) != -1) {
									gsm.print(data);
									gsm.print(F("\r"));
									_readSerial(30000);
									if (_buffer.indexOf(F("OK")) != -1) {
										gsm.print(F("AT+HTTPACTION=1\r"));
										_readSerial();
										if (_buffer.indexOf(F("OK")) != -1) {
											_readSerial(3000);
											if (_buffer.indexOf(F("+HTTPACTION: 1,")) != -1) {
												String kod = _buffer.substring(_buffer.indexOf(F(","))+1, _buffer.lastIndexOf(F(",")));
												String uzunluk = _buffer.substring(_buffer.lastIndexOf(F(","))+1);
												kod.trim();
												uzunluk.trim();

												gsm.print(F("AT+HTTPREAD\r"));
												_readSerial(30000);

												String okuma = "";

												if (_buffer.indexOf(F("+HTTPREAD:")) != -1) {
													String kriter = "+HTTPREAD: " + uzunluk;
													String veri = _buffer.substring(_buffer.indexOf(kriter) + kriter.length(), _buffer.lastIndexOf(F("OK")));
													okuma = veri;
												}
												else {
													return "ERROR:HTTP_READ_ERROR";
												}

												String sonuc = "METHOD:POST|HTTPCODE:";
												sonuc += kod;
												sonuc += "|LENGTH:";
												sonuc += uzunluk;
												sonuc += "|DATA:";
												okuma.trim();
												sonuc += okuma;

												gsm.print(F("AT+HTTPTERM\r"));
												_readSerial();

												sonuc.trim();

												return sonuc;
											}
											else {
												return "HTTP_ACTION_READ_ERROR";
											}
										} else {
											return "ERROR:HTTP_ACTION_ERROR";
										}
									} else {
										return "ERROR:HTTP_DATADOWNLOAD_ERROR";
									}
								} else {
									return "ERROR:HTTP_DATA_ERROR";
								}
							} else {
								return "ERROR:HTTP_SSL_ERROR";
							}
						}else {
							return "ERROR:HTTP_PARAMETER_ERROR";
						}
					} else {
						return "ERROR:HTTP_PARAMETER_ERROR";
					}
				} else {
					return "ERROR:HTTP_PARAMETER_ERROR";
				}
			} else {
				return "ERROR:HTTP_INIT_ERROR";
			}
		} else {
			return "ERROR:GPRS_NOT_CONNECTED";
		}
	} else {
		postWithSSL(url, data, contentType);
	}
}

// Ping to any server. It returns raw value. It still experiment.
String GSMSimHTTP::ping(String address) {
	if(isConnected()) {
		// shutdown conn if opened.
		gsm.print(F("AT+CIPSHUT\r"));
		_readSerial();

		gsm.print(F("AT+CGATT?\r"));
		_readSerial();
		if(_buffer.indexOf(F("+CGATT: 1")) != -1) {
			// start task and set apn
			gsm.print(F("AT+CSTT=\""));
			gsm.print(APN);
			gsm.print(F("\",\""));
			gsm.print(USER);
			gsm.print(F("\",\""));
			gsm.print(PWD);
			gsm.print(F("\"\r"));
			_readSerial();
			if(_buffer.indexOf(F("OK")) != -1) {
				// bring up wireless connection
				gsm.print(F("AT+CIICR\r"));
				_readSerial();
				if(_buffer.indexOf(F("OK")) != -1) {
					// get local ip address
					gsm.print(F("AT+CIFSR\r"));
					_readSerial();
					if(_buffer.indexOf(F(".")) != -1) {
						// now we ping the server...
						gsm.print(F("AT+CIPPING=\""));
						gsm.print(address);
						gsm.print(F("\"\r"));
						//_readSerial();
						_readSerial(30000);

						return _buffer;
					} else {
						return "ERROR:GPRS_LOCAL_IP";
					}
				} else {
					return "ERROR:GPRS_WIRELESS_CONN";
				}
			} else {
				return "ERROR:GPRS_NOT_SET_APN";
			}
		} else {
			return "ERROR:GPRS_NOT_ATTACH";
		}
	} else {
		return "ERROR:GPRS_NOT_CONNECTED";
	}
}
