#include "tasks/AlarmTask.h"
#include <Arduino.h>
#include "config.h"
#include "../constants.h"
#include "kernel/Logger.h"
AlarmTask::AlarmTask(Button* pResetButton, Led* pRedLed, Context* pContext, MsgServiceClass* pMsgService,
TempSensor* pTempSensor):
pResetButton(pResetButton),pRedLed(pRedLed), pContext(pContext), pMsgService(pMsgService), pTempSensor(pTempSensor)
{
    setState(State::IDLE);
}

//enum class State{ IDLE, PRE_ALARM, ALARM };
void AlarmTask::tick(){
    if(pContext->getDroneState() != Context::DroneState::OUTSIDE){
        switch(state){
            case(State::IDLE):{
                if(this->checkAndSetJustEntered()){
                    TempLesserT1Timestamp = 0;
                }
                long dt = elapsedTimeInState();
                float TempReadout = pTempSensor->getTemperature();
                if(TempReadout < Temp1)
                {
                    TempLesserT1Timestamp = dt;
                }
                if(dt - TempLesserT1Timestamp > T3)
                {
                    setState(State::PRE_ALARM);
                }
                break;
            }
            case(State::PRE_ALARM):{
                if(this->checkAndSetJustEntered()){
                    pContext->setToBeStopped(true);
                }
                long dt = elapsedTimeInState();
                float TempReadout = pTempSensor->getTemperature();
                if(TempReadout < Temp2)
                {
                    TempLesserT1Timestamp = dt;
                }
                if(dt - TempLesserT1Timestamp > T4)
                {
                    setState(State::ALARM);
                }
                break;
            }
            case(State::ALARM):{
                if(this->checkAndSetJustEntered()){
                    pRedLed->switchOn();
                    if(pContext->getDroneState() == Context::DroneState::OUTSIDE){
                        pMsgService->sendMsg("ALARM");
                    }
                }
                
                if(pResetButton->isPressed())
                {
                    pRedLed->switchOff();
                    setState(State::IDLE);
                }
                break;
            }
        }
    }
}
void AlarmTask::setState(State s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long AlarmTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool AlarmTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
