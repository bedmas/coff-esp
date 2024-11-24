/*

  Monitoring routines.   Goes around and checks on everything.

*/

#include "Monitor.h"
#include "Brew.h"
#include "Display.h"
#include "Timer.h"

extern Monitor monitor;
extern Brew brew;
extern Display display;
extern Timer timer;


Monitor::Monitor()
{

}

void Monitor::init( void )
{

  // Read in from 36
  pinMode( PIN_LEVER, INPUT_PULLUP );
  pinMode( PIN_BOILER_SENSOR, INPUT );
  pinMode( PIN_RESERVOIR_SENSOR, INPUT );
  
  pinMode( PIN_BOILER, OUTPUT );
  pinMode( PIN_SOLINOID, OUTPUT );
  pinMode( PIN_PUMP, OUTPUT );

}

void Monitor::test( void )
{

}

void Monitor::run(  )
{
  // Serial.print("Task1 running on core ");
  // Serial.println(xPortGetCoreID());

  while ( 1 )
  {

    monitor.checkReservoir();
    monitor.checkBoiler();
    monitor.checkLever();
   
    // digitalWrite(led1, HIGH);
    delay( 200 );

  } 
 
}

uint8_t Monitor::checkBoiler( void )
{
  int read_value = digitalRead( PIN_BOILER_SENSOR );
  if ( read_value != this->water_status )
  {
    // The state has changed do something.
    this->water_status = read_value;

    if ( this->water_status == M_EMPTY )
    {
      if ( this->reservoir_status == M_EMPTY )
      {
        this->turn_off();
      } else {
        this->fill_boiler();
      }
    } 
  }

  return (uint8_t)this->water_status;

}

uint8_t Monitor::checkReservoir( void )
{
  int read_value = digitalRead( PIN_RESERVOIR_SENSOR );
  if ( read_value != this->reservoir_status )
  {
    // The state has changed do something.
    this->reservoir_status = read_value;
  }
  return (uint8_t)this->reservoir_status;
}

uint8_t Monitor::checkLever( void )
{
  // The values are all backwards with the pullup resistors.
  int read_value = !digitalRead( PIN_LEVER );
  if ( read_value != this->lever_status )
  {
    // The state has changed do something.
    this->lever_status = read_value;

    if ( this->lever_status )
    {
      brew.start();
    } else {
      this->turn( PIN_PUMP, M_OFF );
      brew.stop();
    }
    
  }
  return (uint8_t)this->lever_status;
}

void Monitor::fill_boiler()
{
 
  turn( PIN_PUMP, M_ON );
  turn( PIN_SOLINOID, M_ON );
  turn( PIN_BOILER, M_OFF );

  // Wait one second there.
  delay( 1000 );
  while ( this->checkBoiler() == M_EMPTY )
  {
    if ( this->checkReservoir() == M_EMPTY )
    {
      this->turn_off();
      return;
    }
    delay( 1000 );
  }


}


void Monitor::turn_off( void )
{
  digitalWrite( PIN_PUMP, M_OFF );
  digitalWrite( PIN_SOLINOID, M_OFF );
  digitalWrite( PIN_BOILER, M_OFF );
  this->empty = 1;
}


void Monitor::turn( uint8_t device, uint8_t status )
{
  // As long as there is water or we are turning something off.
  if ( !this->empty || ! status)
  {
    digitalWrite( device, status );
  }

}


