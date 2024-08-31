#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class EventAction;
class DetectorConstruction;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(EventAction* eventAction);
  ~SteppingAction() override = default;

  void UserSteppingAction(const G4Step* step) override;

private:
  const DetectorConstruction* detectorConstruction = nullptr;
  EventAction* fEventAction = nullptr;
};

#endif
