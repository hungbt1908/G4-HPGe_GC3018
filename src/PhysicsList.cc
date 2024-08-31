#include "PhysicsList.hh"
#include "G4PhysicsListHelper.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
// photon
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
// e- & e+
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

PhysicsList::PhysicsList():G4VUserPhysicsList()
{
    defaultCutValue = 0.01 * MeV;
    fCutForGamma = 0.01 * MeV;
    fCutForElectron = 0.521 * MeV;
    fCutForPositron = 0.521 * MeV;
    //fCutForProton = defaultCutValue;

    SetVerboseLevel(1);
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructParticle()
{
	G4Electron::Definition();
	G4Gamma::Definition();
	G4Positron::Definition();
    //G4Proton::Definition();
    //G4AntiProton::Definition();
    //G4GenericIon::Definition();
}

void PhysicsList::ConstructProcess()
{
	// Method (provided by the G4VUserPhysicsList base class)
	// That assigns transortation process to all particles
	// Defined in ConstructParticle()
	AddTransportation();

	// Helper method might be defined by the user (for convenience)
	// To add electromagnetic physics processes
	ConstructEM();

	// Helper method might be defined by the user
	// To add all other physics processes
	ConstructGeneral();
}

void PhysicsList::ConstructEM()
{
	// Get the physics list helper
	// It will be used to assign processes to particles
	G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
	auto particleIterator = GetParticleIterator();
	particleIterator->reset();

    while ((*particleIterator)()) {

        G4ParticleDefinition* particle = particleIterator->value();

        G4String particleName = particle->GetParticleName();

        if (particleName == "gamma") {

            ph->RegisterProcess(new G4PhotoElectricEffect(), particle);
            ph->RegisterProcess(new G4ComptonScattering(), particle);
            ph->RegisterProcess(new G4GammaConversion(), particle);

        }
        else if (particleName == "e-") {

            ph->RegisterProcess(new G4eMultipleScattering(), particle);
            ph->RegisterProcess(new G4eIonisation(), particle);
            ph->RegisterProcess(new G4eBremsstrahlung(), particle);

        }
        else if (particleName == "e+") {

            ph->RegisterProcess(new G4eMultipleScattering(), particle);
            ph->RegisterProcess(new G4eIonisation(), particle);
            ph->RegisterProcess(new G4eBremsstrahlung(), particle);
            ph->RegisterProcess(new G4eplusAnnihilation(), particle);

        }
        /*else if (particleName == "mu+" ||
            particleName == "mu-") {

            G4MuMultipleScattering* msc = new G4MuMultipleScattering();
            msc->AddEmModel(0, new G4WentzelVIModel());

            ph->RegisterProcess(msc, particle);
            ph->RegisterProcess(new G4MuIonisation(), particle);
            ph->RegisterProcess(new G4MuBremsstrahlung(), particle);
            ph->RegisterProcess(new G4MuPairProduction(), particle);
            ph->RegisterProcess(new G4CoulombScattering(), particle);

        }
        else if (particleName == "alpha" ||
            particleName == "He3") {

            ph->RegisterProcess(new G4hMultipleScattering(), particle);
            ph->RegisterProcess(new G4ionIonisation(), particle);

        }
        else if (particleName == "GenericIon") {

            ph->RegisterProcess(new G4hMultipleScattering(), particle);
            ph->RegisterProcess(new G4ionIonisation(), particle);

        }
        else if (particleName == "proton") {
            ph->RegisterProcess(new G4hMultipleScattering(), particle);
            ph->RegisterProcess(new G4hIonisation(), particle);
            ph->RegisterProcess(new G4hBremsstrahlung(), particle);
            ph->RegisterProcess(new G4hPairProduction(), particle);

            ph->RegisterProcess(new G4StepLimiter(), particle);

        }*/
    }
}

void PhysicsList::ConstructGeneral()
{}

void PhysicsList::SetCuts()
{
    if (verboseLevel > 0) {
        G4cout << "PhysicsList::SetCuts:";
        G4cout << "Cut Energy for Gamma: " << G4BestUnit(fCutForGamma, "Energy") << G4endl;
        G4cout << "Cut Energy for Electron: " << G4BestUnit(fCutForElectron, "Energy") << G4endl;
        G4cout << "Cut Energy for Positron: " << G4BestUnit(fCutForPositron, "Energy") << G4endl;
        //G4cout << "Cut Energy for Proton: " << G4BestUnit(fCutForProton, "Energy") << G4endl;
    }

    SetCutValue(fCutForGamma, "gamma");
    SetCutValue(fCutForElectron, "e-");
    SetCutValue(fCutForPositron, "e+");
    //SetCutValue(fCutForProton, "proton");
    //SetCutValue(fCutForProton, "anti_proton");

    if (verboseLevel > 0) DumpCutValuesTable();
}

void PhysicsList::SetGammaCut(G4double val)
{
    fCutForGamma = val;
}

void PhysicsList::SetElectronCut(G4double val)
{
    fCutForElectron = val;
}

void PhysicsList::SetPositronCut(G4double val)
{
    fCutForPositron = val;
}

/*void PhysicsList::SetProtonCut(G4double val)
{
    fCutForProton = val;
}*/