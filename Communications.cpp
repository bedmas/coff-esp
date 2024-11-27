#include "Communications.h"
#include <cmath>

extern Display display;
extern Brew brew;
extern Monitor monitor;
extern Timer timer;

Communications::Communications()
{

}

void Communications::init()
{

  this->SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());


}

void Communications::monitor()
{

  if ( this->SerialBT.available() ) {
    uint8_t read = this->SerialBT.read();
    // Serial.write( read );

    if ( read == COMMAND_END )
    {
      // Store the null. character at the end of the string.
      // this->SerialBT.read();    //End the string.  this should only be done when the command end is met but I want to see it every iteration when developing..
      read = this->SerialBT.read();
      this->command_string[ this->command_index] = '\0';
      this->runCommand( );   

    } else if ( read > 31 ) {
      this->command_string[ this->command_index++ ] = read;

    }

    
  }
}

// If avaliable sne a message...
void Communications::send( String message )
{
  if ( this->SerialBT.available() )
  {
    this->SerialBT.println( message );
  }
}

// If avaliable sne a message...
void Communications::debug( String message )
{
  if ( this->debug_flag )
  {
    if ( this->SerialBT.available() )
    {
      this->SerialBT.println( message );
    } else {
      Serial.println( message );
    }
    
  }
}

// If avaliable sne a message...
void Communications::debug( String message, int level )
{
  if ( level > 0 )
  {
    if ( this->SerialBT.available() )
    {
      this->SerialBT.println( message );
    } else {
      Serial.println( message );
    }
  }
}





void Communications::runCommand( )
{

  switch ( this->command() )
  {
    case 'b':
      // Change the brightness;
      this->SerialBT.printf( "Brightness set to %d\n", display.setBrightness( this->command_value() ) );
      break;

    case 'p':
      // Change the program;
      this->SerialBT.printf( "Program set to %d\n", brew.setProgram( this->command_value() ) );
      break;

    case 't':
      // toggle the timer
      if ( timer.isRunning() ) { 
        timer.stop(); 
        this->SerialBT.printf( "Timer has been stopped\n" );
      }
      else { 
        timer.start(); 
        this->SerialBT.printf( "Start the timer\n" );

      }
      break;

    case 'h':
      // toggle the timer
      this->SerialBT.printf( "Help for Coff-esp\n\n" );
      this->SerialBT.printf( "  b<n> - brightness level to <n>\n" );
      this->SerialBT.printf( "  t - toggle the timer on off\n" );
      this->SerialBT.printf( "  h - this help.\n" );
      this->SerialBT.printf( "\n" );

      break;

    default:
        this->SerialBT.printf( "Unknown command %c\n", this->command() );
  }
  this->command_index = 0;
 

}

// Return the command character.
char Communications::command( void )
{
  return this->command_string[0];
}

// Return the command value
uint8_t Communications::command_value( void )
{
  uint8_t sum = 0;
  for( uint8_t i = 1; i < this->command_index; i++ )
  {
    if ( this->command_string[i] >= 48 && this->command_string[i] >= 48  )
    {
      sum += (this->command_string[i]-48)*pow(10,this->command_index-(i+1) );
    }
  }
  return sum;
}

