#include "HWPlatform.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "devices/Led.h"
#include "devices/LightSensorImpl.h"
#include "devices/ServoMotorImpl.h"
#include "kernel/Logger.h"
#include "devices/TempSensorTMP36.h"
#include "devices/FakeSonar.h"
#include "devices/FakeTempSensor.h"
void wakeUp(){}

HWPlatform::HWPlatform(){
  pButton = new ButtonImpl(BT_PIN);
  pRedLed = new Led(RED_PIN);
  pGreen1Led = new Led(GREEN1_PIN);
  pGreen2Led = new Led(GREEN2_PIN);
  pMotor = new ServoMotorImpl(MOTOR_PIN);
  pPir = new Pir(PIR_PIN);
  pSonar = new FakeSonar(SONAR_ECHO_PIN,SONAR_TRIG_PIN,SONAR_TIMEOUT_TIME);
  pTempSensor = new FakeTempSensor(TEMP_PIN);
  pLcd = new Lcd(0x27,20,4);
}


void HWPlatform::init(){
}

Button* HWPlatform::getButton(){
  return this->pButton;
}


Led*  HWPlatform::getRedLed(){
  return this->pRedLed;
}
Led*  HWPlatform::getGreen1Led(){
  return this->pGreen1Led;
}
Led*  HWPlatform::getGreen2Led(){
  return this->pGreen2Led;
}
ServoMotor* HWPlatform::getMotor(){
  return this->pMotor;
}
Pir* HWPlatform::getPir(){
  return this->pPir;
}
Sonar* HWPlatform::getSonar(){
  return this->pSonar;
}
TempSensor* HWPlatform::getTempSensor(){
  return this->pTempSensor;
}
Lcd* HWPlatform::getLcd(){
  return this->pLcd;
}
void HWPlatform::test(){
  bool btPressed = pButton->isPressed();
  pRedLed->switchOn();
  pMotor->on();
  pMotor->setPosition(90);
  Logger.log("Button: " + String(btPressed ? "pressed" : " not pressed"));
  Logger.log("Sonar readout: " + String(pSonar->getDistance()));
  Logger.log("Temp Sensor readout " + String(pTempSensor->getTemperature()));
  Logger.log("PIR readout:" + String(pPir->isDetected()));
  pLcd->printAt(2,2,"HELLO");
  delay(1000);
  pMotor->setPosition(0);
  delay(1000);
  pMotor->off();
  pRedLed->switchOff();
}

