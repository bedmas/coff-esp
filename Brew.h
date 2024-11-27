#ifndef BEDMAS_BREW
#define BEDMAS_BREW

#include "Brew.h"
#include "Display.h"
#include "Monitor.h"

class Brew {

  private:
//     TaskHandle_t taskHandler;
    uint8_t running = 0;
    int program = 0;


  public: 
    uint8_t program_running = 0;


    Brew( );

    void init( void );
    void test( void );
    void run( void );

    void program0( void );
    void program1( void );

    void start( void );
    void stop( void );
 

    int setProgram( int );
    int getProgram(  );

  
};


#endif