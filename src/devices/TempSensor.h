#ifndef __TEMP_SENSOR__
#define __TEMP_SENSOR__

class TempSensor {

public:
  virtual float getTemperature() = 0;
  virtual void setTemperature(float Temp) = 0;
};


#endif

