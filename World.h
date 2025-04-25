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
    }

    void Update() {
        emp::World<Organism>::Update();
        
        // Give all organisms resources in random order
        emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());
        for (size_t i : schedule) {
            if(!IsOccupied(i)) {continue;}
            pop[i]->Process(100.0); // Give 100 points
        }
        
        // Check reproduction in random order
        emp::vector<size_t> repro_schedule = emp::GetPermutation(random, GetSize());
        for (size_t i : repro_schedule) {
            if(!IsOccupied(i)) {continue;}
            emp::Ptr<Organism> offspring = pop[i]->CheckReproduction();
            if(offspring) {
                DoBirth(*offspring, i); // i is the parent's position
            }
        }
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
    // For any cells that it doesn't return, we know these are grass/empty
    std::array<int, 3> CheckNeighbors(size_t i) {
        emp::vector<size_t> neighbor_list = GetValidNeighborOrgIDs(i);
        int mouse_count = 0;
        int owl_count = 0;
        int grass_count = 8;
        for (size_t j : neighbor_list) {
            if (pop[j]->GetSpecies() == 0) { // If it's a mouse
                mouse_count++;
                grass_count--;
            } else if (pop[j]->GetSpecies() == 1) { // If it's an owl
                owl_count++;
                grass_count--;
            }
        }
        std::array<int, 3> output = {mouse_count, owl_count, grass_count};
        return output;
    }
};
#endif