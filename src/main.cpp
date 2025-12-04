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
#include "tasks/OnOffTask.h"

//#define __TESTING_HW__

Scheduler sched;

HWPlatform* pHWPlatform;
Context* pContext;
void onDroneStateChangedHandler(Context::DroneState s) {
    MsgService.sendMsg("DRONE:" + String(Context::droneStateName(s)));
}
void setup() {
  MsgService.init();
  sched.init(50);
  pContext = new Context();
  pContext->reset();
  Logger.log(":::::: Drone System ::::::");
  
  pContext->onDroneStateChanged = onDroneStateChangedHandler;
  pHWPlatform = new HWPlatform();
  pHWPlatform->init();

#ifndef __TESTING_HW__
  Task* pOnOffTask = new OnOffTask(pHWPlatform->getGreen1Led(),pContext);
  pOnOffTask->init();
  Task* pTakeOffTask = new TakeOffTask(pHWPlatform->getSonar(),pHWPlatform->getMotor(),pContext,pHWPlatform->getLcd(),
  &MsgService);
  pTakeOffTask->init();
  Task* pAlarmTask = new AlarmTask(pHWPlatform->getButton(),pHWPlatform->getRedLed(),pContext,&MsgService,
pHWPlatform->getTempSensor(),pHWPlatform->getLcd());
  pAlarmTask->init();
  Task* pBlinkingTask = new BlinkingTask(pHWPlatform->getGreen2Led(), pContext);
  pBlinkingTask->init(500);
  Logger.log("tasks are initialized");
  sched.addTask(pOnOffTask);
  sched.addTask(pTakeOffTask);
  sched.addTask(pAlarmTask);
  sched.addTask(pBlinkingTask);
#endif

#ifdef __TESTING_HW__
  /* Testing */
  Task* pTestHWTask = new TestHWTask(pHWPlatform);
  pTestHWTask->init(2000);
  sched.addTask(pTestHWTask);
#endif
}

void loop() {
    sched.schedule();
}