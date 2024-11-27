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

#define M_ON LOW
#define M_OFF HIGH

#define M_EMPTY LOW
#define M_FULL HIGH

#define M_STATUS_BOILER    0b00000001
#define M_STATUS_SOLINOID  0b00000010
#define M_STATUS_PUMP      0b00000100
#define M_STATUS_WATER     0b00001000
#define M_STATUS_RESERVOIR 0b00010000
#define M_STATUS_LEVER     0b00100000



class Monitor {

  private:
    uint8_t debug = 0;

  public: 
    Monitor( );

    uint device_status = 0;
    uint8_t empty = 0;

    void init( void );
    void test( void );
    void run( void );

    uint8_t checkBoiler( void );
    uint8_t checkReservoir( void );
    uint8_t checkLever( void );

    uint8_t status( int );
    uint8_t setStatus( int, uint8_t );

    void turn_off( void );
    void turn( int , uint8_t );

    uint8_t switchDevice( int , int , uint8_t  );

    void fill_boiler( void );
  
    uint8_t threshold( int );
};


#endif