#include "PhysicsList.hh"
#include "G4EmStandardPhysics.hh"

PhysicsList::PhysicsList() {
    RegisterPhysics(new G4EmStandardPhysics());
}
