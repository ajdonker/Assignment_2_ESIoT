#ifdef __FAKE_UNO__
#include "../pc/FakeArduino.h"
#else
#include <Arduino.h>
#endif

#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"
#include "tasks/TestHWTask.h"
#include "tasks/AlarmTask.h"
#include "tasks/TakeOffTask.h"
#include "tasks/BlinkingTask.h"
#include "tasks/OnOffTask.h"

#define __TESTING_HW__
// #if defined(__AVR__)
// extern unsigned int __bss_end;
// extern void *__brkval;
// int freeMemory(){
//   int v;
//   int free_mem;
//   if((int)__brkval == 0){
//     free_mem = (int)&v - (int)&__bss_end;
//   }
//   else{
//     free_mem = (int)&v - (int)__brkval;
//   }
//   return free_mem;
// }
// #endif
Scheduler sched;

HWPlatform* pHWPlatform;
Context* pContext;
void onDroneStateChangedHandler(Context::DroneState s) {
    // MsgService.sendMsg("DRONE:" + String(Context::droneStateName(s)));
    Serial.print(F("DRONE:"));
    Serial.println(Context::droneStateName(s));
}
void serialEvent(){
  MsgService.SerialEvent();
}
void setup() {
  Serial.begin(115200);
  delay(100);
  MsgService.init();
  Serial.println(F("init"));
  sched.init(50);
  pContext = new Context();
  pContext->reset();
  Serial.println(F("context inited"));
  
  pContext->onDroneStateChanged = onDroneStateChangedHandler;
  Serial.println(F("handler"));
  pHWPlatform = new HWPlatform();
  pHWPlatform->init();
  // Serial.println(freeMemory());

#ifndef __TESTING_HW__
  Task* pOnOffTask = new OnOffTask(pHWPlatform->getGreen1Led(),pContext);
  pOnOffTask->init();
  Task* pTakeOffTask = new TakeOffTask(pHWPlatform->getSonar(),pHWPlatform->getMotor(),pContext,pHWPlatform->getLcd());
  pTakeOffTask->init();
  Task* pAlarmTask = new AlarmTask(pHWPlatform->getButton(),pHWPlatform->getRedLed(),pContext,&MsgService,
pHWPlatform->getTempSensor(),pHWPlatform->getLcd());
  pAlarmTask->init();
  Task* pBlinkingTask = new BlinkingTask(pHWPlatform->getGreen2Led(), pContext);
  pBlinkingTask->init(500);
  Serial.println(F("Tasks inited"));
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