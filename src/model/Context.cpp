#include "Context.h"

Context::Context(){
  started = false;
  stopped = false;
  toBeStopped = false;
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
}
void Context::setDroneState(Context::DroneState s)
{
  droneState = s;
  if (onDroneStateChanged) {
        onDroneStateChanged(s);
  }
}
const char* Context::droneStateName(Context::DroneState s){
  switch(s) {
        case DroneState::INSIDE:     return "DRONE:INSIDE";
        case DroneState::TAKE_OFF:   return "DRONE:TAKE_OFF";
        case DroneState::OUTSIDE:    return "DRONE:OUTSIDE";
        case DroneState::LANDING:    return "DRONE:LANDING";
    }
    return "UNKNOWN";
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
const char* Context::hangarStateName(Context::HangarState s){
  switch(s) {
        case HangarState::IDLE:     return "HANGAR:IDLE";
        case HangarState::PRE_ALARM:   return "HANGAR:PRE_ALARM";
        case HangarState::ALARM:    return "HANGAR:ALARM";
    }
    return "UNKNOWN";
}

