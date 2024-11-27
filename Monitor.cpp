/*

  Monitoring routines.   Goes around and checks on everything.

*/

#include "Monitor.h"
#include "Brew.h"
#include "Display.h"
#include "Timer.h"
#include "Communications.h"

extern Monitor monitor;
extern Brew brew;
extern Display display;
extern Timer timer;
extern Communications coms;

Monitor::Monitor()
{

}

void Monitor::init( void )
{

  // Read in from 36
  pinMode( PIN_LEVER, INPUT_PULLUP );
  pinMode( PIN_BOILER_SENSOR, INPUT );
  pinMode( PIN_RESERVOIR_SENSOR, INPUT );

  this->turn( PIN_BOILER, M_OFF );
  this->turn( PIN_SOLINOID, M_OFF );
  this->turn( PIN_PUMP, M_OFF );

  pinMode( PIN_BOILER, OUTPUT );
  pinMode( PIN_SOLINOID, OUTPUT );
  pinMode( PIN_PUMP, OUTPUT );

}

void Monitor::test( void )
{

    while ( 1 )
    {

      display.setStatus( "All Off.");
      this->turn( PIN_BOILER, M_OFF );
      this->turn( PIN_SOLINOID, M_OFF );
      this->turn( PIN_PUMP, M_OFF );

      this->setStatus(M_STATUS_WATER, M_FULL);
      this->setStatus(M_STATUS_RESERVOIR, M_FULL);
      display.setUpdate( D_UPDATE_ICONS );
      delay( 5000 );

      display.setStatus( "Water empty....");
      this->setStatus(M_STATUS_WATER, M_EMPTY);
      this->setStatus(M_STATUS_RESERVOIR, M_EMPTY);
      display.setUpdate( D_UPDATE_ICONS );
      delay( 5000 );

      display.setStatus( "Boiler Water filled.");
      this->setStatus(M_STATUS_WATER, M_FULL);
      display.setUpdate( D_UPDATE_ICONS );
      delay( 5000 );

      display.setStatus( "Reservoir filled");
      this->setStatus(M_STATUS_RESERVOIR, M_FULL);
      display.setUpdate( D_UPDATE_ICONS );
      delay( 5000 );


      display.setStatus( "Pump ON.");
      this->turn( PIN_PUMP, M_ON );
      delay( 5000 );

      display.setStatus( "Pump OFF.");
      this->turn( PIN_PUMP, M_OFF );
      delay( 5000 );

      display.setStatus( "Solinoid ON.");
      this->turn( PIN_SOLINOID, M_ON );
      delay( 5000 );

      display.setStatus( "Solinoid OFF.");
      this->turn( PIN_SOLINOID, M_OFF );
      delay( 5000 );
      
      display.setStatus( "Boiler ON.");
      this->turn( PIN_BOILER, M_ON );
      delay( 5000 );

      display.setStatus( "Boiler OFF.");
      this->turn( PIN_BOILER, M_OFF );
      delay( 5000 );




      display.setStatus( "Boiler ON.");
      this->turn( PIN_BOILER, M_ON );
      delay( 5000 );

      display.setStatus( "Solinoid ON.");
      this->turn( PIN_SOLINOID, M_ON );
      delay( 5000 );

      display.setStatus( "Pump ON.");
      this->turn( PIN_PUMP, M_ON );
      delay( 5000 );

      display.setStatus( "Boiler OFF.");
      this->turn( PIN_BOILER, M_OFF );
      delay( 5000 );

      display.setStatus( "Solinoid OFF.");
      this->turn( PIN_SOLINOID, M_OFF );
      delay( 5000 );

      display.setStatus( "Pump OFF.");
      this->turn( PIN_PUMP, M_OFF );
      delay( 5000 );

      display.setStatus( "All On.");
      this->turn( PIN_BOILER, M_ON );
      this->turn( PIN_SOLINOID, M_ON );
      this->turn( PIN_PUMP, M_ON );
      delay( 15000 );



    }



}

void Monitor::run(  )
{
  // Serial.print("Task1 running on core ");
  // Serial.println(xPortGetCoreID());

  while ( 1 )
  {

    monitor.checkReservoir();
    monitor.checkBoiler();

    // The boiler is empty.   As long as we have water in the reservoir fill it.
    if ( this->status( M_STATUS_WATER ) == M_EMPTY )
    {
      if ( this->status( M_STATUS_RESERVOIR ) == M_EMPTY )
      {
        display.setMessage( "out of water" );
        this->turn_off();
      } else {
        this->fill_boiler();
      }
    } else {
      display.setMessage( "normal operations" );
      coms.debug( "Normal operations ");
      this->turn( PIN_BOILER, M_ON );

      if ( ! brew.program_running )
      {
        coms.debug( "program not running turning everything off." );
        this->turn( PIN_SOLINOID, M_OFF );
        this->turn( PIN_PUMP, M_OFF );
      }

    }


    // digitalWrite(led1, HIGH);
    delay( 100 );

  } 
 
}

uint8_t Monitor::checkBoiler( void )
{
  int analog = analogRead( PIN_BOILER_SENSOR );
  coms.debug( String( "Boiler analog read?" ) + String( analog ) );   

  uint8_t status = this->threshold( analog );
  // Serial.printf( "Boiler water value %d\n", read_value );
  if ( status != this->status( M_STATUS_WATER )  )
  {
    // The state has changed do something.
    this->setStatus( M_STATUS_WATER, status);
    display.setUpdate( D_UPDATE_ICONS );

  }

  return (uint8_t)this->status( M_STATUS_WATER );

}

uint8_t Monitor::checkReservoir( void )
{
  int analog = analogRead( PIN_RESERVOIR_SENSOR );
  if ( this->debug ) { Serial.printf( "Reservoir Anolog: %d\n", analog ); }
  uint8_t status = this->threshold( analog );
  // Serial.printf( "ReservoiR value %d\n", read_value );
  if ( status != this->status( M_STATUS_RESERVOIR ) )
  {
    // Serial.printf( "Read value %d\n", read_value );
    // The state has changed do something.
    this->setStatus( M_STATUS_RESERVOIR, status );
    display.setUpdate( D_UPDATE_ICONS );

  }
  return (uint8_t)this->status( M_STATUS_RESERVOIR );
}

uint8_t Monitor::checkLever( void )
{
  // The values are all backwards with the pullup resistors.
  uint8_t read_value = !digitalRead( PIN_LEVER );
  if ( read_value != this->status( M_STATUS_LEVER )  )
  {
    // The state has changed do something.
    this->setStatus( M_STATUS_LEVER, read_value );

    if ( this->status( M_STATUS_LEVER ) )
    {
      brew.start();
    } else {
      this->turn( PIN_PUMP, M_OFF );
      brew.stop();
    }
    
  }
  return (uint8_t)this->status( M_STATUS_LEVER );
}

void Monitor::fill_boiler()
{
 
  this->turn( PIN_PUMP, M_ON );
  this->turn( PIN_SOLINOID, M_ON );
  Serial.printf( "fill boiler ");
  this->turn( PIN_BOILER, M_OFF );

  display.setMessage( "filling boiler" );

  int index;
  // Wait one second there.
  delay( 500 );
  while ( this->checkBoiler() == M_EMPTY )
  {
    if ( this->debug ) { Serial.printf( "Fill boiler loop index: %d\n", index++ ); }
    if ( this->checkReservoir() == M_EMPTY )
    {
      this->turn_off();
      return;
    }
    delay( 500 );
    
  }

  this->empty = 0;
  display.clearStatus();

  this->turn( PIN_PUMP, M_OFF );
  this->turn( PIN_SOLINOID, M_OFF );
  // Serial.printf( "fill boiler finished ");
  this->turn( PIN_BOILER, M_ON );

  display.setMessage( "finished filling boiler" );

}

uint8_t Monitor::setStatus( int device, uint8_t status )
{

  // If we are trying to set it to low then we need to xor else we or it.
  if ( status == 0 && ( this->device_status & device )  )   
  {
    // Serial.print( "Device: ");
    // Serial.print( device, BIN );
    // Serial.print( "   Set Status ");
    // Serial.println( this->device_status, BIN );
    // Serial.printf( "Xor %b %b\n", device, status );
    this->device_status = this->device_status ^ device;
  } else if ( status == 1 )  {
    //Serial.printf( "Or %b %b\n", device, status );
    this->device_status = this->device_status | device;
  }

  return (this->device_status & device);

}


uint8_t Monitor::status( int device )
{
  return ( this->device_status & device );

}

void Monitor::turn_off( void )
{
  display.setStatus( "turned off" );
  this->turn( PIN_PUMP, M_OFF );
  this->turn( PIN_SOLINOID, M_OFF );
  this->turn( PIN_BOILER, M_OFF );
  this->empty = 1;
}


void Monitor::turn( int device, uint8_t status )
{
  if ( device == PIN_PUMP )
  {
      this->setStatus( M_STATUS_PUMP, status );
      coms.debug( String( "Pump status ") + String( status ) );

  } else if ( device == PIN_SOLINOID )
  {
      this->setStatus( M_STATUS_SOLINOID, status );
      coms.debug( String( "Solinoid status ") + String( status ) );

  } else if ( device == PIN_BOILER )
  {
      this->setStatus( M_STATUS_BOILER, status); 
      coms.debug( String( "Boiler status ") + String( status ), 1 );
  }

  // Set the icons to update on the display
  display.setUpdate( D_UPDATE_ICONS );

  // As long as there is water or we are turning something off.
  if ( !this->empty || !status )
  {
    digitalWrite( device, status );
  }

}

// We may want to do mor with this....
uint8_t Monitor::threshold( int value )
{
  return ( value < 500 );
}

