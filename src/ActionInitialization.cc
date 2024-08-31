#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    RunAction* runAction = new RunAction();
	SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
    PrimaryGeneratorAction* generator = new PrimaryGeneratorAction();
    SetUserAction(generator);

    RunAction* runAction = new RunAction();
    SetUserAction(runAction);

    EventAction* eventAction = new EventAction();
    SetUserAction(eventAction);

    SteppingAction* steppingAction = new SteppingAction(eventAction);
    SetUserAction(steppingAction);
}