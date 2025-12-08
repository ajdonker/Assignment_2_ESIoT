#include "HWPlatform.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include <Arduino.h>
#endif

HWPlatform::HWPlatform()
{
}

void HWPlatform::init(Lcd* externalLcd) {
  // devices auto-init in constructors or when first used
  pButton     = new ButtonImpl(BT_PIN);
  //Serial.println(F("A"));
  pRedLed     = new Led(RED_PIN);
  //Serial.println(F("B"));
  pGreen1Led  = new Led(GREEN1_PIN);
  //Serial.println(F("C"));
  pGreen2Led  = new Led(GREEN2_PIN);
  //Serial.println(F("D"));
  pLcd = externalLcd;         // <-- MAIN LCD stored here
  //Serial.println(F("LCD linked"));
  //Serial.println(F("E"));
  pMotor      = new ServoMotorImpl(MOTOR_PIN);
  //Serial.println(F("F"));
  pPir        = new Pir(PIR_PIN);
  //Serial.println(F("G"));
  pSonar      = new Sonar(SONAR_ECHO_PIN, SONAR_TRIG_PIN, SONAR_TIMEOUT_TIME);
  //Serial.println(F("H"));
  pTempSensor = new FakeTempSensor(TEMP_PIN);
}

Button* HWPlatform::getButton()     { return pButton; }
Led* HWPlatform::getRedLed()        { return pRedLed; }
Led* HWPlatform::getGreen1Led()     { return pGreen1Led; }
Led* HWPlatform::getGreen2Led()     { return pGreen2Led; }
ServoMotor* HWPlatform::getMotor()  { return pMotor; }
Pir* HWPlatform::getPir()           { return pPir; }
Sonar* HWPlatform::getSonar()       { return pSonar; }
TempSensor* HWPlatform::getTempSensor(){ return pTempSensor; }
Lcd* HWPlatform::getLcd()           { return pLcd; }

void HWPlatform::test() {
  bool btPressed = pButton->isPressed();
  pPir->sync();  
  pRedLed->switchOn();
  pMotor->on();
  pMotor->setPosition(90);

  //Serial.println(F("Button:"));
  //Serial.println(btPressed ? F("pressed") : F("not pressed"));

  //Serial.println(F("Sonar readout:"));
  //Serial.println(pSonar->getDistance());

  //Serial.println(F("Temp Sensor readout"));
  //Serial.println(pTempSensor->getTemperature());

  //Serial.println(F("PIR readout:"));
  //Serial.println(pPir->isDetected());

  pLcd->printAt(2, 2, "HELLO");

  delay(1000);
  pMotor->setPosition(0);
  delay(1000);

  pMotor->off();
  pRedLed->switchOff();
}


