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
  DroneState getDroneState();

private:
  DroneState state;
  bool started; 
  bool stopped;
};

#endif
