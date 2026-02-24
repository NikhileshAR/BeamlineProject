#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

class SteppingAction : public G4UserSteppingAction {
public:
    void UserSteppingAction(const G4Step* step) override;
};
