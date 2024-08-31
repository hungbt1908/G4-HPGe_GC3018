#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class G4Electron;
class G4Gamma;
class G4Positron;
class G4PhysicsListHelper;
class G4ParticleDefinition;
class G4PhotoElectricEffect;
class G4ComptonScattering;
class G4GammaConversion;
class G4eMultipleScattering;
class G4eIonisation;
class G4eBremsstrahlung;
class G4eplusAnnihilation;

class PhysicsList : public G4VUserPhysicsList
{
public:
	PhysicsList();
	virtual ~PhysicsList();
	void ConstructParticle(); // construction of particles
	void ConstructProcess(); // construct processes and register them to particles
	void ConstructEM();
	void ConstructGeneral();
	void SetCuts(); // setting a range cut value for all particles
	void SetGammaCut(G4double);
	void SetElectronCut(G4double);
	void SetPositronCut(G4double);
	//void SetProtonCut(G4double);

private:
	G4double fCutForGamma;
	G4double fCutForElectron;
	G4double fCutForPositron;
	//G4double fCutForProton;
};
#endif