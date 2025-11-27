#ifndef __CONTEXT__
#define __CONTEXT__

class Context {

public:
  Context();
  enum class DroneState{INSIDE,TAKE_OFF,OUTSIDE,LANDING};

  typedef void (*DroneStateChangedCallback)(DroneState);

  DroneStateChangedCallback onDroneStateChanged;

  enum class HangarState{IDLE,PRE_ALARM,ALARM};
  
  typedef void(*HangarStateChangedCallback)(HangarState);

  HangarStateChangedCallback onHangarStateChanged;

  void setStarted();
  void setStopped();

  bool isStarted();  
  bool isStopped();
  void reset();
  bool isToBeStopped();
  void setToBeStopped(bool val);

  void setDroneState(DroneState s);
  static const char* droneStateName(DroneState s);
  DroneState getDroneState();

  void setHangarState(HangarState s);
  static const char* hangarStateName(HangarState s);
  HangarState getHangarState();

private:
  DroneState droneState;
  HangarState hangarState;
  bool started; 
  bool stopped;
  bool toBeStopped;
};

#endif
