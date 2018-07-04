#ifndef CONFIG_H
#define CONFIG_H
/*
To view the feeds:
The Public DNS of the AWS instance is ec2-52-40-239-77.us-west-2.compute.amazonaws.com
http://52.40.239.77:5000/get_latest_data
http://52.40.239.77:5000/get_complete_data
https://dweet.io/follow/raja-vz-raman3
https://demo.thingsboard.io/dashboards/49a59150-7d3b-11e8-9d6f-af866d0f1760
https://thingspeak.com/channels/530530
https://thingspeak.com/channels/530530/private_show (hotmail/R 1.4)
*/
 
//config.h
 
class Config {
public :
    long baud_rate = 115200L;   // 9600L  
    bool simulation_mode  = false;      // 1 for testing; 0 for production  
    bool enable_serial = true;
    bool verbose_mode = true;
    
    // * Timer durations SHOULD be unsigned LONG int, if they are > 16 bit! 
    unsigned int tick_interval  = 100;      // in milliSec; 10 ticks = 1 second 
    unsigned int status_ticks   = 60*10;    // n*10 ticks = n seconds  
    unsigned int release_ticks1 = 120*10;   // n*10 ticks = n seconds   // 60
    unsigned int release_ticks2 = 600*10;   // n*10 ticks = n seconds   // 1200
    unsigned int buzzer_ticks1  = 110*10;   // n*10 ticks = n seconds   // 50
    unsigned int buzzer_ticks2  = 590*10;   // n*10 ticks = n seconds   // 1190

    char *room_id = "\"CR8002\"";   // This should be unique for each device
      
    // json payload for all other portals
    char *payload1 =  "{\"location\":";
    char *payload2 =  ",\"status\":?}"; 

    // json payload for ThingSpeak
    char *payload3 =  "{\"api_key\":\"WZQ1LSWEU5SI0WYH\",\"field1\":?}";
          
    // cloud URL for posting the data
    char * prod_url = "http://52.40.239.77:5000/post_occupancy_status";   // production AWS
    char * test_url = "https://dweet.io/dweet/for/raja-vz-raman3";         
    char * dashboard_url = "http://demo.thingsboard.io:8080/api/v1/R3Nb5GBsW17Blr4CKweS/telemetry";   
    char * analytics_url = "https://api.thingspeak.com/update";   

    // security for dweet
    // copy the thumb print from the Dweet web page
    char * dweet_thumb_print = "4a4e39710b7dd795d23506c1c4bf488d925e3fed";
    //char * dweet_thumb_print = "4A:4E:39:71:0B:7D:D7:95:D2:35:06:C1:C4:BF:48:8D:92:5E:3F:ED";
    //bool thumb_print_needed;
    char *speak_thumb_print = "f9c2656cf9ef7f668bf735fe15ea829f5f55543e";
    
    char* ssid = "RajaCell"; 
    char* password = "raja1234";      
};
#endif

