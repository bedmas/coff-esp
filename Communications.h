#ifndef BEDMAS_COMS
#define BEDMAS_COMS

#include "BluetoothSerial.h"

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

#define COMMAND_END 13  // return is the indicator for ending a command from blue tooth.


#include "Display.h"
#include "Brew.h"
#include "Monitor.h"
#include "Timer.h"


class Communications {

  private:
    BluetoothSerial SerialBT;
    String device_name = "coff-esp";
    char command_string[128];
    uint8_t command_index = 0;


  public:
    uint8_t debug_flag = 0;

    Communications( void );
    void init( void );
    void monitor( void );
    void runCommand( void );
    void send( String message );

    void debug( String message );
    void debug( String message, int );

    char command( void );
    uint8_t command_value( void );

};

#endif