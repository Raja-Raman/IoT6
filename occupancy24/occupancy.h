// occupancy.h
/*
ESP12 Amica board
pin mapping of 8266 - on the right are the GPIO pin numbers:
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
*/

#include "common.h"
#include "config.h"
#include <EEPROM.h>             // bundled with Arduino IDE 
#include <Timer.h>              // https://github.com/SandyWalsh/Timer

extern "C" {
  #include "user_interface.h"
  #include "wpa2_enterprise.h"
}

#define STATUS_FREE       0
#define STATUS_OCCUPIED   1
#define STATUS_WARNED1    2
#define STATUS_WARNED2    3
#define STATUS_RESTARTED  4

const byte led1   = 2;    // GPIO 2,  D4 pin
const byte led2   = 16;   // GPIO 16, D0 pin
const byte pir    = 14;   // GPIO 14, D5 pin
const byte radar  = 13;   // GPIO 13, D7 pin
const byte jumper = 4;    // GPIO 4,  D2 pin; to enter sensor test mode

