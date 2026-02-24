#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4NistManager.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGenerator.hh"
#include "PhysicsList.hh"
#include "SteppingAction.hh"

#include <iostream>
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;

int main() {
    G4double targetThickness;
    std::cout << "Enter target thickness as fraction of X0 (0.01 - 0.5): ";
    std::cin >> targetThickness;

    // Aluminium X0
    auto alMat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
    G4double X0 = alMat->GetRadlen();
    targetThickness *= X0;

    std::cout << "Target thickness = " << targetThickness/mm << " mm" << std::endl;

    // Number of events
    G4int nEvents = 10000;

    // Analysis
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->CreateH1("ScatteringAngle","Scattering Angle (deg)", 180, 0, 180);
    analysisManager->OpenFile("output.root");

// Run manager
    G4RunManager* runManager = new G4RunManager();
    runManager->SetUserInitialization(new DetectorConstruction(targetThickness));
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserAction(new PrimaryGenerator());
    runManager->SetUserAction(new SteppingAction());

    runManager->Initialize();
    runManager->BeamOn(nEvents);

    analysisManager->Write();
    analysisManager->CloseFile();

    std::cout << "Simulation done. Histogram saved to output.root" << std::endl;
    delete runManager;
    return 0;
}
