#ifndef BEDMAS_DISPLAY
#define BEDMAS_DISPLAY

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

#include "Timer.h"
#include "Monitor.h"


#define D_HEIGHT 160
#define D_WIDTH 128

#define D_MAX_BRIGHTNESS 255

#define D_BACKGROUND TFT_NAVY    // Why is red actally blue? Good Question.....

#define D_FONT_DIGITAL 7

#define D_ON 1
#define D_OFF 0

#define D_PIN_LED 33

class Display {

  private:
    TFT_eSPI screen = TFT_eSPI();
    int brightness = 128;
    int update = 2;   // 0 dont update, 1 update timer, 2 update everything.

    int last_timer = 0;

    String message = "";
    String title = "";
    String status = "";

    uint8_t program = 0;
    
    uint8_t boiler_icon = 0;
    uint8_t solinoid_icon = 0;
    uint8_t pump_icon = 0;

    uint8_t water_icon = 0;
    uint8_t reservoir_icon = 0;

  public: 
    Display( void );
    void init( void );
    void test( void );
    void refresh( void );
   
    void setTitle( String );
    void setMessage( String );
    void setStatus( String );
    void setProgram( uint8_t );

    void boilerIcon( uint8_t );
    void solinoidIcon( uint8_t );
    void pumpIcon( uint8_t );

    void waterIcon( uint8_t );
    void reservoirIcon( uint8_t );

    void drawTitle( void );
    void drawMessage( void );
    void drawStatus( void );
    void drawTimer( void );

    void drawIcons();
    void drawIcon( uint8_t, String, uint8_t );

    uint8_t updateBrightness( uint8_t );
    uint8_t setBrightness( uint8_t );
    uint8_t getBrightness( void );
    

};

#endif