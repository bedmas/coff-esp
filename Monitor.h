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

  public: 
    Monitor( );

    uint device_status = 0;
    bool empty = 0;

    void init( void );
    void test( void );
    void run( void );

    bool turnPumpOn( void );
    bool turnPumpOff( void );

    bool turnBoilerOn( void );
    bool turnBoilerOff( void );

    bool turnSolinoidOn( void);
    bool turnSolinoidOff( void );

    bool checkBoiler( void );
    bool checkReservoir( void );
    bool checkLever( void );

    bool status( int );
    bool setStatus( int, bool );

    void turn_off( void );
    void turn( int , bool );

    bool switchDevice( int , int , bool  );

    void fill_boiler( void );
  
    bool threshold( int );
};


#endif