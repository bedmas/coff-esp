#include "Display.h"

/* 
  Display Class to hold the class to display the stuff.   
  
  Maybe use two displays at once using the multiplexer!

*/
extern Communications coms;
extern Timer timer;
extern Monitor monitor;

Display::Display( )
{

}

Display::Display( String title )
{
 this->setTitle( title );
 this->init();

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

  this->drawTitle();
  this->drawTimer();
  this->drawIcons();


}

void Display::refresh( void ) 
{
  if ( this->update > 0 )
  {
    // if ( this->update == D_UPDATE_ALL ) { this->drawTitle(); } 
    if ( this->update & D_UPDATE_MESSAGE ) { this->drawMessage(); }
    if ( this->update & D_UPDATE_STATUS ) { this->drawStatus(); }
    if ( this->update & D_UPDATE_ICONS ) { this->drawIcons(); }
    this->update = 0;
    
  }

  this->drawDateTime();

  // Timer is always outside of the update.  We always updte it when it is running and when
  // the display has changed to stop the flickering...
  if ( timer.isRunning() && this->last_timer != timer.seconds() )
  {
    this->last_timer = timer.seconds();
    this->drawTimer();
  }


}

void Display::setTitle( String value ) { this->title = value; this->setUpdate( D_UPDATE_TITLE ); }
void Display::setMessage( String value ) { this->message = value; this->setUpdate( D_UPDATE_MESSAGE ); }
void Display::setStatus( String value ) { this->status = value; this->setUpdate( D_UPDATE_STATUS ); }
void Display::setProgram( uint value  ) { this->program = value; this->setUpdate( D_UPDATE_ICONS ); }

/*
  Clear the message should be faster if we just draw a rectangle background?
*/
void Display::clearMessage( void ) { this->message = ""; this->setUpdate( D_UPDATE_MESSAGE ); }  
void Display::clearStatus( void ) { this->status = ""; this->setUpdate( D_UPDATE_STATUS ); }  

// void Display::setIcon( int flag, uint8_t status  ) { 
//   if ( status ) { this->icons = this->icons | flag; }
//   else { this->icons = this->icons ^ flag; }
//   this->setUpdate( D_UPDATE_ICONS ); 
// }

void Display::setUpdate( int flag ) { this->update = this->update | flag;  }   // Combine the flags..


/* */
void Display::drawTitle( )
{
  this->screen.setTextDatum(TC_DATUM);
  this->screen.setTextFont(4); // Set the text font to font number 2
  this->screen.setTextSize(1);
  this->screen.setTextColor(D_TITLE_COLOUR, D_BACKGROUND); // Set the text color to white with black background

  // Draw a string at position (100, 100) on the screen
  this->screen.drawString( this->title, D_WIDTH/2, 5 );

  this->screen.setTextDatum(TL_DATUM); // Default.

}

void Display::drawMessage( )
{

  this->screen.setTextFont(2); // Set the text font to font number 2
  this->screen.setTextSize(1);
  this->screen.setTextColor(TFT_WHITE, D_BACKGROUND); // Set the text color to white with black background

  // Draw a string at position (100, 100) on the screen
  this->screen.drawString(this->message + "                            ", 10, 30);

}

void Display::drawStatus( )
{

  this->screen.setTextFont(2); // Set the text font to font number 2
  this->screen.setTextSize(1);
  this->screen.setTextColor(TFT_WHITE, D_BACKGROUND); // Set the text color to white with black background

  // Draw a string at position (100, 100) on the screen
  this->screen.drawString(this->status + "                            ", 10, 106);

}

void Display::drawIcons( )
{

  this->screen.setTextFont(2); // Set the text font to font number 2
  this->screen.setTextSize(1);

  this->drawIcon( M_ON, String("P")+ String(this->program), 5 );

  this->drawIcon( monitor.status( M_STATUS_BOILER) , "B", 30 );
  this->drawIcon( monitor.status( M_STATUS_SOLINOID ), "S", 45 );
  this->drawIcon( monitor.status( M_STATUS_PUMP ) , "P", 60 );
  
  this->drawIcon( monitor.status( M_STATUS_WATER ), "W", 85 );
  this->drawIcon( monitor.status( M_STATUS_RESERVOIR ), "R", 100 );

}

void Display::drawIcon( uint8_t status, String icon, uint8_t x )
{
    // Draw a string at position (100, 100) on the screen
  if ( status == M_ON )
  {
    this->screen.setTextColor(D_ICON_ON, D_BACKGROUND); // Set the text color to white with black background
    this->screen.drawString( icon, x, 144 );
  } else {
    this->screen.setTextColor(D_ICON_OFF, D_BACKGROUND); // Set the text color to white with black background
    this->screen.drawString( icon, x, 144 );
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

void Display::drawDateTime()
{

  // Get the local time... 
  if( !getLocalTime( &this->timeinfo, 3000) )
  {
    Serial.println("Failed to obtain time");
    // Reconnect, and reinitialize the display.
    if ( !coms.ntpTimeSet) { coms.ntpConnect(); this->init(); }

    return;
  }

  // Only need to update every minute.
  if ( this->timeinfo.tm_min == this->last_min )
  {
    return;
  }
  
  if ( timeinfo.tm_year == 0 ) { coms.ntpConnect(); this->init(); }

  #ifdef DEBUG  
  Serial.println(&this->timeinfo, "%A, %B %d %Y %H:%M:%S");
  #endif

  char output[20] = "";
  sprintf( output, "%04d-%02d-%02d %02d:%02d", 
      1900+this->timeinfo.tm_year, 
      this->timeinfo.tm_mon, 
      this->timeinfo.tm_mday, 
      this->timeinfo.tm_hour, 
      this->timeinfo.tm_min );

  this->screen.setTextDatum(TC_DATUM);
  this->screen.setTextFont(2); // Set the text font to font number 2
  this->screen.setTextSize(1);
  this->screen.setTextColor(TFT_WHITE, D_BACKGROUND); // Set the text color to white with black background

  // Draw a string at position (100, 100) on the screen
  this->screen.drawString( output, D_WIDTH/2, 124 );

  this->screen.setTextDatum(TL_DATUM); // Default.
  

}

int Display::updateBrightness( int amount )
{
  this->setBrightness( this->brightness + amount );
  return this->brightness;
}

int Display::setBrightness( int amount )
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

int Display::getBrightness( void )
{
  return this->brightness;
}


void Display::test( void )
{
  while ( 1 )
  {
    // Turn everything on...refresh, turn everyhting off, turn on normal operatoins and run timer.
    timer.start();

    this->setMessage( "Starting");

    // this->setIcon( D_ICON_BOILER, D_OFF );
    // this->setIcon( D_ICON_SOLINOID, D_OFF );
    // this->setIcon( D_ICON_PUMP, D_OFF );

    // this->setIcon( D_ICON_WATER, D_OFF );
    // this->setIcon( D_ICON_RESERVOIR, D_OFF );
    // delay( 2000 );

    // this->setIcon( D_ICON_BOILER, D_ON );
    // delay( 2000 );
    // this->setIcon( D_ICON_SOLINOID,  D_ON );
    // delay( 2000 );
    // this->setIcon( D_ICON_PUMP, D_ON );
    // delay( 2000 );

    // this->setIcon( D_ICON_WATER, D_ON );
    // delay( 2000 );
    // this->setIcon( D_ICON_RESERVOIR, D_ON );
    // delay( 2000 );

    this->setMessage( "message....");
    this->setStatus( "waiting for 2 seconds" );
    delay( 2000 );

    this->clearMessage( );
    this->setStatus( "another 2 seconds" );
    delay( 2000 );

    this->setStatus( "timer" );
    timer.stop();

    delay( 2000 );


  }


}




