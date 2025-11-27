#ifndef __FAKE_SONAR__
#define __FAKE_SONAR__

#include "ProximitySensor.h"

#define NO_OBJ_DETECTED -1
#define FAKE_DISTANCE_SONAR 100.0
class FakeSonar: public ProximitySensor {

public:  
  FakeSonar(int echoPin, int trigPin, long maxTime)
  {};
  float getDistance()
  {  return FAKE_DISTANCE_SONAR;}
  void setTemperature(float temp){temperature = temp;}  

private:
    const float vs = 331.5 + 0.6*20;
    float getSoundSpeed(){return 60000.0;}
    
    float temperature;    
    int echoPin, trigPin;
    long timeOut;
};

#endif 