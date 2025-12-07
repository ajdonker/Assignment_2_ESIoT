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
#include "LiquidCrystal_I2C.h"
#include "devices/Lcd.h"
Scheduler sched;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4); 
HWPlatform pHWPlatform;
// Context *pContext;
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
  lcd.init();
  lcd.backlight();
  Lcd lcdWrapper(&lcd);
  delay(100);
  MsgService.init();
  Serial.println(F("init"));
  sched.init(50);
  pContext.reset();
  //Serial.println(F("context inited"));
  pContext.onDroneStateChanged = onDroneStateChangedHandler;
  Serial.println(F("handler"));
  // pHWPlatform = new HWPlatform();
  pHWPlatform.init(&lcdWrapper);
  Serial.println(F("HW init"));
  // Serial.println(freeMemory());

#ifndef __TESTING_HW__
  Task* pOnOffTask = new OnOffTask(pHWPlatform.getGreen1Led());
  pOnOffTask->init();
  Task* pTakeOffTask = new TakeOffTask(pHWPlatform.getSonar(),pHWPlatform.getMotor(),pHWPlatform.getLcd());
  pTakeOffTask->init();
  Task* pAlarmTask = new AlarmTask(pHWPlatform.getButton(),pHWPlatform.getRedLed(),pHWPlatform.getTempSensor(),pHWPlatform.getLcd());
  pAlarmTask->init();
  Task* pBlinkingTask = new BlinkingTask(pHWPlatform.getGreen2Led());
  pBlinkingTask->init(500);
  Serial.println(F("Tasks inited"));
  sched.addTask(pOnOffTask);
  sched.addTask(pTakeOffTask);
  sched.addTask(pAlarmTask);
  sched.addTask(pBlinkingTask);
#endif

#ifdef __TESTING_HW__
  /* Testing */
  Task* pTestHWTask = new TestHWTask(&pHWPlatform);
  pTestHWTask->init(2000);
  sched.addTask(pTestHWTask);
#endif
}

void loop() {
    sched.schedule();
}