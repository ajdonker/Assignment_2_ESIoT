#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"
#include "tasks/TestHWTask.h"
#include "tasks/SweepingTask.h"
#include "tasks/BlinkingTask.h"

#define __TESTING_HW__

Scheduler sched;

HWPlatform* pHWPlatform;
Context* pContext;

void setup() {
  MsgService.init();
  sched.init(50);
  pContext->reset();
  Logger.log(":::::: Drone System ::::::");
  
  pHWPlatform = new HWPlatform();
  pHWPlatform->init();

#ifndef __TESTING_HW__
  pContext = new Context();

  Task* pSweepingTask = new SweepingTask(pHWPlatform->getButton(), pHWPlatform->getMotor(), pContext);
  pSweepingTask->init(50);

  Task* pAlarmTask = new AlarmTask();
  Task* pBlinkingTask = new BlinkingTask(pHWPlatform->getLed(), pContext);
  pBlinkingTask->init(500);
  
  sched.addTask(pSweepingTask);
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
    sched.schedule();
}