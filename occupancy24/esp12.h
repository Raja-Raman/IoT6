#ifndef ESP12_H
#define ESP12_H

#include "common.h"
#include "config.h"
#include <ESP8266WiFi.h>        // http://arduino.esp8266.com/versions/2.4.1/package_esp8266com_index.json
#include <ESP8266HTTPClient.h>  // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient
#include <EEPROM.h>             // bundled with Arduino IDE 
#include <Timer.h>              // https://github.com/SandyWalsh/Timer

extern "C" {
  #include "user_interface.h"
  #include "wpa2_enterprise.h"
}

class Esp12 {
public:
    Esp12(Config conf);
    void init(); 
    void update();  
    bool sendStatus (byte status, byte destination);
    void disable();
    void enable();
    void reset_watchdog();
private:
    Config C;
    HTTPClient htclient;    
    char body[128];   // This is the maximum payload length ***
    char body2[128];
    int  body_length;      
    int  body_length2;     
    
    void connect_wifi();   
    void makePayload(byte status);      
    bool send2CloudUrl();
    bool send2TestUrl();      
    bool send2DashboardUrl();    
    bool send2AnalyticsUrl();
};

#endif
