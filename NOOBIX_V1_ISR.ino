/* Tutorial: https://electronoobs.com/eng_arduino_tut114.php
 * Schematic: https://electronoobs.com/eng_arduino_tut114_sch1.php
 * PHP files: https://electronoobs.com/eng_arduino_tut114_code1.php
 * Code: https://electronoobs.com/eng_arduino_tut114_code2.php 
 * Video: https://www.youtube.com/watch?v=QabYZiSKHC8 */ 

#include <avr/wdt.h>            // library for default watchdog functions
#include <avr/interrupt.h>      // library for interrupts handling
#include <avr/sleep.h>          // library for sleep
#include <avr/power.h>          // library for power control

#include <SoftwareSerial.h>
//#define _SS_MAX_RX_BUFF 256 
#define ESP8266_rxPin 6
#define ESP8266_txPin 7
#include <avr/power.h>

//////////////////////////CHANGE VALUES HERE//////////////////////////////
//SSID + KEY
const char SSID_ESP[] = "Cuelguesele a esta";   // WiFI name
const char SSID_KEY[] = "Duqueza2102+%";       //WiFi passowrd
const char* host = "electronoobs.com";    //Your domain without the "www"
String ESP8266_CONNECT = "AT+CIPSTART=0,\"TCP\",\"www.electronoobs.com\",80\r\n"; //Your domain with the "www" this time
String unit_id = "1";                     //The id you've placed on the table you've created on the database
String id_password = "12345";             //The password for that id
String url = "";
//////////////////////////////////////////////////////////////////////////


//MODES
const char CWMODE = '1';//CWMODE 1=STATION, 2=APMODE, 3=BOTH
const char CIPMUX = '1';//CWMODE 0=Single Connection, 1=Multiple Connections
SoftwareSerial ESP8266(ESP8266_rxPin, ESP8266_txPin);// rx tx

//DEFINE ALL FUNCTIONS HERE
boolean setup_ESP();
boolean read_until_ESP(const char keyword1[], int key_size, int timeout_val, byte mode);
void timeout_start();
boolean timeout_check(int timeout_ms);
void serial_dump_ESP();
boolean connect_ESP();
void connect_webhost();


int sensor_value = 0;
bool detected = false;

unsigned long timeout_start_val;
char scratch_data_from_ESP[20];//first byte is the length of bytes
char payload[200];
byte payload_size=0, counter=0;
char ip_address[16];
String URL_withPacket = "";    
String payload_closer = " HTTP/1.1\r\n\r\n";
unsigned long multiplier[] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};

//DEFINE KEYWORDS HERE
const char keyword_OK[] = "OK";
const char keyword_Ready[] = "Ready";
const char keyword_no_change[] = "no change";
const char keyword_blank[] = "#&";
const char keyword_ip[] = "192.";
const char keyword_rn[] = "\r\n";
const char keyword_quote[] = "\"";
const char keyword_carrot[] = ">";
const char keyword_sendok[] = "SEND OK";
const char keyword_linkdisc[] = "Unlink";
const char keyword_doublehash[] = "##";


///////////////////////////////////SETUP///////////////////////////////////////
void setup(){
  //Pin Modes for ESP TX/RX
  pinMode(ESP8266_rxPin, INPUT);
  pinMode(ESP8266_txPin, OUTPUT);  
  pinMode(12, OUTPUT);  
  pinMode(A0,INPUT);
  digitalWrite(12,LOW); 
  delay(100);
  digitalWrite(12,HIGH);
  
  ESP8266.begin(9600);    //Default baudrate for ESP, maybe you have one with 115200
  ESP8266.listen();       //Not needed unless using other software serial instances
  Serial.begin(9600);     //For status and debug  
  delay(2000);            //delay before kicking things off
   
  Serial.println("Deep sleep of ESP"); 
  ESP8266.print("AT\r\n");
  delay(3000); 
  ESP8266.print("AT+GSLP=0\r\n");  
  delay(3000);
  
  PCICR |= B00000010;    //enable port C o have interrupts                                                 
  PCMSK1 |= B00000001;  //Set pin A0 (button) trigger an interrupt on state change.

  Serial.println("Deep sleep ATMEGA"); 
  delay(1000);
  //We activate LOW power mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();  
}//SETUP END
///////////////////////////////////SETUP///////////////////////////////////////



void loop(){//         LOOP     START
  if(digitalRead(A0))
  {
    sensor_value = 1000;
    Serial.println("Sensor 1000");
  }
  else
  {
    sensor_value = 0;
    Serial.println("Sensor 0");    
  }
  
  digitalWrite(12,LOW); 
  delay(1000);
  digitalWrite(12,HIGH); 
  delay(100);
  setup_ESP();//go setup the ESP 
  delay(2000);

 
  connect_webhost();
  delay(2000);  

  Serial.println("Deep sleep of ESP"); 
  ESP8266.print("AT\r\n");
  delay(3000); 
  ESP8266.print("AT+GSLP=0\r\n");  
  delay(3000);    
  Serial.println("Deep sleep ATMEGA"); 
 delay(1000); 

  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
  

}//VOID LOOP END










ISR(PCINT1_vect)
{  
  sleep_disable();        
  Serial.println("Wake up");
}
