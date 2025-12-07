#include "tasks/OnOffTask.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "config.h"

OnOffTask::OnOffTask(Led* pGreen1Led): 
    pGreen1Led(pGreen1Led){
    setState(State::OFF);
}
  
void OnOffTask::tick(){
    switch (state){   
    case State::OFF: {
        if (this->checkAndSetJustEntered()){
            pGreen1Led->switchOff();
            Serial.println(F("[OO] OFF"));
        }
        if(pContext.getHangarState() != Context::HangarState::ALARM){
            if (pContext.getDroneState() == Context::DroneState::INSIDE){
                setState(State::ON);
            } 
        }
        break;
    }
    case State::ON: {
        if (this->checkAndSetJustEntered()){
            pGreen1Led->switchOn();
            Serial.println(F("[OO] ON"));
        }
        if(pContext.getHangarState() == Context::HangarState::ALARM){
            setState(State::OFF);
        }
        else if(pContext.getDroneState() != Context::DroneState::INSIDE){
            setState(State::OFF);
        } 
        break;
    }
    }
}


void OnOffTask::setState(State s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long OnOffTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool OnOffTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}