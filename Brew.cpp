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
    // monitor.checkLever();

    // If we have a program running then run the appropriate program..
    if ( brew.running )
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

    }    

    #ifdef DEBUG
    /* Inspect our own high water mark on entering the task. */
    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
    Serial.printf( "High water mark: %d\n", uxHighWaterMark );
    #endif

    // digitalWrite(led1, HIGH);
    delay( 200 );

  } 
 
}

void Brew::start( void )
{
  #ifdef DEBUG
  Serial.printf( "Starting\n" );
  #endif

  timer.start();      
  display.clearStatus( );
  this->running = 1;
  
}

void Brew::stop( void )
{
  #ifdef DEBUG
  Serial.printf( "Stop\n" );
  #endif
  
  timer.stop();
  display.clearStatus( );
  monitor.turnPumpOff();
  
  this->running = 0;

}

// Set which brew program to run.
int Brew::setProgram( int program )
{
  if ( program >= 0 and program <= 1 )
  {
    this->program = program;
    display.setProgram( this->program );
  }
  return this->program;
}

int Brew::getProgram()
{
  return this->program;
}

// First program, run the pump for as long as it takes.
void Brew::program0( void )
{
  display.setStatus( "Running program 0" );
  monitor.turnPumpOn( );

}


void Brew::program1( void )
{
  if ( timer.seconds() < 3 )
  {
    monitor.turnPumpOn();
  } else if ( timer.seconds() < 7  )
  {
    monitor.turnPumpOff();
  } else {
    monitor.turnPumpOn();
  }

}


