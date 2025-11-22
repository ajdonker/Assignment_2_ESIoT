#ifndef __CONTEXT__
#define __CONTEXT__

class Context {

public:
  Context();
  enum class DroneState{INSIDE,TAKE_OFF,OUTSIDE,LANDING};

  void setStarted();
  void setStopped();

  bool isStarted();  
  bool isStopped();
  void reset();
  void setDroneState(DroneState s);
  bool isToBeStopped();
  void setToBeStopped(bool val);
  DroneState getDroneState();

private:
  DroneState state;
  bool started; 
  bool stopped;
  bool toBeStopped;
};

#endif
