#ifndef __FAKE_TEMP_SENSOR__
#define __FAKE_TEMP_SENSOR__
#define FAKE_TEMP 40.0
class FakeTempSensor: public TempSensor {

public:
  FakeTempSensor(int pin){};	
  float getTemperature(){return FAKE_TEMP;}
  
private:
  int pin;
};


#endif