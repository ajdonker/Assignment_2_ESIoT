#ifndef __ALARM_TASK__
#define __ALARM_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/Button.h"
#include "devices/Led.h"
#include "kernel/MsgService.h"
#include "devices/TempSensor.h"
#include "devices/Lcd.h"
#include <Arduino.h>

class AlarmTask : public Task{
public:
  AlarmTask(Button* pResetButton, Led* pRedLed, Context* pContext, MsgServiceClass* MsgService,
TempSensor* pTempSensor, Lcd* pLcd); 
  void tick();

private:  
  enum class State{ IDLE, PRE_ALARM, ALARM };
  void setState(State state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  State state;
  long stateTimestamp;
  bool justEntered;
  long TempLesserT1Timestamp;

  Led* pRedLed;
  Context* pContext;
  MsgServiceClass* pMsgService;
  Button* pResetButton;
  TempSensor* pTempSensor;
  Lcd* pLcd;
};

#endif