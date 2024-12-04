#include "Communications.h"
#include <cmath>

//#define DEBUG 1

extern Display display;
extern Brew brew;
extern Monitor monitor;
extern Timer timer;

Communications::Communications()
{

}

void Communications::init()
{
  this->SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  
  #ifdef DEBUG
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  #endif

}

void Communications::run()
{

  // Look for the date and time...
  // if ( !this->ntpTimeSet ) { this->ntpConnect(); }

  if ( this->SerialBT.available() ) {
    char read = this->SerialBT.read();
    // Serial.write( read );

    if ( read == COMMAND_END )
    {
      // Store the null. character at the end of the string.
      // this->SerialBT.read();    //End the string.  this should only be done when the command end is met but I want to see it every iteration when developing..
      read = this->SerialBT.read();
      this->command_string[ this->command_index] = '\0';
      this->runCommand( );   

    } else if ( read > 31 ) {
      this->command_string[ this->command_index++ ] = read;

    }
  }
}




// If avaliable sne a message...
void Communications::send( String message )
{
  if ( this->SerialBT.available() )
  {
    this->SerialBT.println( message );
  }
}



void Communications::runCommand( )
{

  switch ( this->command() )
  {
    case 'b':
      // Change the brightness;
      this->SerialBT.printf( "Brightness set to %d\n", display.setBrightness( this->command_value() ) );
      break;

    case 'p':
      // Change the program;
      this->SerialBT.printf( "Program set to %d\n", brew.setProgram( this->command_value() ) );
      break;

    case 't':
      // toggle the timer
      if ( timer.isRunning() ) { 
        timer.stop(); 
        this->SerialBT.printf( "Timer has been stopped\n" );
      }
      else { 
        timer.start(); 
        this->SerialBT.printf( "Start the timer\n" );

      }
      break;

    case 'h':
      // toggle the timer
      this->SerialBT.printf( "Help for Coff-esp\n\n" );
      this->SerialBT.printf( "  b<n> - brightness level to <n>\n" );
      this->SerialBT.printf( "  t - toggle the timer on off\n" );
      this->SerialBT.printf( "  h - this help.\n" );
      this->SerialBT.printf( "\n" );

      break;

    default:
        this->SerialBT.printf( "Unknown command %c\n", this->command() );
  }
  this->command_index = 0;
 

}

// Return the command character.
char Communications::command( void )
{
  return this->command_string[0];
}

// Return the command value
int Communications::command_value( void )
{
  char *value = &this->command_string[1];
  return String( value ).toInt();

}

void Communications::ntpConnect()
{

  const char* ssid               = "bedmas";
  const char* password           = "uncooldaddio42";
  const char* ntpServer          = "pool.ntp.org";
  const int   daylightOffset_sec =  3600;
  const long  gmtOffset_sec      = daylightOffset_sec * TIME_ZONE;

  // // Connect to Wi-Fi
  #ifdef DEBUG
  Serial.print("Connecting to ");
  Serial.println( ssid);
  // Serial.println( password);
  #endif

  Serial.print("[WiFi] Attemping to connect to WiFi: ");
  Serial.println(ssid);
  WiFi.begin( ssid, password);
  delay(1000);

  int count = 0;

  while ( 1 )
  {
    int status = WiFi.status();

    #ifdef DEBUG
    switch ( status ) {
      case WL_NO_SSID_AVAIL: Serial.println("[WiFi] SSID not found"); break;
      case WL_CONNECT_FAILED:
        Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
        break;
      case WL_CONNECTION_LOST: Serial.println("[WiFi] Connection was lost"); break;
      case WL_SCAN_COMPLETED:  Serial.println("[WiFi] Scan is completed"); break;
      case WL_DISCONNECTED:    Serial.println("[WiFi] WiFi is disconnected"); break;
      case WL_CONNECTED:
        // Serial.println("[WiFi] WiFi is connected!");
        // Serial.print("[WiFi] IP address: ");
        // Serial.println(WiFi.localIP());
        break;
      default:
        Serial.print("[WiFi] WiFi Status: ");
        Serial.println(WiFi.status());
        break;
    }
    #endif

    if ( status == WL_CONNECTED ) 
    { 
      Serial.println("[WiFi] WiFi is connected!");
      Serial.print("[WiFi] IP address: ");
      Serial.println(WiFi.localIP());

      configTime( gmtOffset_sec, daylightOffset_sec, ntpServer );

      // Once we have connected then stop trying....
      this->ntpTimeSet = 1;

      this->printLocalTime();

      // //disconnect WiFi as it's no longer needed
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      break;
    }

    if ( count++ > 10 ) { break; }
    delay(200);

  }


}

void Communications::printLocalTime()
{
  struct tm timeinfo;
  if( !getLocalTime(&timeinfo) ){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  delay(5000);
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}


