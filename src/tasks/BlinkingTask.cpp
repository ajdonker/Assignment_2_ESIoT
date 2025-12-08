#include "tasks/BlinkingTask.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "config.h"

BlinkingTask::BlinkingTask(Led* pGreen2Led): 
    pGreen2Led(pGreen2Led){
    setActive(true);
    setState(State::IDLE);
}
  
void BlinkingTask::tick(){
    switch (state){   
    case State::IDLE: {
        if (this->checkAndSetJustEntered()){
            pGreen2Led->switchOff();
            Serial.println(F("[BT] IDLE"));

        }
        Context::DroneState ds = pContext.getDroneState();
        if (ds == Context::DroneState::TAKE_OFF || ds == Context::DroneState::LANDING){
            setState(State::OFF);
        }
        break;
    }
    case State::OFF: {
        if (this->checkAndSetJustEntered()){
            pGreen2Led->switchOff();
            //Serial.println(F("[BT] OFF"));
        }
        Context::DroneState ds = pContext.getDroneState();
        if (ds != Context::DroneState::TAKE_OFF && ds != Context::DroneState::LANDING){
            setState(State::IDLE);
        } else {
             setState(State::ON);
        }
        break;
    }
    case State::ON: {
        if (this->checkAndSetJustEntered()){
            pGreen2Led->switchOn();
            //Serial.println(F("[BT] ON"));
        }
        Context::DroneState ds = pContext.getDroneState();
        if (ds != Context::DroneState::TAKE_OFF && ds != Context::DroneState::LANDING){
            setState(State::IDLE);
        } else {
            setState(State::OFF);
        }
        break;
    }
    }
}


void BlinkingTask::setState(State s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long BlinkingTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool BlinkingTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
