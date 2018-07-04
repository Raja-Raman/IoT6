#ifndef COMMON_H
#define COMMON_H

//common.h
 
// comment out this line to disable serial messages
#define ENABLE_DEBUG
// serial debugging
#ifdef ENABLE_DEBUG
  #define  SERIAL_PRINT(x)  if(C.enable_serial) Serial.print(x)
  #define  SERIAL_PRINTLN(x)  if(C.enable_serial) Serial.println(x)
#else
  #define  SERIAL_PRINT(x)
  #define  SERIAL_PRINTLN(x)
#endif

#define SEND_TO_CLOUD_URL       1
#define SEND_TO_TEST_URL        2
#define SEND_TO_ANALYTICS_URL   3
#define SEND_TO_DASHBOARD_URL   4
#endif

