#include "TakeOffTask.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "config.h"
#include "../constants.h"
// #include "kernel/Serial.printlnger.h"
// #include "model/Context.h"
#define OPEN_DOOR_TIME 2000
#define CLOSE_DOOR_TIME 1000
#define START_TIME 100
#define RESET_TIME 500
#define TIMEOUT_TIME 25000

TakeOffTask::TakeOffTask(Sonar *pSonar, ServoMotor *pMotor,Lcd *pLcd)
 : pSonar(pSonar), pMotor(pMotor),pLcd(pLcd)
{
    setState(State::IDLE);
    setActive(true);
}
// DroneState{INSIDE,TAKE_OFF,OUTSIDE,LANDING}
//  {IDLE, OPEN_DOOR, WAIT, TIMEOUT, EXITED}
void TakeOffTask::tick()
{
    switch (state)
    {
        case State::IDLE:
        {
            if(this->checkAndSetJustEntered())
            {
                Serial.println(F("[TO]:IDLE"));
            }
            if(pContext.getHangarState() == Context::HangarState::IDLE)
                {
                    if(pContext.getDroneState() == Context::DroneState::INSIDE)
                    {
                        pLcd->printAt(2,2,F("DRONE_INSIDE"));
                    }
                    else if(pContext.getDroneState() == Context::DroneState::TAKE_OFF)
                    {
                        setState(State::OPEN_DOOR);
                        pMotor->on();
                        pContext.setStarted();  
                    }
                }
        break;
        }
        case State::OPEN_DOOR:
        {
            if (this->checkAndSetJustEntered())
            {
                Serial.println(F("[TO]:OPEN_DOOR"));
                //pContext.setDroneState(Context::DroneState::TAKE_OFF);
                pLcd->clear();
                pLcd->printAt(2, 2, F("TAKE_OFF"));
            }

            /* update motor pos*/

            long dt = elapsedTimeInState();
            currentPos = (((float)dt) / OPEN_DOOR_TIME) * 180;
            pMotor->setPosition(currentPos);
            if(pContext.getHangarState() == Context::HangarState::ALARM)
            {
                pMotor->setPosition(0);
                setState(State::IDLE);
            }
            if (dt > OPEN_DOOR_TIME)
            {
                setState(State::WAIT);
            }
            break;
        }
        case State::WAIT:
        {
            if (this->checkAndSetJustEntered())
            {
                Serial.println(F("[TO]:WAIT"));
                distanceLessD1Timestamp = 0;
            }
            long dt = elapsedTimeInState();
            float readOut = pSonar->getDistance();
            Serial.println(readOut);
            if(pContext.getHangarState() == Context::HangarState::ALARM)
            {
                pMotor->setPosition(0);
                setState(State::IDLE);
            }
            if(readOut > 0)
            {
                if (readOut < D1)
                {
                    distanceLessD1Timestamp = dt;
                }
            }
            if (dt - distanceLessD1Timestamp > T1)
            {
                setState(State::EXITED);
            }
            if (dt > TIMEOUT_TIME)
            {
                setState(State::TIMEOUT);
            }
            break;
        }
        case State::TIMEOUT:
        {
            if (this->checkAndSetJustEntered())
            {
                Serial.println(F("[TO]:TIMEOUT"));
            }
            /* update motor pos*/
            // close door needed
            long dt = elapsedTimeInState();
            currentPos = (((float)dt) / CLOSE_DOOR_TIME) * 180;
            pMotor->setPosition(currentPos);
            if(pContext.getHangarState() == Context::HangarState::ALARM)
            {
                pMotor->setPosition(0);
                setState(State::IDLE);
            }
            if (dt > CLOSE_DOOR_TIME)
            {
                pMotor->off();
                setState(State::IDLE);
                pContext.setDroneState(Context::DroneState::INSIDE);
            }
            break;
        }
        case State::EXITED:
        {
            if (this->checkAndSetJustEntered())
            {
                Serial.println(F("[TO]:EXITED"));
                pLcd->clear();
                pLcd->printAt(2, 2, F("DRONE_OUTSIDE"));
                pContext.setStopped();
                pContext.setDroneState(Context::DroneState::OUTSIDE);
            }
            // closing door code 
            long dt = elapsedTimeInState();
            currentPos = (((float)dt) / CLOSE_DOOR_TIME) * 180;
            pMotor->setPosition(currentPos);
            if (elapsedTimeInState() > RESET_TIME)
            {
                pMotor->off();
                setState(State::IDLE);
            }
            break;
        }
    }
}

void TakeOffTask::setState(State s)
{
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long TakeOffTask::elapsedTimeInState()
{
    return millis() - stateTimestamp;
}

bool TakeOffTask::checkAndSetJustEntered()
{
    bool bak = justEntered;
    if (justEntered)
    {
        justEntered = false;
    }
    return bak;
}