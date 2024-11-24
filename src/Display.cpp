#include "Display.h"

/* 
  Display Class to hold the class to display the stuff.   
  
  Maybe use two displays at once using the multiplexer!

*/
extern Timer timer;
extern Monitor monitor;

Display::Display( )
{

}

void Display::init( void )
{
  // Set up the ld display brightness...
  pinMode(33, OUTPUT);
  analogWrite(33, this->brightness );


  // Setup the LCD
  this->screen.init();
  this->screen.setRotation(2);
  this->screen.fillScreen(D_BACKGROUND);

  this->drawTimer();
  this->drawIcons();
}

void Display::refresh( void ) 
{
  if ( this->update > 0 )
  {
    if ( this->update > 1 )
    {
      this->drawTitle();
      this->drawMessage();
      this->drawStatus();
      this->drawIcons();
      this->update = 0;

    }

  }

  if ( timer.isRunning() && this->last_timer != timer.seconds() )
  {
    this->last_timer = timer.seconds();
    this->drawTimer();
  }


}

void Display::setTitle( String value ) { this->title = value; this->update = 2; }
void Display::setMessage( String value ) { this->message = value; this->update = 2; }
void Display::setStatus( String value ) { this->status = value; this->update = 2; }
void Display::setProgram( uint8_t value  ) { this->program = value; this->update = 2; }


/* */
void Display::drawTitle( )
{
  this->screen.setTextDatum(TC_DATUM);
  this->screen.setTextFont(4); // Set the text font to font number 2
  this->screen.setTextSize(1);
  this->screen.setTextColor(TFT_WHITE, D_BACKGROUND); // Set the text color to white with black background

  // Draw a string at position (100, 100) on the screen
  this->screen.drawString(this->title, D_WIDTH/2, 5 );

  this->screen.setTextDatum(TL_DATUM); // Default.

}

void Display::drawMessage( )
{

  this->screen.setTextFont(2); // Set the text font to font number 2
  this->screen.setTextSize(1);
  this->screen.setTextColor(TFT_WHITE, D_BACKGROUND); // Set the text color to white with black background

  // Draw a string at position (100, 100) on the screen
  this->screen.drawString(this->message, 10, 30);

}

void Display::drawStatus( )
{

  this->screen.setTextFont(2); // Set the text font to font number 2
  this->screen.setTextSize(1);
  this->screen.setTextColor(TFT_WHITE, D_BACKGROUND); // Set the text color to white with black background

  // Draw a string at position (100, 100) on the screen
  this->screen.drawString(this->status, 10, 106);

}

void Display::drawIcons( )
{

  this->screen.setTextFont(2); // Set the text font to font number 2
  this->screen.setTextSize(1);
  this->screen.setTextColor(TFT_WHITE, D_BACKGROUND); // Set the text color to white with black background

  this->drawIcon( 1, String("P")+ String(this->program), 5 );

  this->drawIcon( monitor.boiler_status, "B", 20 );
  this->drawIcon( monitor.solinoid_status, "S", 35 );
  this->drawIcon( monitor.pump_status, "P", 50 );
  
  this->drawIcon( monitor.water_status, "W", 80 );
  this->drawIcon( monitor.reservoir_status, "R", 95 );

}

void Display::drawIcon( uint8_t status, String icon, uint8_t x )
{
    // Draw a string at position (100, 100) on the screen
  if ( status )
  {
    this->screen.drawString( icon, x, 144 );
  } else {
    this->screen.drawString( "  ", x, 144 );
  }

}

void Display::drawTimer( )
{
  
  this->screen.setTextSize(1);
  // Uncomment ONE of the next 2 lines, using the ghost image demonstrates text overlay as time is drawn over it
  this->screen.setTextColor(TFT_DARKGREY, D_BACKGROUND);  // Leave a 7 segment ghost image, comment out next line!
  this->screen.drawString("88", 30, 55, D_FONT_DIGITAL); // Overwrite the text to clear it

  this->screen.setTextColor(TFT_GREEN); // Orange

  String seconds = String( "00" ) + String( timer.seconds() );
  this->screen.drawString( seconds.substring( seconds.length()-2), 30, 55, D_FONT_DIGITAL); // Overwrite the text to clear it

}

uint8_t Display::updateBrightness( uint8_t amount )
{
  this->setBrightness( this->brightness + amount );
  return this->brightness;
}

uint8_t Display::setBrightness( uint8_t amount )
{
  this->brightness = amount;
  if ( this->brightness <= 0 )
  {
    this->brightness = 0;
  } else if ( this->brightness > D_MAX_BRIGHTNESS )
  {
    this->brightness = D_MAX_BRIGHTNESS;
  }

  this->setStatus( String("Brightness set to ") + String( this->brightness ) );
  analogWrite(D_PIN_LED, this->brightness );
  return this->brightness;

}

uint8_t Display::getBrightness( void )
{
  return this->brightness;
}



void Display::boilerIcon( uint8_t status )
{
  this->boiler_icon = status;
  this->update = 2;
}

void Display::solinoidIcon( uint8_t status )
{
  this->solinoid_icon = status;
  this->update = 2;
}

void Display::pumpIcon( uint8_t status )
{
  this->pump_icon = status;
  this->update = 2;
}

void Display::waterIcon( uint8_t status )
{
  this->water_icon = status;
  this->update = 2;
}

void Display::reservoirIcon( uint8_t status )
{
  this->reservoir_icon = status;
  this->update = 2;
}


void Display::test( void )
{
  while ( 1 )
  {
    // Turn everything on...refresh, turn everyhting off, turn on normal operatoins and run timer.
    timer.start();

    this->setMessage( "Starting");

    this->boilerIcon( D_OFF );
    this->solinoidIcon( D_OFF );
    this->pumpIcon( D_OFF );

    this->waterIcon( D_OFF );
    this->reservoirIcon( D_OFF );
    delay( 2000 );

    this->boilerIcon( D_ON );
    delay( 2000 );
    this->solinoidIcon( D_ON );
    delay( 2000 );
    this->pumpIcon( D_ON );
    delay( 2000 );

    this->waterIcon( D_ON );
    delay( 2000 );
    this->reservoirIcon( D_ON );
    delay( 2000 );

    this->setMessage( "message....");
    this->setStatus( "waiting for 2 seconds              " );
    delay( 2000 );

    this->setMessage( "                                  ");
    this->setStatus( "another 2 seconds                  " );
    delay( 2000 );

    this->setStatus( "timer                               " );
    timer.stop();

    delay( 2000 );


  }


}




