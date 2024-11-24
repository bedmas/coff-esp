/*
  Monitor.h

  Monitor the changes on the system.

*/

#ifndef BEDMAS_MONITOR
#define BEDMAS_MONITOR

#include "Brew.h"
#include "Display.h"

#define PIN_LEVER 36
#define PIN_BOILER_SENSOR 39
#define PIN_RESERVOIR_SENSOR 34

#define PIN_BOILER 25
#define PIN_SOLINOID 26
#define PIN_PUMP 27

#define M_ON 1
#define M_OFF 0

#define M_EMPTY 1


class Monitor {

  private:


  public: 
    Monitor( );

    uint8_t boiler_status = 0;
    uint8_t solinoid_status = 0;
    uint8_t pump_status = 0;

    uint8_t water_status = 0;
    uint8_t reservoir_status = 0;
    uint8_t lever_status = 0;

    uint8_t empty = 0;

    void init( void );
    void test( void );
    void run( void );
    uint8_t checkBoiler( void );
    uint8_t checkReservoir( void );
    uint8_t checkLever( void );
 
    void turn_off( void );
    void turn( uint8_t device, uint8_t status);

    void fill_boiler( void );
  
};


#endif