#ifndef __ON_OFF_TASK__
#define __ON_OFF_TASK__

#include "kernel/Task.h"
#include "model/Context.h"
#include "devices/Led.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
class OnOffTask: public Task {

public:
  OnOffTask(Led* pGreen1Led, Context* pContext); 
  void tick();

private:  
  enum class State{ OFF, ON };
  void setState(State state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  State state;
  long stateTimestamp;
  bool justEntered;

  Led* pGreen1Led;
  Context* pContext;
};

#endif