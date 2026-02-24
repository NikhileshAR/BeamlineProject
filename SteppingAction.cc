#include "SteppingAction.hh"
#include "G4AnalysisManager.hh"
#include <cmath>

#include "G4SystemOfUnits.hh"
using namespace CLHEP;

void SteppingAction::UserSteppingAction(const G4Step* step) {
    auto pre  = step->GetPreStepPoint();
    auto post = step->GetPostStepPoint();

    // Check if particle is leaving the target
    if(pre->GetPhysicalVolume()->GetName() == "TargetPhys" &&
       (post->GetPhysicalVolume() == nullptr || post->GetPhysicalVolume()->GetName() != "TargetPhys")) {

        auto p_in  = pre->GetMomentumDirection();
        auto p_out = post->GetMomentumDirection();

        double cosTheta = p_in.dot(p_out);

        // Safety for acos
        if(cosTheta > 1.0) cosTheta = 1.0;
        if(cosTheta < -1.0) cosTheta = -1.0;

        double theta = std::acos(cosTheta); // radians

        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillH1(0, theta/CLHEP::deg); // store in degrees

        // Optional: print first few for debugging
        // G4cout << "Scattering angle (deg): " << theta/CLHEP::deg << G4endl;
    }
}
