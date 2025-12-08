#include "tasks/AlarmTask.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "config.h"
#include "../constants.h"

AlarmTask::AlarmTask(Button* pResetButton, Led* pRedLed,TempSensor* pTempSensor, Lcd* pLcd):
pResetButton(pResetButton),pRedLed(pRedLed),pTempSensor(pTempSensor),pLcd(pLcd)
{
    setActive(true);
    setState(State::IDLE);
}

//enum class State{ IDLE, PRE_ALARM, ALARM };
void AlarmTask::tick(){
    if(pContext.getDroneState() != Context::DroneState::OUTSIDE){
        switch(state){
            case(State::IDLE):{
                if(this->checkAndSetJustEntered()){
                    TempLesserT1Timestamp = 0;
                    //MsgService->sendMsg("HANGAR:IDLE");
                    pContext.setToBeStopped(false);
                    pContext.setHangarState(Context::HangarState::IDLE);
                    Serial.println("[AA]:IDLE");
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
                    pContext.setToBeStopped(true);
                    pContext.setHangarState(Context::HangarState::PRE_ALARM);
                    Serial.println("[AA]:PRE_ALARM");
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
                    Serial.println("[AA]:ALARM");
                    pLcd->clear();
                    pLcd->printAt(2,2,"ALARM");
                    pContext.setHangarState(Context::HangarState::ALARM);
                    if(pContext.getDroneState() == Context::DroneState::OUTSIDE){
                        MsgService.sendMsg("ALERT");
                    }
                }
                
                if(pResetButton->isPressed())
                {
                    pRedLed->switchOff();
                    pLcd->clear();
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
