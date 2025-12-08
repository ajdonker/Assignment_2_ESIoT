#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "MsgService.h"
#include "model/HWPlatform.h"
// optional : does not fit the architecture to put here 
MsgServiceClass MsgService;

bool MsgServiceClass::isMsgAvailable(){
  // if (msgAvailable) {
  //     Serial.println("[DEBUG] isMsgAvailable = TRUE");
  //   }
    return msgAvailable;
}

Msg* MsgServiceClass::receiveMsg(){
  //Serial.println(F("MSG"));
  if (msgAvailable){
    Msg* msg = currentMsg;
    //Serial.println(msg->getContent());
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  }
}

void MsgServiceClass::init(){
  // Serial.begin(115200);
  // Serial.println("init");
  content.reserve(64);
  content = "";
  currentMsg = NULL;
  msgAvailable = false;  
  //Serial.println("MsgService inited");
}

void MsgServiceClass::sendMsg(const String& msg){
  Serial.println(msg);  
}

void MsgServiceClass::SerialEvent() {
  /* reading the content */
  //Serial.println("[DEBUG] SERIAL EVENT");
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch == '\n'){
      MsgService.currentMsg = new Msg(content);
      MsgService.msgAvailable = true;      
    } else {
      content += ch;      
    }
  }
}

bool MsgServiceClass::isMsgAvailable(Pattern& pattern){
  return (msgAvailable && pattern.match(*currentMsg));
}

Msg* MsgServiceClass::receiveMsg(Pattern& pattern){
  if (msgAvailable && pattern.match(*currentMsg)){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  } 
}
void MsgServiceClass::handleMessages(){
  if (MsgService.isMsgAvailable()) {
    Msg* msg = MsgService.receiveMsg();
    msg->getContent().trim();
    Serial.print(F("Received message: "));
    Serial.println(msg->getContent());
    // can be made as msg.startswith 
    if(msg->getContent().indexOf("[DRU]:TAKE_OFF") != -1  && 
        pContext.getHangarState() != Context::HangarState::ALARM &&
        pContext.getHangarState() != Context::HangarState::PRE_ALARM) {
        pContext.setDroneState(Context::DroneState::TAKE_OFF);
    } else if(msg->getContent().indexOf("[DRU]:LAND") != -1  && 
              pContext.getHangarState() != Context::HangarState::ALARM &&
              pContext.getHangarState() != Context::HangarState::PRE_ALARM) {
        pContext.setDroneState(Context::DroneState::LANDING);
    } 
    else if(msg->getContent().startsWith("[DRU]:TEMP")) {
      String tempStr = msg->getContent().substring(11); // skip "[DRU]:TEMP:"
      float fakeTemp = tempStr.toFloat();
      Serial.print(F("Fake temperature received: "));
      Serial.println(fakeTemp);

      pHWPlatform.getTempSensor()->setTemperature(fakeTemp);
    }
    delete msg;
  }
}

