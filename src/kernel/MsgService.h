#ifndef __MSGSERVICE__
#define __MSGSERVICE__
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "model/Context.h"
class Msg {
  String content;

public:
  Msg(String content){
    this->content = content;
  }
  
  String getContent() const{
    return content;
  }
};

class Pattern {
public:
  virtual bool match(const Msg& m) = 0;  
};

class MsgServiceClass {
    
public: 
  Msg* currentMsg;
  bool msgAvailable;
  String content;
  void init();  

  bool isMsgAvailable();
  Msg* receiveMsg();

  bool isMsgAvailable(Pattern& pattern);
  Msg* receiveMsg(Pattern& pattern);
  
  void sendMsg(const String& msg);
  void SerialEvent();

  void handleMessages();
};

extern MsgServiceClass MsgService;

#endif

