/*

  Brewing routines.  Prime the pumps and pour the coffee..

*/


#include "Brew.h"
#include "Brew.h"
#include "Display.h"

extern Brew brew;
extern Display display;
extern Timer timer;
extern Monitor monitor;

Brew::Brew()
{

}

void Brew::init( void )
{

}

void Brew::test( void )
{

}

void Brew::run( )
{
  // Serial.print("Task1 running on core ");
  // Serial.println(xPortGetCoreID());

  while ( 1 )
  {

    switch ( this->program )
    {
      case 0:
        program0();
        break;
      case 1:
        program1();
        break;

    }
    
    // digitalWrite(led1, HIGH);
    delay( 200 );

  } 
 
}

void Brew::start( void )
{
  timer.start();      
  this->running = 1;

}

void Brew::stop( void )
{
  monitor.turn( PIN_PUMP, M_OFF );
  timer.stop();
  this->running = 0;

}

// Set which brew program to run.
uint8_t Brew::setProgram( uint8_t program )
{
  if ( program >= 0 and program <= 1 )
  {
    this->program = program;
    display.setProgram( this->program );
  }
  return this->program;
}

uint8_t Brew::getProgram( )
{
  return this->program;
}

// First program, run the pump for as long as it takes.
void Brew::program0( void )
{
  monitor.turn( PIN_PUMP, M_ON );

}


void Brew::program1( void )
{
    if ( brew.running )
    {
      if ( timer.seconds() < 3 )
      {
        monitor.turn( PIN_PUMP, M_ON );
      } else if ( timer.seconds() < 7  )
      {
        monitor.turn( PIN_PUMP, M_OFF );
      } else {
        monitor.turn( PIN_PUMP, M_ON );
      }

    }

}


