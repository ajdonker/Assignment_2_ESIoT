#include "tasks/TestHWTask.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "config.h"
#include "LiquidCrystal_I2C.h"
#include "devices/Lcd.h"

TestHWTask::TestHWTask(HWPlatform* pHW): pHW(pHW){
    setActive(true);
}
  
void TestHWTask::tick(){
    pHW->test();
}


