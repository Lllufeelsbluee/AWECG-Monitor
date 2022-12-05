#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>


//BLE server name
#define bleServerName "AWECG Monitor"

float data;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 4;



bool deviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "832a0638-67db-11ed-9022-0242ac120002"

#define CHARACTERISTIC_UUID "832a0639-67db-11ed-9022-0242ac120002"

#define CHARACTERISTIC_Battery_Level BLEUUID((uint16_t)0x2A19)

#define CHARACTERISTIC_Heartrate_Voltage BLEUUID((uint16_t)0x2B18)


BLECharacteristic ecgMonitor(CHARACTERISTIC_Heartrate_Voltage, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor ecgDescriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic batteryLevel(CHARACTERISTIC_Battery_Level, BLECharacteristic::PROPERTY_READ);
BLEDescriptor batteryDescriptor(BLEUUID((uint16_t)0x2902));

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("BLE Client connected");
    //Serial.println(CHARACTERISTIC_Heartrate_Voltage.toString().c_str());
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    Serial.println("BLE Client disconnected");
    deviceConnected = false;
    pServer->getAdvertising()->start();
  }
};

void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init(bleServerName);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *ecgService = pServer->createService(BLEUUID(SERVICE_UUID), 256);

  // Create BLE Characteristics and Create a BLE Descriptor

  ecgService->addCharacteristic(&ecgMonitor);
  //ecgDescriptor.setValue("ECG Monitor Data");
  ecgMonitor.addDescriptor(&ecgDescriptor);

  ecgService->addCharacteristic(&batteryLevel);
  //ecgDescriptor.setValue("ECG Monitor Data");
  batteryLevel.addDescriptor(&batteryDescriptor);
  
  // Start the service
  ecgService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
}

void loop() {
  if (deviceConnected) {
    if ((millis() - lastTime) >= timerDelay) {
      //Notify temperature reading from BME sensor
      //Set temperature Characteristic value and notify connected client
      // convert float to string
      data = analogRead(32);
      
      // get the voltage reading
      data = float(data * (3.3 / 4095.0) - 1.65);
      
      data = float(data / 100.0);
      // convert float to string with 18 decimal places
      char str[23];
      dtostrf(data,-21, 18, str);
      //Serial.println(str);


      //Serial.println(str);
      ecgMonitor.setValue(str);
      //String dataString = String(data);
      //ecgMonitor.setValue(dataString.c_str());
      // set battery leve in 100 %
      batteryLevel.setValue("200");
      //batteryLevel.setValue(String(0x64).c_str());

      ecgMonitor.notify();   
      //Serial.println(ESP.getFreeHeap());
      lastTime = millis();
    }
  }
}