#ifndef WirelessRGBW_h
#define WirelessRGBW_h


//J6B Informations
//------------Compile for 1M 64K SPIFFS------------
//------------/!\/!\/!\------Use Flash DOUT!!!------/!\/!\/!\------------
//Configuration Web Pages
//http://IP/fw
//http://IP/config
//http://IP/status
//RGBW Request Web Pages
//http://IP/setColor?color=RRGGBB
//http://IP/setColor?color=RRGGBBWW
//http://IP/getColor
//http://IP/getJSONColor


#define VERSION_NUMBER "3.0"

//Enable developper mode (fwdev webpage and SPIFFS is used)
#define DEVELOPPER_MODE 0

//For ARILUX AL-LC01, Pins used are fixed
//GPIO 14 = Red Leds
//GPIO 5 = Green Leds
//GPIO 12 = Blue Leds

//For ARILUX AL-LC02, Pins used are fixed
//GPIO 5 = Red Leds
//GPIO 14 = Green Leds
//GPIO 12 = Blue Leds
//GPIO 13 = White Leds


//Choose Serial Speed
#define SERIAL_SPEED 115200

//No Real Rescue because no Button
//Choose Pin used to boot in Rescue Mode
#define RESCUE_BTN_PIN 4


//construct Version text
#if DEVELOPPER_MODE
#define VERSION VERSION_NUMBER "-DEV"
#else
#define VERSION VERSION_NUMBER
#endif

#endif


