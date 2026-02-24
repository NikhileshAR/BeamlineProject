#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
    G4ParticleGun* fParticleGun;
    PrimaryGenerator();
    void GeneratePrimaries(G4Event* event) override;
};
