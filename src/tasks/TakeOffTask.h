#ifndef __TAKE_OFF_TASK__
#define __TAKE_OFF_TASK__

#include "kernel/Task.h"
#include "kernel/MsgService.h"
#include "kernel/Logger.h"
#include "devices/ServoMotor.h"
#include "devices/Button.h"
#include "model/Context.h"
#include "devices/Lcd.h"
#include "devices/Sonar.h"
#include "TakeOffPattern.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
class TakeOffTask: public Task {

public:
  TakeOffTask(Sonar* pSonar, ServoMotor* pMotor, Context* pContext, Lcd* pLcd,  
    MsgServiceClass *pMsgService); 
  void tick();

private:  
  enum class State{IDLE, OPEN_DOOR, WAIT, TIMEOUT, EXITED};
  void setState(State state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  State state;
  long stateTimestamp;
  bool justEntered;

  Sonar* pSonar;
  ServoMotor* pMotor;
  Context* pContext;
  Lcd* pLcd;
  MsgServiceClass* pMsgService;

  TakeOffPattern takeOffPattern;
  int currentPos;
  long distanceLessD1Timestamp;
  bool toBeStopped;
};

#endif