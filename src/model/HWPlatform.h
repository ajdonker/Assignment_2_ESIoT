#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__
#include "config.h"
#include "devices/Button.h"
#include "devices/ButtonImpl.h"
#include "devices/ServoMotor.h"
#include "devices/Led.h"
#include "devices/ServoMotorImpl.h"
#include "devices/Pir.h"
#include "devices/Sonar.h"
#include "devices/FakeTempSensor.h"
#include "devices/TempSensor.h"
#include "devices/Lcd.h"

class HWPlatform {

public:
  HWPlatform();
  void init(Lcd* lcd);
  void test();

  Button* getButton();
  Led*  getRedLed();
  Led* getGreen1Led();
  Led* getGreen2Led();
  ServoMotor* getMotor();
  Pir* getPir();
  Sonar* getSonar();
  TempSensor* getTempSensor();
  Lcd* getLcd();

private:
  Button *pButton;
  Led *pRedLed;
  Led *pGreen1Led;
  Led *pGreen2Led;
  Lcd *pLcd;
  ServoMotor *pMotor;
  Pir *pPir;
  Sonar *pSonar;
  FakeTempSensor *pTempSensor;
};
extern HWPlatform pHWPlatform;
#endif
