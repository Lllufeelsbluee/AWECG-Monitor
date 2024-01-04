#include <Arduino.h>
//#include <BLEDevice.h>
//#include <BLEServer.h>
//#include <BLEUtils.h>
//#include <BLE2902.h>
#include <Wire.h>
#include <driver/i2s.h>
#include "esp_adc_cal.h"
#include <string.h>

void sendData();
void intToChar(int data, char *str, int size);
String joinData(int vals[], int size);

// BLE server name
//#define bleServerName "AWECG Monitor"

#define I2S_SAMPLE_RATE 2500
#define ADC_INPUT ADC1_CHANNEL_4 // pin 32

#define bufferLength 5
#define bufferCount 2

int data[bufferCount][bufferLength];

bool dataReady = false;
int countRow = 0;
int countBuffer = 0;
int dataRead = 0;

// Timer variables
//unsigned long lastTime = 0;
//unsigned long timerDelay = 3;

//int pinShutdown = 14;
//int sleepPin = 12;

//bool deviceConnected = false;
//bool goSleep = false;

// create timer
hw_timer_t *timer = NULL;
//hw_timer_t *timer = NULL;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
//#define SERVICE_UUID "832a0638-67db-11ed-9022-0242ac120002"

//#define CHARACTERISTIC_UUID "832a0639-67db-11ed-9022-0242ac120002"

//#define CHARACTERISTIC_Battery_Level BLEUUID((uint16_t)0x2A19)

//#define CHARACTERISTIC_Heartrate_Voltage BLEUUID((uint16_t)0x2B18)

//BLECharacteristic ecgMonitor(CHARACTERISTIC_Heartrate_Voltage, BLECharacteristic::PROPERTY_NOTIFY);
//BLEDescriptor ecgDescriptor(BLEUUID((uint16_t)0x2902));

// BLECharacteristic batteryLevel(CHARACTERISTIC_Battery_Level, BLECharacteristic::PROPERTY_READ);
// BLEDescriptor batteryDescriptor(BLEUUID((uint16_t)0x2902));

// // Setup callbacks onConnect and onDisconnect
// class MyServerCallbacks : public BLEServerCallbacks
// {
//   void onConnect(BLEServer *pServer)
//   {
//     digitalWrite(pinShutdown, HIGH);
//     //Serial.println("BLE Client connected");
//     dataReady = false;
//     dataRead = 0;
//     timerAlarmEnable(timer);
//     //  Serial.println(CHARACTERISTIC_Heartrate_Voltage.toString().c_str());
//     deviceConnected = true;
//     timerAlarmDisable(timer1);
//   };
//   void onDisconnect(BLEServer *pServer)
//   {
//     digitalWrite(pinShutdown, LOW);
//     //Serial.println("BLE Client disconnected");
//     timerAlarmDisable(timer);
//     // start the timer to go to sleep after 30 seconds
//     timerAlarmWrite(timer1, 30000000, false);
//     timerAlarmEnable(timer1);
//     dataReady = false;
//     dataRead = 0;
//     deviceConnected = false;
//     pServer->getAdvertising()->start();
//   }
// };

void IRAM_ATTR onTimer()
{
  // Serial.printf("timer interrupt");
  /*size_t bytes_read;
  uint16_t buffer[1] = {0};
  i2s_read(I2S_NUM_0, &buffer, sizeof(buffer), &bytes_read, 1);
  sendData();*/

  data[countBuffer][countRow] = adc1_get_raw(ADC_INPUT);
  countRow++;
  if (countRow == bufferLength)
  {
    countRow = 0;
    countBuffer++;
    dataRead = countBuffer;
    
    dataReady = true;
    
     //dataRead =0;
    if (countBuffer == bufferCount)
    {
      countBuffer = 0;
       //dataRead=1;
    }
  }

   //
   Serial.println(millis());
   
}

//void IRAM_ATTR goToSleep()
//{
 //goSleep = true;
//}

void setup()
{

  //pinMode(pinShutdown, OUTPUT);

  //digitalWrite(pinShutdown, LOW);

  // set up the sleep pin to wake up the device from sleep
  //pinMode(sleepPin, INPUT);
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 1);

  // set up the timer to go to sleep after 30 seconds
  // timer1 = timerBegin(1, 80, true);
  // timerAttachInterrupt(timer1, &goToSleep, true);
  // timerAlarmWrite(timer1, 30000000, false);
  // timerAlarmEnable(timer1);

  Serial.begin(115200);
  
  // Create the BLE Device
  //BLEDevice::init(bleServerName);

  // set the mtu to 512
  // BLEDevice::setMTU(512);

  // Create the BLE Server
  //BLEServer *pServer = BLEDevice::createServer();
  //pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  //BLEService *ecgService = pServer->createService(BLEUUID(SERVICE_UUID), 256);

  // Create BLE Characteristics and Create a BLE Descriptor

  //ecgService->addCharacteristic(&ecgMonitor);
  // ecgDescriptor.setValue("ECG Monitor Data");
 //ecgMonitor.addDescriptor(&ecgDescriptor);

 // ecgService->addCharacteristic(&batteryLevel);
  // ecgDescriptor.setValue("ECG Monitor Data");
  //batteryLevel.addDescriptor(&batteryDescriptor);

  // Start the service
  //ecgService->start();

  // Start advertising
  //BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  //pAdvertising->addServiceUUID(SERVICE_UUID);
  //pServer->getAdvertising()->start();
  // i2sInit();

  // setting adc to read from pin 32 with 12 bits of resolution
  adc1_config_width(ADC_WIDTH_BIT_12 );
  adc1_config_channel_atten(ADC_INPUT, ADC_ATTEN_DB_0);
  
  // create a interrupt each 0.5s
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 500000 , true);
  timerAlarmEnable(timer);
  
}

// create varianble to save the millis of the last time the data was sent
unsigned long lastTimeDataSent = 0;

void loop()
{
  
  // if (goSleep)
  // {
  //   if (!deviceConnected)
  //   {
  //     //Serial.println("Going to sleep");
  //     esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 1);
  //     esp_deep_sleep_start();
  //   }
  //   goSleep = false;
  // }
  
  //IRAM_ATTR goToSleep();
      if(dataReady)
      {

     
    lastTimeDataSent = millis();
    
    Serial.printf("timer interrupt %d \t", lastTimeDataSent);
    sendData();
    dataReady = false;
    
    Serial.printf("timer end interrupt %d \n", millis());
    
     }
    // data[count] = dataRead;
    // count++;

    // Serial.println(lastTimeDataSent);
    /*if(count == 2){
      count = 0;
      sendData();
    }*/
    // sendData(dataRead);
  
  /*data = analogRead(32);
  data = float(data * (3.3 / 4095.0));
    // remove the offset voltage
    // data = data > 1 ? data - 1.65 : data;
    data = float(data - float(1.65));

    data = float(data / 1.1);
    // convert float to string with 18 decimal places
    char str[23];
    dtostrf(data, -21, 18, str);
    ecgMonitor.setValue(str);*/
  /*if (deviceConnected) {
    if ((millis() - lastTime) >= timerDelay) {
      Serial.println(millis());
      //Notify temperature reading from BME sensor
      //Set temperature Characteristic value and notify connected client
      // convert float to string
      data = analogRead(32);

      // get the voltage reading
      data = float(data * (3.3 / 4095.0) );
      // remove the offset voltage
      //data = data > 1 ? data - 1.65 : data;
      data = float(data - float(1.65));

      data = float(data / 1.1);
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
  }*/
}


void intToChar(int data, char *str, int size)
{
  int i = 0;

  while (i < size)
  {
    str[i] = (data % 10) + '0';
    data = data / 10;
    i++;
  }
  // invert the string
  for (int j = 0; j < i / 2; j++)
  {
    char temp = str[j];
    str[j] = str[i - j - 1];
    str[i - j - 1] = temp;
  }
}

// create join function to join an array of char
String joinData(int vals[bufferLength], int size)
{
  String str = "";
  for (int i = 0; i < size; i++)
  {
    str += String(vals[i]) + ",";
  }
  return str;
}
void sendData()
{

 
  
    // Serial.println(millis());
    //  ecgMonitor.notify();
    //  Serial.println(millis());
    //  data = analogRead(32);

    // get the voltage reading
    // data = float(dataIn * (3.3 / 4095.0));
    // remove the offset voltage
    // data = data > 1 ? data - 1.65 : data;
    // data = float(data - float(1.65));

    // data = float(data / 1.1);
    //  convert float to string with 18 decimal places
    // char str[4];
    // char str2[4];
    // char str3[9];
    //  convert int to string
    // intToChar(data[0], str, 4);
    // intToChar(data[1], str2, 4);

    String dataString = joinData(data[dataRead], bufferLength);
    //Serial.printf("%s \n",dataString);
    Serial.printf("ADC value: %d \t",data[countBuffer][countRow]);
    /*str3[6] = str2[0];
    str3[7] = str2[1];
    str3[8] = str2[2];
    str3[9] = str2[3];
    str3[10] = str2[4];*/

    // dtostrf(data[0], -21, 18, str);
    // dtostrf(data[1], -21, 18, str);

    // Serial.println(str);
    // unir los dos datos
    // strcat(str, ",");
    // strcat(str, str2);
    // Serial.printf("%s \n", str3);
    //ecgMonitor.setValue(dataString.c_str());
    // ecgMonitor.setValue(str);
    //  String dataString = String(data);
    //  ecgMonitor.setValue(dataString.c_str());
    //   set battery leve in 100 %
    //  batteryLevel.setValue("200");
    //  batteryLevel.setValue(String(0x64).c_str());

    //ecgMonitor.notify();
    // Serial.println(millis());
  
}