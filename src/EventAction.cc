#define _USE_MATH_DEFINES

#include "EventAction.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"

#include "G4AnalysisManager.hh"

#include "Randomize.hh"
#include <cmath>
#include<math.h>

EventAction::EventAction()
{
	fEdep = 0.;
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
	// initialisation per event
	fEdep = 0.;
}

void EventAction::EndOfEventAction(const G4Event* event)
{
	// G4cout << "Energy deposition: " << fEdep << G4endl;

	// get analysis manager
	G4AnalysisManager* man = G4AnalysisManager::Instance();

	if (fEdep > 0)
	{
		fEdep = fEdep/MeV;

		// fill histograms
		man->FillH1(0, fEdep);

		// fill ntuple
		man->FillNtupleDColumn(0, fEdep);
		man->AddNtupleRow();
	}
}
