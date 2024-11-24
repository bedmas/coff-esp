#ifndef BEDMAS_TIMER
#define BEDMAS_TIMER

class Timer {

  private:
    unsigned long startTime = 0;
    bool running = 0;

  public:
    Timer( void );
    void start( void );
    void stop( void );
    int seconds( void );
    bool isRunning( void );

    void dump( void );

};

#endif