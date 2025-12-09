#include "LandingTask.h"
#ifdef __FAKE_UNO__
#include "FakeArduino.h"
#else
#include "Arduino.h"
#endif
#include "config.h"
#include "../constants.h"
#define OPEN_DOOR_TIME 2000
#define CLOSE_DOOR_TIME 1000
#define START_TIME 100
#define RESET_TIME 500
#define TIMEOUT_TIME 25000
#define LANDING_MSG_PERIOD 2000
#define PIR_DETECT_PERIOD 15000
LandingTask::LandingTask(Sonar *pSonar, ServoMotor *pMotor,Lcd *pLcd,Pir *pPir) : pSonar(pSonar), pMotor(pMotor), 
pLcd(pLcd),pPir(pPir)
{
    setState(State::IDLE);
    setActive(true);
}
// DroneState{INSIDE,TAKE_OFF,OUTSIDE,LANDING}
//  {IDLE, OPEN_DOOR,WAIT_DETECT, WAIT, TIMEOUT, ENTERED}
void LandingTask::tick()
{
    switch (state)
    {
        case State::IDLE:
        {
            if(this->checkAndSetJustEntered())
            {
                Serial.println(F("[LO]:IDLE"));
            }
            if(pContext.getHangarState() == Context::HangarState::IDLE) // if any alarm do nothing
            {
                if(pContext.getDroneState() == Context::DroneState::OUTSIDE)
                {
                    pLcd->printAt(2,2,F("DRONE_OUTSIDE"));
                }
                else if(pContext.getDroneState() == Context::DroneState::LANDING)
                {
                    pMotor->on();
                    pContext.setStarted();
                    setState(State::WAIT_DETECT);
                }
            }
        break;
        }
        case State::WAIT_DETECT:
        {
            if(this->checkAndSetJustEntered())
            {
                Serial.println(F("[LO]:WAIT_DETECT"));
                //pPir->calibrate();
            }
            long dt = elapsedTimeInState();
            pPir->sync();
            bool detection = pPir->isDetected();
            Serial.println(detection);
            if(detection)
            {
                setState(State::OPEN_DOOR);
            }
            // optionally, if still not detected drone dont even start the land/takeoff
            if(dt > PIR_DETECT_PERIOD || pContext.isToBeStopped())
            {
                pContext.setDroneState(Context::DroneState::OUTSIDE); // risky but no other way to implement
                setState(State::IDLE);
            }
            break;
        }
        case State::OPEN_DOOR:
        {
            if (this->checkAndSetJustEntered())
            {
                Serial.println(F("OPEN_DOOR"));
                pLcd->clear();
                pLcd->printAt(2, 2, F("LANDING"));
            }

            /* update motor pos*/
            // add unconditional 
            long dt = elapsedTimeInState();
            currentPos = (((float)dt) / OPEN_DOOR_TIME) * 180;
            pMotor->setPosition(currentPos);
            if(pContext.getHangarState() == Context::HangarState::ALARM)
            {
                pMotor->setPosition(0);
                setState(State::IDLE);
            }
            else if (dt > OPEN_DOOR_TIME)
            {
                setState(State::WAIT);
            }
            break;
        }
        case State::WAIT:
        {
            if (this->checkAndSetJustEntered())
            {
                Serial.println(F("WAIT"));
                distanceGreaterD2Timestamp = 0;
                landingMsgTimestamp = 0;
            }
            long dt = elapsedTimeInState();
            float readOut = pSonar->getDistance();
            if(pContext.getHangarState() == Context::HangarState::ALARM)
            {
                pMotor->setPosition(0);
                setState(State::IDLE);
            }
            else if(readOut > 0)
            {
                if(dt - landingMsgTimestamp > LANDING_MSG_PERIOD)
                {
                    landingMsgTimestamp = dt;
                    MsgService.sendMsg("DIST:"+ String(readOut));
                }
                if (readOut >= D2)
                {
                    distanceGreaterD2Timestamp = dt;
                }
                if (dt - distanceGreaterD2Timestamp > T2)
                {
                    setState(State::ENTERED);
                }
            }
            else if (dt > TIMEOUT_TIME)
            {
                setState(State::TIMEOUT);
            }
            break;
        }
        case State::TIMEOUT:
        {
            if (this->checkAndSetJustEntered())
            {
                Serial.println(F("TIMEOUT"));
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
            else if (dt > CLOSE_DOOR_TIME)
            {
                pMotor->off();
                setState(State::IDLE);
                pContext.setDroneState(Context::DroneState::OUTSIDE);
            }
            break;
        }
        case State::ENTERED:
        {
            if (this->checkAndSetJustEntered())
            {
                Serial.println(F("ENTERED"));
                pLcd->clear();
                pLcd->printAt(2, 2, F("DRONE_INSIDE"));
                pContext.setStopped();
                pContext.setDroneState(Context::DroneState::INSIDE);
            }
            if(pContext.getHangarState() == Context::HangarState::ALARM)
            {
                pMotor->setPosition(0);
                setState(State::IDLE);
            }
            if (elapsedTimeInState() > RESET_TIME)
            {
                pMotor->off();
                setState(State::IDLE);
                //setActive(false);
            }
            break;
        }
    }
    
}

void LandingTask::setState(State s)
{
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long LandingTask::elapsedTimeInState()
{
    return millis() - stateTimestamp;
}

bool LandingTask::checkAndSetJustEntered()
{
    bool bak = justEntered;
    if (justEntered)
    {
        justEntered = false;
    }
    return bak;
}