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
#include "tasks/LandingTask.h"
#include "tasks/OnOffTask.h"
#include "LiquidCrystal_I2C.h"
#include "devices/Lcd.h"
//#define __TESTING_HW__
Scheduler sched;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4); 
HWPlatform pHWPlatform;
Lcd lcdWrapper(&lcd);
// Context *pContext;
void onDroneStateChangedHandler(Context::DroneState s) {
    Serial.println(Context::droneStateName(s));
    Serial.flush();
}
void onHangarStateChangedHandler(Context::HangarState s){
  Serial.println(Context::hangarStateName(s));
  Serial.flush();
}
void serialEvent(){
  MsgService.SerialEvent();
}
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  delay(100);
  MsgService.init();
  Serial.println(F("init"));
  sched.init(50);
  pHWPlatform.init(&lcdWrapper);
  pContext.reset();
  pContext.onDroneStateChanged = onDroneStateChangedHandler;
  pContext.onHangarStateChanged = onHangarStateChangedHandler;
  Serial.println(F("handler"));
  Serial.println(F("HW init"));

#ifndef __TESTING_HW__
  Task* pOnOffTask = new OnOffTask(pHWPlatform.getGreen1Led());
  pOnOffTask->init(500);
  Task* pTakeOffTask = new TakeOffTask(pHWPlatform.getSonar(),pHWPlatform.getMotor(),pHWPlatform.getLcd());
  pTakeOffTask->init(500);
  Task* pAlarmTask = new AlarmTask(pHWPlatform.getButton(),pHWPlatform.getRedLed(),pHWPlatform.getTempSensor(),pHWPlatform.getLcd());
  pAlarmTask->init(100);
  Task* pBlinkingTask = new BlinkingTask(pHWPlatform.getGreen2Led());
  pBlinkingTask->init(500);
  Task* pLandingTask = new LandingTask(pHWPlatform.getSonar(),pHWPlatform.getMotor(),pHWPlatform.getLcd(),pHWPlatform.getPir());
  pLandingTask->init(500);
  Serial.println(F("Tasks inited"));
  sched.addTask(pOnOffTask);
  sched.addTask(pTakeOffTask);
  sched.addTask(pAlarmTask);
  sched.addTask(pBlinkingTask);
  sched.addTask(pLandingTask);
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
    MsgService.handleMessages();
}