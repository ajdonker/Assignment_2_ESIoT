#ifndef __LANDING_TASK__
#define __LANDING_TASK__

#include "kernel/Task.h"
#include "kernel/MsgService.h"
#include "devices/ServoMotor.h"
#include "devices/Button.h"
#include "model/Context.h"
#include "devices/Lcd.h"
#include "devices/Sonar.h"
#include "devices/Pir.h"
#include "LandingPattern.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
class LandingTask: public Task {

public:
  LandingTask(Sonar* pSonar, ServoMotor* pMotor,Lcd* pLcd,Pir* pPir); 
  void tick();

private:  
  enum class State{IDLE,WAIT_DETECT, OPEN_DOOR, WAIT, TIMEOUT, ENTERED};
  void setState(State state);
  long elapsedTimeInState();
  
  bool checkAndSetJustEntered();
  
  State state;
  long stateTimestamp;
  bool justEntered;

  Sonar* pSonar;
  ServoMotor* pMotor;
  Lcd* pLcd;  
  Pir* pPir;
  LandingPattern landingPattern;
  int currentPos;
  long distanceGreaterD2Timestamp;
  long landingMsgTimestamp;
  bool toBeStopped;
};

#endif