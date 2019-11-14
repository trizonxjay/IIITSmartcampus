#include <Wire.h>;

// sda 21
// scl 22

#define si7021Addr 0x40
String tempStr;

void tempMeasure()
{
  unsigned int data[2];
  
  Wire.beginTransmission(si7021Addr);
  //Send humidity measurement command
  Wire.write(0xF5);
  Wire.endTransmission();
  delay(500);
    
  // Request 2 bytes of data
  Wire.requestFrom(si7021Addr, 2);
  // Read 2 bytes of data to get humidity
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
    
  // Convert the data
  float humidity  = ((data[0] * 256.0) + data[1]);
  humidity = ((125 * humidity) / 65536.0) - 6;

  Wire.beginTransmission(si7021Addr);
  // Send temperature measurement command
  Wire.write(0xF3);
  Wire.endTransmission();
  delay(500);
    
  // Request 2 bytes of data
  Wire.requestFrom(si7021Addr, 2);
  
  // Read 2 bytes of data for temperature
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data
  float temp  = ((data[0] * 256.0) + data[1]);
  float celsTemp = ((175.72 * temp) / 65536.0) - 46.85;
  float fahrTemp = celsTemp * 1.8 + 32;

  tempStr = "Humidity : " + (String)humidity + " % RH" + "\n";
  tempStr += "Celsius : " + (String)celsTemp + " C" + "\n" + "Fahrenheit : " + (String)fahrTemp + " F" + "\n";
  pushRH = (String)humidity;
  pushTemp = (String)celsTemp;
}