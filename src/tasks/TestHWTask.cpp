#include "tasks/TestHWTask.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "config.h"


TestHWTask::TestHWTask(HWPlatform* pHW): pHW(pHW){
    setActive(true);
}
  
void TestHWTask::tick(){
    pHW->test();
}


