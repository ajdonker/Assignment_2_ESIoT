#include "LandingTask.h"
#include <Arduino.h>
#include "config.h"
#include "../constants.h"
// #include "kernel/Logger.h"
// #include "model/Context.h"
#define OPEN_DOOR_TIME 2000
#define CLOSE_DOOR_TIME 1000
#define START_TIME 100
#define RESET_TIME 500
#define TIMEOUT_TIME 25000
LandingTask::LandingTask(Sonar *pSonar, ServoMotor *pMotor, Context *pContext, Lcd *pLcd,
                         MsgServiceClass *pMsgService) : pSonar(pSonar), pMotor(pMotor), pContext(pContext), pLcd(pLcd), pMsgService(pMsgService)
{
    setState(State::IDLE);
}
void LandingTask::log(const String &msg)
{
    Logger.log("[LA]:");
    Logger.log(msg);
}
// DroneState{INSIDE,TAKE_OFF,OUTSIDE,LANDING}
//  {IDLE, OPEN_DOOR, WAIT, TIMEOUT, EXITED}
void LandingTask::tick()
{
    if (!isActive() && pContext->getDroneState() == Context::DroneState::OUTSIDE)
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
                log(F("IDLE"));
            }
            if(pMsgService->isMsgAvailable())
            {
                if(pMsgService->receiveMsg(takeOffPattern))
                {
                    pMotor->on();
                    pContext->setStarted();
                    pContext->setDroneState(Context::DroneState::LANDING);
                    setState(State::OPEN_DOOR);
                }
            }
        break;
        }
        case State::OPEN_DOOR:
        {
            if (this->checkAndSetJustEntered())
            {
                log(F("OPEN_DOOR"));
                //pContext->setDroneState(Context::DroneState::TAKE_OFF);
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
                log(F("WAIT"));
                distanceGreaterD2Timestamp = 0;
            }
            long dt = elapsedTimeInState();
            float readOut = pSonar->getDistance();
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
                log(F("TIMEOUT"));
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
        case State::EXITED:
        {
            if (this->checkAndSetJustEntered())
            {
                log(F("EXITED"));
                pLcd->clear();
                pLcd->printAt(2, 2, F("DRONE_OUT"));
                pContext->setStopped();
                pContext->setDroneState(Context::DroneState::OUTSIDE);
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