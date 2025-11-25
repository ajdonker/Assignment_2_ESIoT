#ifdef __FAKE_UNO__
#include "../pc/FakeArduino.h"
#else
#include <Arduino.h>
#endif
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"
#include "tasks/TestHWTask.h"
#include "tasks/AlarmTask.h"
#include "tasks/TakeOffTask.h"
#include "tasks/SweepingTask.h"
#include "tasks/BlinkingTask.h"

//#define __TESTING_HW__

Scheduler sched;

HWPlatform* pHWPlatform;
Context* pContext;

void setup() {
  MsgService.init();
  sched.init(50);
  pContext = new Context();
  pContext->reset();
  Logger.log(":::::: Drone System ::::::");
  
  pHWPlatform = new HWPlatform();
  pHWPlatform->init();

#ifndef __TESTING_HW__
  Task* pTakeOffTask = new TakeOffTask(pHWPlatform->getSonar(),pHWPlatform->getMotor(),pContext,pHWPlatform->getLcd(),
&MsgService);

  Task* pAlarmTask = new AlarmTask(pHWPlatform->getButton(),pHWPlatform->getRedLed(),pContext,&MsgService,
pHWPlatform->getTempSensor(),pHWPlatform->getLcd());
  Task* pBlinkingTask = new BlinkingTask(pHWPlatform->getGreen2Led(), pContext);
  pBlinkingTask->init(500);
  
  sched.addTask(pTakeOffTask);
  sched.addTask(pAlarmTask);
  sched.addTask(pBlinkingTask);
#endif

#ifdef __TESTING_HW__
  /* Testing */
  Task* pTestHWTask = new TestHWTask(pHWPlatform);
  pTestHWTask->init(500);
  sched.addTask(pTestHWTask);
#endif
}

void loop() {
    MsgService.SerialEvent();
    sched.schedule();
}