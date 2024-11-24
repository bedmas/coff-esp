#ifndef BEDMAS_BREW
#define BEDMAS_BREW

#include "Brew.h"
#include "Display.h"
#include "Monitor.h"

class Brew {

  private:
//     TaskHandle_t taskHandler;
    uint8_t running = 0;
    uint8_t program = 0;

  public: 
    Brew( );

    void init( void );
    void test( void );
    void run( void );

    void program0( void );
    void program1( void );

    void start( void );
    void stop( void );
 

    uint8_t setProgram( uint8_t );
    uint8_t getProgram(  );

  
};


#endif