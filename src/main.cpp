#include <Arduino.h>
// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>
 #include <Wire.h>
 #include <driver/i2s.h>
 #include "esp_adc_cal.h"
 #include <string.h>

//void sendData();
// void intToChar(int data, char *str, int size);
// String joinData(int vals[], int size);

// BLE server name
// #define bleServerName "AWECG Monitor"

// #define I2S_SAMPLE_RATE 2500
#define ADC_INPUT ADC1_CHANNEL_4 // pin 32

int data;

 

// bool dataReady = false;

 
// // Timer variables
// unsigned long lastTime = 0;
// unsigned long timerDelay = 3;

// int pinShutdown = 14;
// int sleepPin = 12;

// //bool deviceConnected = false;
// //bool goSleep = false;

// // create timer
// hw_timer_t *timer = NULL;
// hw_timer_t *timer1 = NULL;

// // See the following for generating UUIDs:
// // https://www.uuidgenerator.net/

// #define SERVICE_UUID "832a0638-67db-11ed-9022-0242ac120002"

// #define CHARACTERISTIC_UUID "832a0639-67db-11ed-9022-0242ac120002"

// #define CHARACTERISTIC_Battery_Level BLEUUID((uint16_t)0x2A19)

// #define CHARACTERISTIC_Heartrate_Voltage BLEUUID((uint16_t)0x2B18)
/*
BLECharacteristic ecgMonitor(CHARACTERISTIC_Heartrate_Voltage, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor ecgDescriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic batteryLevel(CHARACTERISTIC_Battery_Level, BLECharacteristic::PROPERTY_READ);
BLEDescriptor batteryDescriptor(BLEUUID((uint16_t)0x2902));

// Setup callbacks onConnect and onDisconnect

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    digitalWrite(pinShutdown, HIGH);
    //Serial.println("BLE Client connected");
    dataReady = false;
    dataRead = 0;
    timerAlarmEnable(timer);
    //  Serial.println(CHARACTERISTIC_Heartrate_Voltage.toString().c_str());
    deviceConnected = true;
    timerAlarmDisable(timer1);
  };
  void onDisconnect(BLEServer *pServer)
  {
    digitalWrite(pinShutdown, LOW);
    //Serial.println("BLE Client disconnected");
    timerAlarmDisable(timer);
    // start the timer to go to sleep after 30 seconds
    timerAlarmWrite(timer1, 30000000, false);
    timerAlarmEnable(timer1);
    dataReady = false;
    dataRead = 0;
    deviceConnected = false;
    pServer->getAdvertising()->start();
  }
};
*/


// void IRAM_ATTR goToSleep()
// {
//   goSleep = true;
// }

void setup()
{
    
  Serial.begin(115200);
  
}
void loop()
{
   data = adc1_get_raw(ADC_INPUT);
  
     
     Serial.print("data: ");
     Serial.println(data);
     delay(1000);
}



// create join function to join an array of char
