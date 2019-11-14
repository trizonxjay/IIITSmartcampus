#define LED_BUILTIN 2
#define SENSOR  15

int countFlag = 0;
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 2.22;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
String flowStr;

// interrupting function to measure pulsecounts of flow meter
void IRAM_ATTR pulseCounter()
{
  if(countFlag == 1)
    pulseCount++;
}

int flowMeasure(int previousMillis)
{
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) 
  {
    pulse1Sec = pulseCount;
    pulseCount = 0;

    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;

    flowMilliLitres = (flowRate / 60) * 1000;

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
    
    flowStr = "Flow rate: " + (String)((float)(flowRate)) + "L/min" + "\t";
    pushFlow = (String)((float)(flowRate));
    flowStr += "Output Liquid Quantity: " + (String)totalMilliLitres + "mL / " + (String)(totalMilliLitres / 1000) + "L" + "\n";
    return 1;
  }
  return 0;
}
