#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

#include <cmath>

SteppingAction::SteppingAction(G4double angleThreshold)
: fAngleThreshold(angleThreshold),
  fLargeAngleCount(0)
{}

G4int SteppingAction::GetLargeAngleCount() const {
    return fLargeAngleCount;
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto track = step->GetTrack();

    // Only primary electron
    if(track->GetTrackID() != 1) return;

    auto pre  = step->GetPreStepPoint();
    auto post = step->GetPostStepPoint();

    if(!pre || !post) return;

    if(pre->GetPhysicalVolume()->GetName() == "TargetPhys" &&
       (post->GetPhysicalVolume() == nullptr ||
        post->GetPhysicalVolume()->GetName() != "TargetPhys"))
    {
        // Angle relative to beam direction
        G4ThreeVector beamDir(0,0,1);
        auto p_out = post->GetMomentumDirection();

        double cosTheta = beamDir.dot(p_out);
        if(cosTheta > 1.0) cosTheta = 1.0;
        if(cosTheta < -1.0) cosTheta = -1.0;

        double thetaRad = std::acos(cosTheta);
        double thetaDeg = thetaRad / CLHEP::deg;

        double energyMeV = post->GetKineticEnergy() / CLHEP::MeV;

        auto analysisManager = G4AnalysisManager::Instance();

        // Fill both histograms
        analysisManager->FillH1(0, thetaDeg);
        analysisManager->FillH1(1, thetaDeg);

        // Fill ntuple
        analysisManager->FillNtupleDColumn(0, thetaDeg);
        analysisManager->FillNtupleDColumn(1, energyMeV);
        analysisManager->AddNtupleRow();

        // Large angle counting
        if(thetaRad > fAngleThreshold)
            fLargeAngleCount++;
    }
}
