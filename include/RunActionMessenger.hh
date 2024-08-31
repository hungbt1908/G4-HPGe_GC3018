#ifndef RunActionMessenger_h
#define RunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PhysicsList;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class RunAction;

class RunActionMessenger : public G4UImessenger
{
  public:
    RunActionMessenger(RunAction* pRunAction);
    virtual ~RunActionMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    RunAction* fRunAction;

    G4UIcmdWithAString* fFileNameCmd;
}; 
#endif