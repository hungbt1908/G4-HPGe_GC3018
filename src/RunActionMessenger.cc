#include "RunActionMessenger.hh"

#include "RunAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

RunActionMessenger::RunActionMessenger(RunAction* pRunAction)
  : G4UImessenger(), fRunAction(pRunAction)
{
  fFileNameCmd = new G4UIcmdWithAString("/output/FileName", this);
  fFileNameCmd->SetGuidance("Set output file name");
  fFileNameCmd->SetParameterName("PList",false);
  fFileNameCmd->AvailableForStates(G4State_PreInit);
}

RunActionMessenger::~RunActionMessenger()
{
  delete fFileNameCmd;
}

void RunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command == fFileNameCmd)
  {
    fRunAction->SetFilename(newValue);
  }
}
