#ifndef __LOGGER__
#define __LOGGER__


#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif


class LoggerService {
    
public: 
  
  void log(const String& msg);
};

extern LoggerService Logger;

#endif

