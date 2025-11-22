#ifndef __SWEEPING_TASK__
#define __SWEEPING_TASK__

#include "kernel/Task.h"
#include "kernel/MsgService.h"
#include "kernel/Logger.h"
#include "devices/ServoMotor.h"
#include "devices/Button.h"
#include "model/Context.h"
#include "devices/Lcd.h"
#include "devices/Sonar.h"
#include <Arduino.h>
class TakeOffPattern: public Pattern{
  public:
    bool match(const Msg& m) override {
        return m.getContent() == "TAKE_OFF";
    }
};
class TakeOffTask: public Task {

public:
  TakeOffTask(Sonar* pSonar, ServoMotor* pMotor, Context* pContext, Lcd* pLcd,  
    MsgServiceClass* pMsgService, LoggerService* pLogger); 
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

  Button* pButton;
  ServoMotor* pMotor;
  Context* pContext;
  MsgServiceClass* pMsgService;
  LoggerService* pLogger;
  Lcd* pLcd;
  Sonar* pSonar;
  TakeOffPattern takeOffPattern;
  int currentPos;
  long distanceLessD1Timestamp;
  bool toBeStopped;
};

#endif