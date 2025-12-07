#include "LandingTask.h"
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
#define LANDING_MSG_PERIOD 2000
LandingTask::LandingTask(Sonar *pSonar, ServoMotor *pMotor,Lcd *pLcd) : pSonar(pSonar), pMotor(pMotor), 
pLcd(pLcd)
{
    setState(State::IDLE);
}
// DroneState{INSIDE,TAKE_OFF,OUTSIDE,LANDING}
//  {IDLE, OPEN_DOOR, WAIT, TIMEOUT, ENTERED}
void LandingTask::tick()
{
    if (!pContext.isToBeStopped() && !isActive() && pContext.getDroneState() == Context::DroneState::OUTSIDE)
    {
        setActive(true);
    }
    if (isActive())
    {
        switch (state)
        {
        case State::IDLE:
        {
            if(this->checkAndSetJustEntered())
            {
                Serial.println(F("IDLE"));
            }
            if(MsgService.isMsgAvailable())
            {
                if(MsgService.receiveMsg(landingPattern))
                {
                    pMotor->on();
                    pContext.setStarted();
                    pContext.setDroneState(Context::DroneState::LANDING);
                    setState(State::OPEN_DOOR);
                }
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

            long dt = elapsedTimeInState();
            currentPos = (((float)dt) / OPEN_DOOR_TIME) * 180;
            pMotor->setPosition(currentPos);

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
                Serial.println(F("WAIT"));
                distanceGreaterD2Timestamp = 0;
                landingMsgTimestamp = 0;
            }
            long dt = elapsedTimeInState();
            float readOut = pSonar->getDistance();
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
                Serial.println(F("TIMEOUT"));
            }
            /* update motor pos*/
            // close door needed
            long dt = elapsedTimeInState();
            currentPos = (((float)dt) / CLOSE_DOOR_TIME) * 180;
            pMotor->setPosition(currentPos);

            if (dt > CLOSE_DOOR_TIME)
            {
                pMotor->off();
                setState(State::IDLE);
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
                pContext.setDroneState(Context::DroneState::OUTSIDE);
            }
            if (elapsedTimeInState() > RESET_TIME)
            {
                pMotor->off();
                setState(State::IDLE);
                setActive(false);
            }
            break;
        }
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