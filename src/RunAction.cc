#include "RunAction.hh"
#include "RunActionMessenger.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

double RunAction::diffclock(clock_t clock1, clock_t clock2)
{
	double diffticks = clock1-clock2;
	double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
	return diffms;
}

RunAction::RunAction()
    : G4UserRunAction()
{
    fRunActionMessenger = new RunActionMessenger(this);
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleMerging(true);

    // Create histogram
    analysisManager->CreateH1("H1", "Edep in absorber, Energy[MeV]", 2000, 0., 2.0*MeV);

    // Create ntuple
    analysisManager->CreateNtuple("HPGe", "Edep");
    analysisManager->CreateNtupleDColumn("fEdep");
    analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
    delete fRunActionMessenger;
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
    // get time
    time_t beginnow = time(0);
    beginTime = asctime(localtime(&beginnow));
    begin = clock();

    auto analysisManager = G4AnalysisManager::Instance();

    // Open an output file
    //
    if(fFileName.size()) // or whatever check is necessesary :)
        analysisManager->OpenFile(fFileName);
    else
        analysisManager->OpenFile("defaultFilename.root");

    // G4cout << "Using " << analysisManager->GetType() << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
    // get time
    time_t endnow = time(0);
    endTime = asctime(localtime(&endnow));
    end = clock();

    int numberOfWorkerThreads = G4Threading::GetNumberOfRunningWorkerThreads();
    G4RunManager* runManager = G4RunManager::GetRunManager();
    int nofEvents = runManager->GetNumberOfEventsToBeProcessed();
    if(IsMaster())
    {
        G4cout << "                                                                                      " << G4endl;
        G4cout << "                                                                                      " << G4endl;
        G4cout << "                              ==> GEANT4 OUTPUT REPORT <==                            " << G4endl;
        G4cout << "  __________________________________ [ Job Title ] _________________________________  " << G4endl;
        G4cout << " |                                                                                  | " << G4endl;
        G4cout << " | Application for HPGe simulation                                                  | " << G4endl;
        G4cout << " | -------------------------------------------------------------------------------- | " << G4endl;
        G4cout << " | author: BUI Tien Hung                                                            | " << G4endl;
        G4cout << " | email : hungbt1908@gmail.com                                                     | " << G4endl;
        G4cout << " | adress: Institute for Nuclear Science and Technology (INST)                      | " << G4endl;
        G4cout << " |__________________________________________________________________________________| " << G4endl;
        G4cout << "                                                                                      " << G4endl;
        G4cout << "                         Starting = " << beginTime                                      << G4endl;
        G4cout << "                         Ending   = " << endTime                                        << G4endl;
        G4cout << "                                                                                      " << G4endl;
        G4cout << " # Number of worker threads        : " << numberOfWorkerThreads << " threads"           << G4endl;
        G4cout << " # Number of primary events        : " << nofEvents             << " events"            << G4endl;
        G4cout << " # Elapsed time                    : " << (double(diffclock(end, begin)/1000)) <<  " s" << G4endl;
        G4cout << "                                                                                      " << G4endl;
        G4cout << "                                                                                      " << G4endl;
    }

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}

void RunAction::SetFilename(G4String fileName) 
{
    fFileName = G4String(fileName);
}