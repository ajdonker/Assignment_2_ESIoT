#include "tasks/TestHWTask.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "config.h"


TestHWTask::TestHWTask(HWPlatform* pHW): pHW(pHW){
}
  
void TestHWTask::tick(){
    pHW->test();
}


