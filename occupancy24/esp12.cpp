// esp12.cpp
// See config.h for the cloud URLs

#include "esp12.h"
 
Esp12::Esp12(Config conf) 
{
    //SERIAL_PRINTLN ("ESP12 initializing..");
    this->C = conf;
}

void Esp12::init() {
    strcpy(body, C.payload1);
    strcat(body, C.room_id);
    strcat(body, C.payload2);
    body_length = strlen(body);    

    strcpy(body2, C.payload3);
    body_length2 = strlen(body2); 
    
    SERIAL_PRINT("Room id: ");
    SERIAL_PRINTLN(C.room_id);
    SERIAL_PRINT("cloud URL: ");
    SERIAL_PRINTLN(C.prod_url);
    SERIAL_PRINT("test URL: ");
    SERIAL_PRINTLN(C.test_url);    
    SERIAL_PRINT("dashboard URL: ");
    SERIAL_PRINTLN(C.dashboard_url);       
    SERIAL_PRINT("analytics URL: ");
    SERIAL_PRINTLN(C.analytics_url);     
    SERIAL_PRINT ("Payload 1: "); 
    SERIAL_PRINTLN (body);        
    SERIAL_PRINT ("Payload 1 length: "); 
    SERIAL_PRINTLN (body_length);      
    SERIAL_PRINT ("Payload 2: "); 
    SERIAL_PRINTLN (body2);        
    SERIAL_PRINT ("Payload 2 length: "); 
    SERIAL_PRINTLN (body_length2);  
    
    connect_wifi();
}

void Esp12::connect_wifi() {
    SERIAL_PRINTLN("\nConnecting to hot spot: ");
    SERIAL_PRINTLN(C.ssid);  
    
    WiFi.begin(C.ssid, C.password);
    // Wait for connection AND IP address from DHCP
    byte MAX_ATTEMPTS = 25;
    byte attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < MAX_ATTEMPTS) {        
        delay(500);
        if (C.verbose_mode) SERIAL_PRINT (".");
        attempts++;
    }
    SERIAL_PRINTLN ("!");
    delay(500);  // time to recover ?
    if (WiFi.status() == WL_CONNECTED) {
        SERIAL_PRINTLN ("WiFi connected.");  
        SERIAL_PRINTLN ("IP address: ");
        SERIAL_PRINTLN (WiFi.localIP()); 
    }  
    else
        SERIAL_PRINTLN ("* Cannot connect to WiFi *");
}

// this is the equivalent of loop() in manin program, and yielding happens here
void Esp12::update() {
    //mqtt_client.update(); TODO
    //ESP.wdtFeed(); // why do this unnecessarily ?
    //yield();  ??
}

void Esp12::reset_watchdog() {
    ESP.wdtFeed();
}

// ASSUMPTION: input status is a single digit number, in binary form
bool Esp12::sendStatus(byte status, byte destination) {
    makePayload(status); // status is now stored in payload
    switch (destination) {
      case SEND_TO_CLOUD_URL:
          send2CloudUrl();   
          break;
      case SEND_TO_TEST_URL:
          send2TestUrl();   
          break;
      case SEND_TO_DASHBOARD_URL:
          send2DashboardUrl();   
          break;          
      case SEND_TO_ANALYTICS_URL:
          send2AnalyticsUrl();   
          break;    
    }
}

void Esp12::makePayload(byte status) {
    body[body_length-2] = '0'+status;
    body2[body_length2-2] = '0'+status;
}
void Esp12::disable() {
    SERIAL_PRINTLN("-- Disabling wi-fi radio --");
    WiFi.forceSleepBegin(); // disable wifi
}

void Esp12::enable() {  // TODO: implement
    SERIAL_PRINTLN("-- Disabling wi-fi radio --");
}

// TODO: combine the following 3 into one method
bool Esp12::send2CloudUrl() {
    if(WiFi.status()!= WL_CONNECTED) {
        SERIAL_PRINTLN("No Wi-Fi connection");
        connect_wifi();
        return false;   
    }
    bool server_connected = htclient.begin (C.prod_url);        
    if (!server_connected) {
        SERIAL_PRINTLN("Could not connect to AWS server");
        return false;   
    }
    if (C.verbose_mode)
        SERIAL_PRINTLN("Connected to AWS server");
    htclient.addHeader("Content-Type", "application/json");
    if (C.verbose_mode)
        SERIAL_PRINTLN(body);
    int retcode = htclient.POST(body); 
    SERIAL_PRINTLN(retcode);
    if (retcode < 0) {
        SERIAL_PRINTLN("Cannot POST to AWS server..");
        SERIAL_PRINTLN(htclient.errorToString(retcode).c_str());
    }
    else {
        String response = htclient.getString(); 
        if (C.verbose_mode)
            SERIAL_PRINTLN(response);
    }
    //SERIAL_PRINTLN("Client disconnected.\n");
    htclient.end();   // close  
    return (retcode >= 0);       
}

bool Esp12::send2TestUrl() {
    if(WiFi.status()!= WL_CONNECTED) {
        SERIAL_PRINTLN("No Wi-Fi connection");
        connect_wifi();
        return false;   
    }
    bool server_connected = htclient.begin (C.test_url, C.dweet_thumb_print);        
    if (!server_connected) {
        SERIAL_PRINTLN("Could not connect to test server");
        return false;   
    }
    if (C.verbose_mode)
        SERIAL_PRINTLN("Connected to test server");
    htclient.addHeader("Content-Type", "application/json");
    if (C.verbose_mode)
        SERIAL_PRINTLN(body);
    int retcode = htclient.POST(body); 
    SERIAL_PRINTLN(retcode);
    if (retcode < 0) {
        SERIAL_PRINTLN("Cannot POST to test server..");
        SERIAL_PRINTLN(htclient.errorToString(retcode).c_str());
    }
    else {
        String response = htclient.getString(); 
        if (C.verbose_mode){
            SERIAL_PRINTLN(response);
            SERIAL_PRINTLN("");
        }
    }
    //SERIAL_PRINTLN("Client disconnected.\n");
    htclient.end();   // close  
    return (retcode >= 0);       
}


bool Esp12::send2DashboardUrl() {
    if(WiFi.status()!= WL_CONNECTED) {
        SERIAL_PRINTLN("No Wi-Fi connection");
        connect_wifi();
        return false;   
    }
    bool server_connected = htclient.begin (C.dashboard_url);          
    if (!server_connected) {
        SERIAL_PRINTLN("Could not connect to dashboard server");
        return false;   
    }
    if (C.verbose_mode)
        SERIAL_PRINTLN("Connected to dashboard server");
    htclient.addHeader("Content-Type", "application/json");
    if (C.verbose_mode)
        SERIAL_PRINTLN(body);
    int retcode = htclient.POST(body); 
    SERIAL_PRINTLN(retcode);
    if (retcode < 0) {
        SERIAL_PRINTLN("Cannot POST to dashboard server..");
        SERIAL_PRINTLN(htclient.errorToString(retcode).c_str());
    }
    else {
        String response = htclient.getString(); 
        if (C.verbose_mode){
            SERIAL_PRINTLN(response);
            SERIAL_PRINTLN("");
        }
    }
    //SERIAL_PRINTLN("Client disconnected.\n");
    htclient.end();   // close  
    return (retcode >= 0);       
}

bool Esp12::send2AnalyticsUrl() {
    if(WiFi.status()!= WL_CONNECTED) {
        SERIAL_PRINTLN("No Wi-Fi connection");
        connect_wifi();
        return false;   
    }
    bool server_connected = htclient.begin (C.analytics_url, C.speak_thumb_print);          
    if (!server_connected) {
        SERIAL_PRINTLN("Could not connect to analytics server");
        return false;   
    }
    if (C.verbose_mode)
        SERIAL_PRINTLN("Connected to analytics server");
    htclient.addHeader("Content-Type", "application/json");
    if (C.verbose_mode)
        SERIAL_PRINTLN(body2);
    int retcode = htclient.POST(body2); 
    SERIAL_PRINTLN(retcode);
    if (retcode < 0) {
        SERIAL_PRINTLN("Cannot POST to analytics server..");
        SERIAL_PRINTLN(htclient.errorToString(retcode).c_str());
    }
    else {
        String response = htclient.getString(); 
        if (C.verbose_mode){
            SERIAL_PRINTLN(response);
            SERIAL_PRINTLN("");
        }
    }
    //SERIAL_PRINTLN("Client disconnected.\n");
    htclient.end();   // close  
    return (retcode >= 0);       
}
