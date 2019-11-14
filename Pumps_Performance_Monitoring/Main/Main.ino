#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

//  =======================================================================================================

// declaring buffers to store push values for onem2m
String pushCurrent = "";
String pushVoltage = "";
String pushPower = "";
String pushTemp = "";
String pushRH = "";
String pushFlow = "";

//  ---------------------------------------------- Energy Meter -------------------------------------------
#include "energyMeterFunctions.h"
//  ---------------------------------------------- Temp sensor --------------------------------------------
#include "temperatureFunctions.h"
//  ---------------------------------------------- Flow meter --------------------------------------------- 
#include "flowMeterFunctions.h"

//  =======================================================================================================

// declaring wifi ssid and wifi password as strings
char* wifi_ssid = "esw-m19@iiith";
char* wifi_pwd = "e5W-eMai@3!20hOct";
char* wifi_ssid2="";
char* wifi_pwd2="";

int ledFlag=0;
int LED_R = 4;
int LED_G = 5;

//  ------------------------------------- One M2M and Wifi Connect ----------------------------------------
#include "onem2mFunctions.h"
#include "connectToWifi.h"
//  =======================================================================================================

void setup()
{
  Serial.begin(9600);
  ledUpdate();
  connect_to_WIFI();

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G,OUTPUT);

//  ---------------------------------------------- Energy Meter --------------------------------------------

  Serial1.begin(9600, SERIAL_8E1, PIN_26, PIN_27);  // Modbus connection

  modbus.onData([](uint8_t serverAddress, esp32Modbus::FunctionCode fc, uint8_t* data, size_t length) {
 
    uint8_t data2[4];
    data2[0] = data[1];
    data2[1] = data[0];
    data2[2] = data[3];
    data2[3] = data[2];

    energyBuff = (String)(*reinterpret_cast<float*>(data2));
  });
  
  modbus.onError([](esp32Modbus::Error error) {
    Serial.printf("Got error: 0x%02x\n\n", static_cast<uint8_t>(error));
  });
  modbus.begin();

//  ---------------------------------------------- Flow meter --------------------------------------------- 

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);

//  ---------------------------------------------- Temp sensor --------------------------------------------

  Wire.begin();
  Wire.beginTransmission(si7021Addr);
  Wire.endTransmission();
  delay(300);
  
//------------------------------
  Serial.println("Setup done");
}

//  =======================================================================================================

void loop()
{

  // check whether wifi is connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection lost.. trying to reconnect");
    ledFlag=0; ledUpdate();
    connect_to_WIFI();
  }

  // initializing all buffers as empty
  pushCurrent = "";
  pushVoltage = "";
  pushPower = "";
  pushTemp = "";
  pushRH = "";
  pushFlow = "";

  // measure temperature
  tempMeasure();

  previousMillis = millis();
  countFlag = 1;

  // measure flow
  while(!flowMeasure(previousMillis));
  countFlag = 0;

  String energyStr = energyMeasure();

  // printing respective values stored in buffers
  Serial.println(tempStr);
  
  Serial.print("Flow: ");
  Serial.print(pushFlow);
  Serial.println("L/min");
  
  Serial.print("Current: ");
  Serial.println(pushCurrent);
  Serial.print("Voltage: ");
  Serial.println(pushVoltage);
  Serial.print("Power: ");
  Serial.println(pushPower);
  
  // Send data to OneM2M server (to the respective AE/containers)
  pushMyData("em/em_1_vll_avg", pushVoltage);
  pushMyData("fm/fm_1_pump_flowrate", pushFlow);
  pushMyData("oe/oe_1_temperature", pushTemp);
  pushMyData("oe/oe_1_rh", pushRH);
  pushMyData("em/em_1_watts_total", pushPower);
  pushMyData("em/em_1_current_total", pushCurrent);

  delay(600000);
}
