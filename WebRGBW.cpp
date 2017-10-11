#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "WirelessRGBW.h"

#include "WebRGBW.h"


//------------------------------------------
//simple function that convert an hexadecimal char to byte
byte WebRGBW::AsciiToHex(char c) {
  return (c < 0x3A) ? (c - 0x30) : (c > 0x60 ? c - 0x57 : c - 0x37);
}

//------------------------------------------
//return RGBW Color
void WebRGBW::SetColor(byte* colorCode) {

  for (byte i = 0; i < _numberOfLeds; i++) _leds[i]->set(colorCode[i]);
}
//------------------------------------------
//return RGBW Status
String WebRGBW::GetColor(bool json = false) {

  String color('#');

  for (byte i = 0; i < _numberOfLeds; i++)  {
    String hex(_leds[i]->get(), HEX);
    if (hex.length() == 1) hex = '0' + hex;
    color += hex;
  }

  if (json) color = String(F("{\"color\":\"")) + color + F("\"}");

  return color;
}

//------------------------------------------
//return RGBW Status
String WebRGBW::GetStatus() {
  return GetColor(true);
}

//------------------------------------------
//Function to initiate RGBW with configured model
void WebRGBW::Init(byte model) {

  analogWriteRange(255);

  switch (model) {
    case 1:
      _leds[0] = new FadeLed(5);
      _leds[1] = new FadeLed(14);
      _leds[2] = new FadeLed(12);
      _leds[3] = new FadeLed(13);
      _leds[0]->setGammaTable(gamma8, 255);
      _leds[1]->setGammaTable(gamma8, 255);
      _leds[2]->setGammaTable(gamma8, 255);
      _leds[3]->setGammaTable(gamma8, 255);
      FadeLed::setInterval(20);
      _leds[0]->setTime(1000, true);
      _leds[1]->setTime(1000, true);
      _leds[2]->setTime(1000, true);
      _leds[3]->setTime(1000, true);
      _leds[0]->begin(0);
      _leds[1]->begin(0);
      _leds[2]->begin(0);
      _leds[3]->begin(0);
      _numberOfLeds = 4;
      break;

    default:
      _leds[0] = new FadeLed(14);
      _leds[1] = new FadeLed(5);
      _leds[2] = new FadeLed(12);
      _leds[0]->setGammaTable(gamma8, 255);
      _leds[1]->setGammaTable(gamma8, 255);
      _leds[2]->setGammaTable(gamma8, 255);
      FadeLed::setInterval(20);
      _leds[0]->setTime(1000, true);
      _leds[1]->setTime(1000, true);
      _leds[2]->setTime(1000, true);
      _leds[0]->begin(0);
      _leds[1]->begin(0);
      _leds[2]->begin(0);
      _numberOfLeds = 3;
      break;
  }

  Run();

  //BIG FADE to check LEDS
  for (byte i = 0; i < _numberOfLeds; i++) {
    _leds[i]->on();
    while (!_leds[i]->done()) {
      yield(); //let ESP live and no WDT
      delay(5);
      Run();
    }
    _leds[i]->off();
    while (!_leds[i]->done()) {
      yield(); //let ESP live and no WDT
      delay(5);
      Run();
    }
  }

  _initialized = true;
}

void WebRGBW::InitWebServer(AsyncWebServer &server) {

  server.on("/setColor", HTTP_GET, [this](AsyncWebServerRequest * request) {

    //check WebRGBW is initialized
    if (!_initialized) {
      request->send(400, F("text/html"), F("RGBW not Initialized"));
      return;
    }

    //try to find color code
    if (!request->hasParam(F("color"))) {
      request->send(400, F("text/html"), F("Missing color"));
      return;
    }

    //check length of colorcode
    if (request->getParam(F("color"))->value().length() != (_numberOfLeds * 2)) {
      request->send(400, F("text/html"), F("Incorrect color size"));
      return;
    }

    const char * colorCodeA = request->getParam(F("color"))->value().c_str();
    //check that color code is hexa
    for (byte i = 0; i < strlen(colorCodeA); i++) {
      if (!isHexadecimalDigit(colorCodeA[i])) {
        request->send(400, F("text/html"), F("Incorrect color code"));
        return;
      }
    }

    //convert text to byte
    byte colorCode[4];
    for (byte i = 0; i < _numberOfLeds; i++) {
      colorCode[i] = AsciiToHex(colorCodeA[i * 2]) * 0x10 + AsciiToHex(colorCodeA[i * 2 + 1]);
    }

    SetColor(colorCode);
    request->send(200);
  });

  server.on("/getColor", HTTP_GET, [this](AsyncWebServerRequest * request) {
    request->send(200, F("text/html"), GetColor());
  });

  server.on("/getJSONColor", HTTP_GET, [this](AsyncWebServerRequest * request) {
    request->send(200, F("text/html"), GetColor(true));
  });

  server.on("/gs1", HTTP_GET, [this](AsyncWebServerRequest * request) {
    request->send(200, F("text/json"), GetStatus());
  });
}

void WebRGBW::Run() {
  FadeLed::update();
}

