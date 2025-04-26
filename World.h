#ifndef WORLD_H
#define WORLD_H

#include "emp/Evolve/World.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include <array>

#include "Org.h"

// Species 0 is mouse, species 1 is owl

class OrgWorld : public emp::World<Organism> {

    emp::Random &random;
    emp::Ptr<emp::Random> random_ptr;

    public:

    OrgWorld(emp::Random &_random) : emp::World<Organism>(_random), random(_random) {
        random_ptr.New(_random);
    }

    ~OrgWorld() {
        random_ptr.Delete();
    }

    
    // Extract organism from the population
    emp::Ptr<Organism> ExtractOrganism(size_t i) {
        if (!IsOccupied(i)) return nullptr;
        
        emp::Ptr<Organism> org = pop[i];
        pop[i] = nullptr;
        return org;
    }
    
    // Move organism to a random neighbor position
    bool MoveOrganism(size_t i) {
        if (!IsOccupied(i)) return false;
        
        emp::WorldPosition new_pos = GetRandomNeighborPos(i);
        if (!new_pos.IsValid()) return false;
        
        // Only move if the new position is empty
        if (!IsOccupied(new_pos)) {
            emp::Ptr<Organism> org = ExtractOrganism(i);
            AddOrgAt(org, new_pos);
            return true;
        }
        
        return false;
    }

    // Remove organism from the world
    void RemoveOrganism(size_t i) {
        if (IsOccupied(i)) {
            pop[i] = nullptr;
        }
    }
    
    // Check all of the surrounding cells for mice, grass, or owls
    // Returns array with [mouse_count, owl_count, grass_count]
    std::array<int, 3> CheckNeighbors(size_t i) {
        emp::vector<size_t> neighbor_ids = GetValidNeighborOrgIDs(i);
        int mouse_count = 0;
        int owl_count = 0;
        int grass_count = neighbor_ids.size(); // Start with all cells as grass
        
        for (size_t j : neighbor_ids) {
            if (IsOccupied(j)) {
                if (pop[j]->GetSpecies() == 0) { // If it's a mouse
                    mouse_count++;
                    grass_count--;
                } else if (pop[j]->GetSpecies() == 1) { // If it's an owl
                    owl_count++;
                    grass_count--;
                }
            }
        }
        
        std::array<int, 3> output = {mouse_count, owl_count, grass_count};
        return output;
    }
};
#endif