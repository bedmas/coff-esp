#include <Arduino.h>

/*

  Nice thing about the ESP is that I can run separate tasks independently and on two cores.

*/

#include "Display.h"
#include "Monitor.h"
#include "Communications.h"

void test_display_task( void* pvParameters );
void monitor_run_task( void* pvParameters );
void brew_run_task( void* pvParameters );

void launch_monitor( void );
void launch_brewer( void );


Display display;
Monitor monitor;
Timer timer;
Brew brew;
Communications coms;

TaskHandle_t monitor_task;
TaskHandle_t brew_task;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println( "Start" );
  Serial.print("Loop running on core ");
  Serial.println(xPortGetCoreID());

  coms.init();

  display.init();
  display.setTitle( "Coff-ESP" );

//  test_display();  // Lauch a task to update test out the display.

  launch_monitor();
//   launch_brewer();

  delay( 1000 );
  
  
}
 

void loop() 
{
  // Update the display,
  display.refresh();

  coms.monitor();
  
  delay (20);

}

void test_display_task( void* pvParameters ) { display.test(); }
void monitor_run_task( void* pvParameters ) { monitor.run(); }
void brew_run_task( void* pvParameters ) { brew.run(); }

void test_display()
{

  xTaskCreatePinnedToCore(
    test_display_task,   /* Task function. */
    "Test Display Task",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    NULL,    /* Task handle to keep track of created task */
    0 );          /* pin task to core 0 */   

}

void launch_monitor( void )
{
  monitor.init();
  xTaskCreatePinnedToCore(
    monitor_run_task,   /* Task function. */
    "Monitor Sensors",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &monitor_task,    /* Task handle to keep track of created task */
    0 );          /* pin task to core 0 */   

}

void launch_brewer()
{
  brew.init();
  xTaskCreatePinnedToCore(
    brew_run_task,   /* Task function. */
    "Brew Sensors",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &brew_task,   /* Task handle to keep track of created task */
    1 );          /* pin task to core 0 */   

}







