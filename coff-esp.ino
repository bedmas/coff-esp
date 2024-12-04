/*

  Nice thing about the ESP is that I can run separate tasks independently and on two cores.

*/

// #define DEBUG

#include "Display.h"
#include "Monitor.h"
#include "Communications.h"

Display display;
Monitor monitor;
Timer timer;
Brew brew;
Communications coms;

TaskHandle_t monitor_task;
TaskHandle_t brew_task;

#ifdef DEBUG
uint count = 0;
#endif


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  #ifndef DEBUG
  Serial.println( "Start" );
  Serial.print( F("Loop running on core.") );
  Serial.println( xPortGetCoreID() );
  #endif

  // this interfers with the GPIO 2 and 4, which mess up the display.... will need to.
  coms.ntpConnect();
  coms.init();

  // display = new Display(  );
  display.setTitle( "Coff-ESP" );
  display.init();

  // // Set up the monitor
  monitor.init();
  launch_monitor();

  // // Set up the brew
  brew.init();
  launch_brewer();

  // Try and connect to the wifi
  // launch_wifi();

  delay( 1000 );
  
  
}
 

void loop() 
{

  // Update the display,
  display.refresh();
  coms.run();

  #ifdef DEBUG
  if ( count % 9064 == 0 )
  {
    /* Inspect our own high water mark on entering the task. */
    UBaseType_t uxHWMonitorMark = uxTaskGetStackHighWaterMark( monitor_task );
    UBaseType_t uxHWBrewMark = uxTaskGetStackHighWaterMark( brew_task );
    Serial.printf( "Monitor High water mark: %d      Brew High water mark: %d\n", uxHWMonitorMark, uxHWBrewMark );
  }  
  #endif


  delay (100);

}


#ifdef DEBUG
void test_display()
{
  xTaskCreatePinnedToCore(
    test_display_task,   /* Task function. */
    "Test Display Task",     /* name of task. */
    2048,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    NULL,    /* Task handle to keep track of created task */
    0 );          /* pin task to core 0 */   

}
void test_display_task( void* pvParameters ) { display.test(); }
#endif

/*
  launch the wifi 
*/
void launch_wifi()
{
  xTaskCreatePinnedToCore(
    wifi_display_task,   /* Task function. */
    "WiFi Task",     /* name of task. */
    1024,         /* Stack size of task */
    NULL,         /* parameter of the task */
    1,            /* priority of the task */
    NULL,         /* Task handle to keep track of created task */
    0 );          /* pin task to core 0 */   
}
void wifi_display_task( void* pvParameters ) { coms.ntpConnect(); }

/*
  launch the monitor 
*/
void launch_monitor()
{
  monitor.init();
  xTaskCreatePinnedToCore(
    monitor_run_task,   /* Task function. */
    "Monitor Sensors",     /* name of task. */
    4096,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &monitor_task,    /* Task handle to keep track of created task */
    0 );          /* pin task to core 0 */   

}
void monitor_run_task( void* pvParameters ) { monitor.run(); }

void launch_brewer()
{
  brew.init();
  xTaskCreatePinnedToCore(
    brew_run_task,   /* Task function. */
    "Brew Sensors",     /* name of task. */
    1024,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &brew_task,   /* Task handle to keep track of created task */
    1 );          /* pin task to core 0 */   

}
void brew_run_task( void* pvParameters ) { brew.run(); }


