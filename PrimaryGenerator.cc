#include "PrimaryGenerator.hh"
#include "G4ParticleTable.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

PrimaryGenerator::PrimaryGenerator() {
    fParticleGun = new G4ParticleGun(1);
}

void PrimaryGenerator::GeneratePrimaries(G4Event* event) {
    auto particleTable = G4ParticleTable::GetParticleTable();
    auto electron = particleTable->FindParticle("e-");
    fParticleGun->SetParticleDefinition(electron);
    fParticleGun->SetParticleEnergy(500*MeV);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
    fParticleGun->SetParticlePosition(G4ThreeVector(0,0,-0.5*m));
    fParticleGun->GeneratePrimaryVertex(event);
}
