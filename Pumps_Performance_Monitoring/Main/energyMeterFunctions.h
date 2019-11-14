#include <Arduino.h>
#include "esp32ModbusRTU.h"

// modbus register addresses
#define CURRENT_ADDRESS 148
#define VOLTAGE_ADDRESS 140
#define POWER_ADDRESS 100

#define PIN_25 25
#define PIN_26 26
#define PIN_27 27

String energyBuff = "";
// use Serial1 and pin 16 as RTS
esp32ModbusRTU modbus(&Serial1, PIN_25);

/*
  SUCCESS               = 0x00,
  ILLEGAL_FUNCTION      = 0x01,
  ILLEGAL_DATA_ADDRESS  = 0x02,
  ILLEGAL_DATA_VALUE    = 0x03,
  SERVER_DEVICE_FAILURE = 0x04,
  ACKNOWLEDGE           = 0x05,
  SERVER_DEVICE_BUSY    = 0x06,
  NEGATIVE_ACKNOWLEDGE  = 0x07,
  MEMORY_PARITY_ERROR   = 0x08,
  TIMEOUT               = 0xE0,
  INVALID_SLAVE         = 0xE1,
  INVALID_FUNCTION      = 0xE2,
  CRC_ERROR             = 0xE3,  // only for Modbus-RTU
  COMM_ERROR            = 0xE4   // general communication error
*/

String energyMeasure() {

    // modbus.readHoldingRegisters stores values in energyBuff
    String retstr = "";

    retstr += "Current: ";
    modbus.readHoldingRegisters(0x01, CURRENT_ADDRESS, 2);
    delay(1000);
    retstr += energyBuff + "A\n";
    pushCurrent = energyBuff;
    energyBuff = "";
    
    retstr += "Voltage: ";
    modbus.readHoldingRegisters(0x01, VOLTAGE_ADDRESS, 2); 
    delay(1000);
    retstr += energyBuff + "V\n";
    pushVoltage = energyBuff;
    energyBuff = "";
    
    retstr += "Power: ";
    modbus.readHoldingRegisters(0x01, POWER_ADDRESS, 2); 
    delay(1000);
    retstr += energyBuff + "W\n";
    pushPower = energyBuff;

    energyBuff = "";
    return retstr;
}
