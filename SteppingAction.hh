#ifndef SteppingAction_h
#define SteppingAction_h

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4Step;

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction(G4double angleThreshold);
    virtual ~SteppingAction() = default;

    virtual void UserSteppingAction(const G4Step* step) override;

    G4int GetLargeAngleCount() const;

private:
    G4double fAngleThreshold;
    G4int fLargeAngleCount;
};

#endif
