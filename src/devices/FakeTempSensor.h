#ifndef __FAKE_TEMP_SENSOR__
#define __FAKE_TEMP_SENSOR__
#define FAKE_TEMP 100.0
#include "TempSensor.h"
class FakeTempSensor: public TempSensor {

public:
  FakeTempSensor(int pin){};	
  float getTemperature(){return FAKE_TEMP;}
  void setTemperature(float temp){temperature = temp;}
private:
  int pin;
  float temperature;
};


#endif