#include <iostream>
#include "emp/math/Random.hpp"

#include "World.h"
#include "Org.h"
#include "Mouse.h"
#include "Owl.h"

// You run this from going "./compile-run-native.sh" in the terminal.

int main() {
    emp::Random random(5);  
    OrgWorld world(random);
    
    Mouse* new_org1 = new Mouse(&random, 0.0, 0); // Create organism of species 0
    world.Inject(*new_org1);
    
    Owl* new_org2 = new Owl(&random, 0.0, 1); // Create organism of species 1
    world.Inject(*new_org2);
    
    // Resize the world to a 10x10 grid
    world.Resize(10, 10);
    world.SetPopStruct_Grid(10, 10); // Set population structure to grid for spatial constraints
    
    std::cout << "World size: " << world.GetSize() << std::endl;
    std::cout << "Number of organisms: " << world.GetNumOrgs() << std::endl;
    
    std::cout << "Running simulation for 10 updates..." << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << "Update " << i << std::endl;
        world.Update();
        std::cout << "Population after update " << i << ": " << world.GetNumOrgs() << std::endl;
        
        // Movement test
        /*for (int j = 0; j < world.GetSize(); j++) {
            if (world.IsOccupied(j) && random.P(0.5)) {
                world.MoveOrganism(j);
            }
        }*/
    }
    
    return 0;
}