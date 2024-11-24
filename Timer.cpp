#include "Timer.h"
#include <Arduino.h>

Timer::Timer()
{
}

void Timer::start()
{
  this->startTime = micros();
  this->running = 1;
}

void Timer::stop()
{
  this->startTime = 0;
  this->running = 0;
}

bool Timer::isRunning() 
{
  return this->running;
}

int Timer::seconds()
{
  int seconds = 0;
  if ( this->startTime > 0 )
  {
    unsigned long now = micros();
    unsigned long difference = now - this->startTime;

    seconds = (float)difference / 1000000;
  
  }
  return seconds;
}

void Timer::dump()
{
  Serial.print( "Start: " );
  Serial.println( this->startTime );

  Serial.print( "Running: " );
  Serial.println( this->running );

}

