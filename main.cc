#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4NistManager.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGenerator.hh"
#include "PhysicsList.hh"
#include "SteppingAction.hh"

#include <iostream>
#include "G4SystemOfUnits.hh"
#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;

int main()
{
    // ==============================
    // Ask for target thickness
    // ==============================

    G4double thicknessFraction;
    std::cout << "Enter target thickness as fraction of X0 (0.01 - 0.5): ";
    std::cin >> thicknessFraction;

    auto alMat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
    G4double X0 = alMat->GetRadlen();

    G4double targetThickness = thicknessFraction * X0;

    std::cout << "Target thickness = "
              << targetThickness/mm << " mm\n" << std::endl;

    // ==============================
    // Ask for large-angle threshold
    // ==============================

    G4double thresholdDeg;
    std::cout << "Enter threshold angle (degrees): ";
    std::cin >> thresholdDeg;

    G4double thresholdRad = thresholdDeg * deg;

    // ==============================
    // Number of events
    // ==============================

    G4int nEvents = 10000;

    // ==============================
    // Analysis Manager
    // ==============================

    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->CreateH1(
        "ScatteringAngle_Full",
        "Scattering Angle (Full);Theta (deg);Counts",
        180, 0, 180);

    analysisManager->CreateH1(
        "ScatteringAngle",
        "Scattering Angle;Theta (deg);Probability",
        200, 0, 60);

    analysisManager->CreateNtuple("scattering", "Scattering Data");
    analysisManager->CreateNtupleDColumn("theta");   // Column 0
    analysisManager->CreateNtupleDColumn("energy");  // Column 1
    analysisManager->FinishNtuple();

    analysisManager->OpenFile("output.root");

    // ==============================
    // Run Manager
    // ==============================

    auto runManager = new G4RunManager();

    runManager->SetUserInitialization(
        new DetectorConstruction(targetThickness));

    runManager->SetUserInitialization(
        new PhysicsList());

    runManager->SetUserAction(
        new PrimaryGenerator());

    auto steppingAction =
        new SteppingAction(thresholdRad);

    runManager->SetUserAction(steppingAction);

    // ==============================
    // Run Simulation
    // ==============================

    runManager->Initialize();
    runManager->BeamOn(nEvents);

    // ==============================
    // Print Large Angle Results
    // ==============================

    G4int largeCount =
        steppingAction->GetLargeAngleCount();

    std::cout << "\n===== Large Angle Results =====" << std::endl;
    std::cout << "Total events: " << nEvents << std::endl;
    std::cout << "Events with theta > "
              << thresholdDeg << " deg: "
              << largeCount << std::endl;
    std::cout << "Probability: "
              << (double)largeCount / nEvents << std::endl;
    std::cout << "================================\n" << std::endl;

    // ==============================
    // Normalize histograms
    // ==============================

    analysisManager->ScaleH1(0, 1.0/nEvents);
    analysisManager->ScaleH1(1, 1.0/nEvents);

    // ==============================
    // Save Output
    // ==============================

    analysisManager->Write();
    analysisManager->CloseFile();

    std::cout << "Simulation done. Histogram saved to output.root"
              << std::endl;

    delete runManager;
    return 0;
}
