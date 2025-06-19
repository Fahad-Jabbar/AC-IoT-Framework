#include <Arduino.h>
#include <assert.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
//#include <WiFi.h>
//#include <WiFiUdp.h>
//#include <NTPClient.h>
//#include <HTTPClient.h>
//#include <ArduinoJson.h>
#include <EEPROM.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
//const long utcOffsetInSeconds = 5*3600; // UTC+1 hour (3600 seconds)
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
//
//#include <BLEDevice.h>
//#include <BLEServer.h>
//#include <BLEUtils.h>
//#include <BLE2902.h>
//
//BLEServer* pServer = NULL;
//BLECharacteristic* pCharacteristic = NULL;
//bool deviceConnected = false;
//bool oldDeviceConnected = false;
//String receivedMessage = ""; // Variable to store the received message
//
//class MyServerCallbacks : public BLEServerCallbacks {
//    void onConnect(BLEServer* pServer) {
//      deviceConnected = true;
//    };
//
//    void onDisconnect(BLEServer* pServer) {
//      deviceConnected = false;
//    }
//};
//
//class MyCallbacks : public BLECharacteristicCallbacks {
//    void onWrite(BLECharacteristic* pCharacteristic) {
//      std::string value = pCharacteristic->getValue();
//      if (value.length() > 0) {
//        receivedMessage = String(value.c_str());
//      }
//    }
//};


const char* ssid;
const char* password;
String ssid_str;
String pass_str;
int reset_btn = 0; 
int rst = 0;
String time_str;
String dateString_str;
String status_str;
String diff;
String entry_time = "09:00:00";
#define led 14


//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

// GPIO where the DS18B20 is connected to
const int oneWireBus_Internal = 25;     
const int oneWireBus_External = 27;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire1(oneWireBus_Internal);
OneWire oneWire2(oneWireBus_External);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature ext_temp(&oneWire1);
DallasTemperature int_temp(&oneWire2);

//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN

int EEPROM_limit = 4095;
int rst_pin=13;
String save_str, device_id;
int id_pos=10;
String pub_str;
int time_ch=0;
int time_check=2;
float Temp_internal;
float Temp_external;
//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN


char *buttons_p[] = {"Power-Off", "Power-On", "temperature to 16", "temperature to 17", "temperature to 18", "temperature to 19", "temperature to 20", "temperature to 21", "temperature to 22", "temperature to 23", "temperature to 24", "temperature to 25", "temperature to 26", "temperature to 27", "temperature to 28"};
int pos_lenght[] = {200,400,600,800,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000};

decode_type_t protocol_abc;
decode_type_t protocol;

uint16_t size;
uint16_t read_size;
uint8_t read_state;
int butt=0;
uint8_t arrayFromEEPROM[100];

#ifdef ARDUINO_ESP32C3_DEV
const uint16_t kRecvPin = 35;  // 14 on a ESP32-C3 causes a boot loop.
#else  // ARDUINO_ESP32C3_DEV
const uint16_t kRecvPin = 35;
#endif  // ARDUINO_ESP32C3_DEV

////////////////////////// IR Settings /////////////////////////////////////
const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;
const uint16_t kFrequency = 38000;  // in Hz. e.g. 38kHz.

#if DECODE_AC
  const uint8_t kTimeout = 50;
#else   // DECODE_AC
  const uint8_t kTimeout = 15;
#endif  // DECODE_AC

const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;  // kTolerance is normally 25%

#define LEGACY_TIMING_INFO false

/////////// ==================== end of TUNEABLE PARAMETERS ====================/////////////////////////

// Use turn on the save buffer feature for more complete capture coverage.
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;  // Somewhere to store the results

const uint16_t kIrLedPin = 32;
IRsend irsend(kIrLedPin);

int Mode_ch, temp_max, temp_min, t_on_hr, t_on_min, t_off_hr, t_off_min; 
int mode_ch=0;
int mode_on=0;
int hr_check, min_check; 
int old_mode = 0;
int old_temp_max = 0;
int old_temp_min = 0;
int thresh = 0;
int old_thresh = 0;
int old_t_on_hr = 0;
int old_t_on_min = 0;
int old_t_off_hr = 0;
int old_t_off_min = 0;
int red_pin = 22;
int blue_pin = 21;
int no_of_blinks = 5;
int blinking_speed = 20;

String AC_val, In_temp, Out_temp, max_temp, min_temp, on_time, off_time, mode_val, thr_val, st, str_date, str_time;
String max_temp_old, min_temp_old, on_time_old, off_time_old, mode_val_old, thr_val_old;
int change=0;
int t_mins, t_on_mins, t_off_mins, temp_set, ac_on_off, pos_set;


void setup() {
  Serial.begin(115200);
  while (!Serial)  // Wait for the serial connection to be establised.
  delay(50);
  pinMode(red_pin,OUTPUT);
  pinMode(blue_pin,OUTPUT);
  led_blinking(red_pin, no_of_blinks, blinking_speed); ////////////////////////////////////////////// Red led indication
  led_blinking(blue_pin, no_of_blinks, blinking_speed); ////////////////////////////////////////////// Red led indication
  
  EEPROM.begin(EEPROM_limit);
  Serial.println("Device Initialize....");
  remote_setup();
  delay(2000);
//  EEPROM.write(1,0);EEPROM.commit();
  Serial.print("EEPROM.read(1) : ");Serial.println(EEPROM.read(1));
  if (EEPROM.read(1)!=1){
    Serial.println("The device started, in BLE Mode!");
    ble_setup();
  }
  else{
    Serial.println("The device started, in Wi-Fi Mode!");
//    // Start the DS18B20 sensor
    int_temp.begin();
    ext_temp.begin();

    SD_Card_Setup();
    
    wifi_setup();
//    wifi_mode_set();
    // Initialize NTP client
    timeClient.begin();
//    digitalWrite(led,0); 
    wifi_api();
  }
  pinMode(rst_pin,INPUT);
}

int ch=0;

int aa = millis()/1000;

void loop() {
  // put your main code here, to run repeatedly:
  if (EEPROM.read(1)!=1){
//      Serial.println("The device started, in BLE Mode!");
      BLE_Loop();
      if (pos_set==1){
        Serial.println("Turn AC ON");
        send_command(400); //// set AC to ON
        delay(500);
      }
    }
  else{
    wifi_check();
    fetchAPI();
    get_time();   //// get time from NTP
    temperature_loop();
    detect_change();
    rst_fcn();
    int b=millis()/1000;
    if ((b-aa)>1500){ //   API hit after every hour (3600)
      fetchAPI();
      get_time();   //// get time from NTP
      temperature_loop();
      detect_change();
      rst_fcn();
      hit_api();
      aa = millis()/1000;
    }
    delay(10000);
  }
  
  //  check time for data posting
}
