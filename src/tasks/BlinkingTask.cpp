#include "tasks/BlinkingTask.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "config.h"
#include "kernel/Logger.h"

BlinkingTask::BlinkingTask(Led* pGreen2Led, Context* pContext): 
    pContext(pContext), pGreen2Led(pGreen2Led){
    setState(State::IDLE);
}
  
void BlinkingTask::tick(){
    switch (state){   
    case State::IDLE: {
        if (this->checkAndSetJustEntered()){
            pGreen2Led->switchOff();
            Logger.log(F("[BT] IDLE"));

        }
        if (pContext->isStarted()){
            setState(State::OFF);
        }
        break;
    }
    case State::OFF: {
        if (this->checkAndSetJustEntered()){
            pGreen2Led->switchOff();
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
            pGreen2Led->switchOn();
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
