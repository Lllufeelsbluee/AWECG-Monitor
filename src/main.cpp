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
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() {

  // Create the BLE Device
  BLEDevice::init(bleServerName);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *ecgService = pServer->createService(SERVICE_UUID);

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
      data = analogRead(34);
      // get the voltage reading
      data = data * (3.3 / 4095.0) - 1.65;
      data = data / 1000.0;
      String dataString = String(data);
      ecgMonitor.setValue(dataString.c_str());
      batteryLevel.setValue(String(0x64).c_str());

      ecgMonitor.notify();   
      lastTime = millis();
    }
  }
}