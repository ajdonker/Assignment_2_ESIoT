#include "Context.h"
#include <Arduino.h>
Context::Context(){
  started = false;
  stopped = false;
  toBeStopped = false;
  setDroneState(Context::DroneState::INSIDE);
  setHangarState(Context::HangarState::IDLE);
}

bool Context::isStarted(){
  return started;
}

bool Context::isStopped(){
  return stopped;
}
bool Context::isToBeStopped(){
  return toBeStopped;
}
void Context::setToBeStopped(bool val){
  toBeStopped = val;
}
void Context::setStarted(){
  started = true;
  stopped = false;
}

void Context::setStopped(){
  stopped = true;
  started = false;
}

void Context::reset(){
  started = false;
  stopped = false;
  setDroneState(Context::DroneState::INSIDE);
  setHangarState(Context::HangarState::IDLE);
}
void Context::setDroneState(Context::DroneState s)
{
  droneState = s;
  if (onDroneStateChanged) {
        onDroneStateChanged(s);
  }
}
const __FlashStringHelper* Context::droneStateName(Context::DroneState s){
  switch(s) {
        case DroneState::INSIDE:     return F("DRONE:INSIDE");
        case DroneState::TAKE_OFF:   return F("DRONE:TAKE_OFF");
        case DroneState::OUTSIDE:    return F("DRONE:OUTSIDE");
        case DroneState::LANDING:    return F("DRONE:LANDING");
    }
    return F("UNKNOWN");
}
Context::DroneState Context::getDroneState()
{
  return droneState;
}
Context::HangarState Context::getHangarState()
{
  return hangarState;
}
void Context::setHangarState(Context::HangarState s)
{
  hangarState = s;
  if(onHangarStateChanged){
    onHangarStateChanged(s);
  }
}
const __FlashStringHelper* Context::hangarStateName(Context::HangarState s){
  switch(s) {
        case HangarState::IDLE:     return F("HANGAR:IDLE");
        case HangarState::PRE_ALARM:   return F("HANGAR:PRE_ALARM");
        case HangarState::ALARM:    return F("HANGAR:ALARM");
    }
    return F("UNKNOWN");
}

