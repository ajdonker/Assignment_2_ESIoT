#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__
#include "config.h"
#include "devices/Button.h"
#include "devices/Led.h"
#include "devices/ServoMotor.h"
#include "devices/Pir.h"
#include "devices/Sonar.h"
#include "devices/TempSensor.h"

class HWPlatform {

public:
  HWPlatform();
  void init();
  void test();

  Button* getButton();
  Led*  getRedLed();
  Led* getGreen1Led();
  Led* getGreen2Led();
  ServoMotor* getMotor();
  Pir* getPir();
  Sonar* getSonar();
  TempSensor* getTempSensor();

private:
  Button* pButton;
  Led* pRedLed;
  Led* pGreen1Led;
  Led* pGreen2Led;
  ServoMotor* pMotor;
  Pir* pPir;
  Sonar* pSonar;
  TempSensor* pTempSensor;
};

#endif
