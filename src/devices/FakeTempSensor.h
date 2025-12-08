#ifndef __FAKE_TEMP_SENSOR__
#define __FAKE_TEMP_SENSOR__
#define FAKE_TEMP 20.0
#include "TempSensor.h"
class FakeTempSensor: public TempSensor {

public:
  FakeTempSensor(int pin){temperature = FAKE_TEMP;}	
  float getTemperature(){return temperature;}
  void setTemperature(float temp){temperature = temp;}
private:
  int pin;
  float temperature;
};


#endif