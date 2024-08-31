#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <ctime>

class G4Run;
class RunActionMessenger;

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    ~RunAction();

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;
    void SetFilename(G4String fileName);
    double diffclock(clock_t clock1, clock_t clock2);

private:
    G4String fFileName;
    G4String beginTime, endTime;
    clock_t begin;
    clock_t end;
    RunActionMessenger* fRunActionMessenger;
};
#endif

