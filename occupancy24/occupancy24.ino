/* 
Room occupancy sensor with ESP12 as the only processor (No ATMega).
See config.h for the cloud URLs
*/

#include "occupancy.h"
#include "esp12.h"

Config C;
Esp12  E(C);
Timer  T;  // software timer

// shared globals
boolean  occupied = true;        
boolean pir_status;
boolean radar_status;
unsigned int pir_tick_counter = 0;
unsigned int radar_tick_counter = 0;
unsigned int status_counter = 0;

void setup() {
    pinMode(led1, OUTPUT);   //  active low
    pinMode(led2, OUTPUT);   //  active low
    //pinMode(pir, INPUT_PULLUP);      // for testing without sensors
    //pinMode(radar, INPUT_PULLUP);     
    pinMode(pir, INPUT);      
    pinMode(radar, INPUT);    
    pinMode(jumper, INPUT_PULLUP);
    boolean sensor_test_mode = !digitalRead(jumper);
    //if(C.enable_serial)  // still need to receive Rx commands from PC
        Serial.begin(C.baud_rate);
    delay(200); // go over the initial garbage printing
    SERIAL_PRINTLN("\n\nESP12 restarting...");

    if (sensor_test_mode) 
        sensor_test_loop();  // *** THIS IS AN INFINITE LOOP ****
        
    if (C.simulation_mode) { // without these braces, the macro is not expanded correctly
        SERIAL_PRINTLN ("Occupancy monitor is in simulation mode ***");
    }
    else {       
        SERIAL_PRINTLN ("Occupancy monitor is in production mode.");
    }
    blinker1();
    
    E.init();  // essential      

    occupied = true;   // TODO: revisit this   
    T.every(C.tick_interval, ticker);     
    T.after(1000, send_restarted_status); 
}

void loop() {
    T.update();  // this is a must
    E.update();  // this is a must
    if (C.enable_serial && Serial.available()) {
        char c = Serial.read();
        execute_serial_command (c);
    }
}

void ticker() {
    read_sensors();  // read PIR and radar...
    update_status(); // ...and then compute occupancy status     
}

void occupy_room() {
    occupied = true; 
    if (C.verbose_mode)
        SERIAL_PRINTLN ("Room ccupied");    
    send_status(STATUS_OCCUPIED); 
}

// pre-release warning flashes
void warn(byte warning_type) {
    T.oscillate (led2,50, HIGH, 10);  
    //the end state is HIGH, i.e, led is off     
    if (C.verbose_mode)     
        SERIAL_PRINTLN ("- About to release room -");   
    send_status(warning_type); 
}

void release_room() {
    occupied = false; 
    if (C.verbose_mode)    
        SERIAL_PRINTLN ("Room released.");
    send_status(STATUS_FREE); 
}

boolean simulated_occupied = true;
byte status_cache;
void send_status (byte status) {
    status_cache = status;
    if (C.simulation_mode) {
        //simulated_occupied = !simulated_occupied;  
        simulated_occupied = random(0,2);  // [min, max)
        if (C.verbose_mode) {
          SERIAL_PRINT("Sending (s) status: ");
          SERIAL_PRINTLN(simulated_occupied);      
        } 
        E.sendStatus(simulated_occupied ? STATUS_OCCUPIED : STATUS_FREE, SEND_TO_TEST_URL);    
    }
    else {
        if (C.verbose_mode) {      
          SERIAL_PRINT("Sending (p) status: ");
          SERIAL_PRINTLN(status);  
        }
        E.sendStatus(status_cache, SEND_TO_CLOUD_URL);    
        T.after(2000, send2TestUrl);    
        T.after(4000, send2DashboardUrl);           
        T.after(6000, send2AnalyticsUrl);   
    }  
}

void send2TestUrl() {
    E.sendStatus(status_cache, SEND_TO_TEST_URL); 
}

void send2DashboardUrl() {
    E.sendStatus(status_cache, SEND_TO_DASHBOARD_URL); 
}

void send2AnalyticsUrl() {
    E.sendStatus(status_cache, SEND_TO_ANALYTICS_URL); 
}

// called by the timer
void send_restarted_status() {
    send_status(STATUS_RESTARTED);
}

// You MUST call read_sensors before calling update_status
void read_sensors() { 
    pir_status = digitalRead(pir);   
    radar_status = digitalRead(radar);
    digitalWrite (led1, !pir_status);  // active low
    digitalWrite (led2, !radar_status);
    if (pir_status) pir_tick_counter = 0;      // keep resetting it, if there is any motion
    if (radar_status) radar_tick_counter = 0;  // keep resetting it, if there is any motion  
}

// Uses shared variables; Call read_sensors before calling this !
void update_status() {    
    pir_tick_counter++;      
    radar_tick_counter++;    
    status_counter++;   
      
    if (!occupied) {
        if (pir_status && radar_status)    // both the sensors fired, so occupy the room
            occupy_room();
    }
    if (status_counter == C.status_ticks) { 
        send_status(occupied ? STATUS_OCCUPIED : STATUS_FREE);
        status_counter = 0;
    }
    if (radar_tick_counter == C.buzzer_ticks1) {
        if (occupied)
            warn(STATUS_WARNED1);  // warn about the imminent release
    }
    if (pir_tick_counter == C.buzzer_ticks2) {
        if (occupied)
            warn(STATUS_WARNED2);  // warn about the imminent release
    }
         
    if (radar_tick_counter >= C.release_ticks1){  // even the radar was silent for 2 minutes
         radar_tick_counter = 0; // to avoid counter overflow during nights/holidays
         if (occupied)
            release_room();  
    }
    if (pir_tick_counter >= C.release_ticks2){  // PIR was silent for 10 minutes
         pir_tick_counter = 0; // to avoid counter overflow during nights/holidays
         if (occupied)
            release_room();              
    }  
}

/*
 Receive a single byte command from PC and execute it.
 R = restart 8266
 L = sensor test loop. (infinite loop, needs reset).
 S = send status immediately
 */
void execute_serial_command (char c) {
    switch(c) {
        case ('R'):
            SERIAL_PRINTLN("***** Now ESP will restart !! ******");
            delay(1000);
            ESP.reset();
            break;
        case ('L'):
            sensor_test_loop();  // this is an infinite loop ***
            break;         
        case ('S'):
            if (C.verbose_mode)
                SERIAL_PRINTLN("resending status data");
            send_status(occupied ? STATUS_OCCUPIED : STATUS_FREE);
            break;                 
    }
}

// WARNING: THIS IS AN INFINITE LOOP
// for normal program, the jumper MUST be be disconnected
void sensor_test_loop() {
    SERIAL_PRINTLN ("--- ENTERING SENSOR TEST MODE ---");
    SERIAL_PRINT ("For normal operation, ensure jumper on GPIO ");
    SERIAL_PRINT(jumper); 
    SERIAL_PRINTLN (" is not connected");
    SERIAL_PRINTLN ("and restart the device.");
    E.disable(); // turn off wifi
    blinker2();  
    while (1) {
        digitalWrite (led1, !digitalRead(pir));   
        digitalWrite (led2, !digitalRead(radar));   
        E.reset_watchdog(); // resetting watchdog is needed only in this loop  
        E.update();  // essential to receive MQTT commands
        delay(50); 
    }
}

void blinker1 () {
    for (int i=0; i<10; i++) {
        digitalWrite (led1, 0);      
        delay(50); 
        digitalWrite (led1, 1);   // active low 
        delay(50);   
    }
    for (int i=0; i<4; i++) {
        digitalWrite (led2, 0);      
        delay(200); 
        digitalWrite (led2, 1);   // active low 
        delay(200);   
    } 
}

void blinker2 () {
    for (int i=0; i<10; i++) {
        digitalWrite (led1, 0);  // active low    
        digitalWrite (led2, 1);  // active low 
        delay(250); 
        digitalWrite (led1, 1);   
        digitalWrite (led2, 0);  
        delay(250);   
    }
    digitalWrite (led2, 1);    
}
