#include "tasks/BlinkingTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

BlinkingTask::BlinkingTask(Led* pLed, Context* pContext): 
    pContext(pContext), pLed(pLed){
    setState(State::IDLE);
}
  
void BlinkingTask::tick(){
    switch (state){   
    case State::IDLE: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
            Logger.log(F("[BT] IDLE"));

        }
        if (pContext->isStarted()){
            setState(State::OFF);
        }
        break;
    }
    case State::OFF: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOff();
            Logger.log(F("[BT] OFF"));
        }
        if (pContext->isStopped()){
            setState(State::IDLE);
        } else {
             setState(State::ON);
        }
        break;
    }
    case State::ON: {
        if (this->checkAndSetJustEntered()){
            pLed->switchOn();
            Logger.log(F("[BT] ON"));
        }
        if (pContext->isStopped()){
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
