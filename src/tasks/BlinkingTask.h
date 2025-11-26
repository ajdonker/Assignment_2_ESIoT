#ifndef __BLINKING_TASK__
#define __BLINKING_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/Led.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
class BlinkingTask: public Task {

public:
  BlinkingTask(Led* pGreen2Led, Context* pContext); 
  void tick();

private:  
  enum class State{ IDLE, OFF, ON };
  void setState(State state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  State state;
  long stateTimestamp;
  bool justEntered;

  Led* pGreen2Led;
  Context* pContext;
};

#endif