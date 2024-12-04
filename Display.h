#ifndef BEDMAS_DISPLAY
#define BEDMAS_DISPLAY

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

#include "Timer.h"
#include "Monitor.h"
#include "Communications.h"


#define D_HEIGHT 160
#define D_WIDTH 128

#define D_MAX_BRIGHTNESS 255

#define D_BACKGROUND TFT_NAVY    // Why is red actally blue? Good Question.....
#define D_TITLE_COLOUR TFT_GOLD
#define D_MESSAGE_COLOUR TFT_WHITE
#define D_STATUS_COLOUR TFT_WHITE
#define D_ICON_COLOUR TFT_WHITE
#define D_ICON_OFF TFT_DARKGREY
#define D_ICON_ON TFT_WHITE

#define D_FONT_DIGITAL 7

#define D_ON 1
#define D_OFF 0

#define D_PIN_LED 33

// Flag the updates....
#define D_UPDATE_ALL     0b11111111
#define D_UPDATE_TITLE   0b00000001
#define D_UPDATE_MESSAGE 0b00000010
#define D_UPDATE_STATUS  0b00000100
#define D_UPDATE_ICONS   0b00001000


class Display {

  private:
    TFT_eSPI screen = TFT_eSPI();
    int brightness = 128;

    int last_timer = 0;

    String message = "";
    String title = "";
    String status = "";

    uint program = 0;
    
    int last_min = 0;

    int icons = 0b0000000000000000;    // Everything off.
    /* uint8_t boiler_icon = 0;
    uint8_t solinoid_icon = 0;
    uint8_t pump_icon = 0;

    uint8_t water_icon = 0;
    uint8_t reservoir_icon = 0;
    */
    struct tm timeinfo;

  public: 

    int update = D_UPDATE_ALL;   // 0 dont update, 1 everything, 2 update status, 3 update message, 4 update icons

    Display( void );
    Display( String );
    void init( void );
    void test( void );
    void refresh( void );
   
    void setTitle( String );
    void setMessage( String );
    void setStatus( String );
    void setProgram( uint );
    void setUpdate( int );
    // void setIcon( int, uint8_t );

    // void boilerIcon( uint8_t );
    // void solinoidIcon( uint8_t );
    // void pumpIcon( uint8_t );

    // void waterIcon( uint8_t );
    // void reservoirIcon( uint8_t );

    void drawTitle( void );
    void drawMessage( void );
    void drawStatus( void );
    void drawTimer( void );
    void drawDateTime( void );

    void clearMessage( void );
    void clearStatus( void );

    void drawIcons();
    void drawIcon( uint8_t, String, uint8_t );

    int updateBrightness( int );
    int setBrightness( int );
    int getBrightness( void );
    

};

#endif