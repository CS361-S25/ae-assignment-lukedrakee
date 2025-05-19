#ifndef WORLD_H
#define WORLD_H

#include "emp/Evolve/World.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include <array>
#include <vector>

#include "Org.h"

/**
 * @brief World class managing the ecosystem simulation
 * 
 * Manages organisms, their interactions, and environmental dynamics.
 * Provides a grid-based world where organisms can move, reproduce,
 * and interact with each other and their environment.
 */
class OrgWorld : public emp::World<Organism> {
    private:
        emp::Random &random;              ///< Reference to random number generator
        emp::Ptr<emp::Random> random_ptr; ///< Owned pointer to random generator
        
        static constexpr double MOVE_PROBABILITY = 0.2; ///< Chance organism moves each turn

    public:
        /**
         * @brief Construct a new OrgWorld
         * @param _random Reference to random number generator
         */
        OrgWorld(emp::Random &_random) : emp::World<Organism>(_random), random(_random) {
            random_ptr.New(_random);
        }

        /**
         * @brief Destructor - cleanup resources
         */
        ~OrgWorld() {
            random_ptr.Delete();
        }

        /**
         * @brief Update all organisms in the world for one simulation step
         * 
         * Processes each organism in random order, handling predation,
         * feeding, movement, reproduction, and death. This method was moved
         * from AEAnimate.cpp to centralize world management logic.
         */
        void UpdateEcology() {
            // Process each organism in random order
            emp::vector<size_t> action_schedule = emp::GetPermutation(random, GetSize());
            for (size_t i : action_schedule) {
                if (IsOccupied(i)) {
                    ProcessOrganism(i);
                }
            }
            
            // Remove dead organisms
            RemoveDeadOrganisms();
            
            // Move organisms randomly
            MoveOrganisms();
        }

        /**
         * @brief Extract organism from the population without deleting
         * @param i Position index
         * @return Pointer to extracted organism or nullptr if position empty
         */
        emp::Ptr<Organism> ExtractOrganism(size_t i) {
            if (!IsOccupied(i)) return nullptr;
            
            emp::Ptr<Organism> org = pop[i];
            pop[i] = nullptr;
            return org;
        }

        /**
         * @brief Move organism to a random neighboring position
         * @param i Current position of organism
         * @return True if organism was successfully moved
         */
        bool MoveOrganism(size_t i) {
            if (!IsOccupied(i)) return false;
            
            emp::WorldPosition new_pos = GetRandomNeighborPos(i);
            if (!new_pos.IsValid() || IsOccupied(new_pos)) {
                return false;
            }
            
            emp::Ptr<Organism> org = ExtractOrganism(i);
            AddOrgAt(org, new_pos);
            return true;
        }

        /**
         * @brief Remove organism from the world
         * @param i Position index
         */
        void RemoveOrganism(size_t i) {
            if (IsOccupied(i)) {
                pop[i] = nullptr;
            }
        }

        /**
         * @brief Count neighbors by type (mice, owls, grass)
         * @param i Position to check around
         * @return Array with [mouse_count, owl_count, grass_count]
         */
        std::array<int, 3> CheckNeighbors(size_t i) {
            emp::vector<size_t> neighbor_ids = GetValidNeighborOrgIDs(i);
            int mouse_count = 0;
            int owl_count = 0;
            int grass_count = 8; // Start with all cells as grass (8 neighbors)
            
            for (size_t j : neighbor_ids) {
                if (IsOccupied(j)) {
                    if (pop[j]->GetSpecies() == 0) { // Mouse
                        mouse_count++;
                        grass_count--;
                    } else if (pop[j]->GetSpecies() == 1) { // Owl
                        owl_count++;
                        grass_count--;
                    }
                }
            }
            
            std::array<int, 3> output = {mouse_count, owl_count, grass_count};
            return output;
        }

        /**
         * @brief Get positions of all neighboring cells
         * @param pos Center position
         * @param width Grid width
         * @param height Grid height
         * @return Vector of neighboring positions with toroidal wrapping
         */
        std::vector<size_t> GetNeighborPositions(size_t pos, int width, int height) {
            std::vector<size_t> neighbors;
            int x = pos % width;
            int y = pos / width;
            
            // Check all 8 neighbors with toroidal wrapping
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue; // Skip center position
                    
                    // Handle toroidal wrapping
                    int nx = (x + dx + width) % width;
                    int ny = (y + dy + height) % height;
                    
                    size_t neighbor_pos = ny * width + nx;
                    neighbors.push_back(neighbor_pos);
                }
            }
            return neighbors;
        }

    private:
        /**
         * @brief Process a single organism's behavior
         * @param pos Position of organism to process
         */
        void ProcessOrganism(size_t pos) {
            if (!IsOccupied(pos)) return;
            
            // Use the new ProcessInWorld method for polymorphic behavior
            pop[pos]->ProcessInWorld(*this, pos);
        }

        /**
         * @brief Remove dead organisms from the world
         */
        void RemoveDeadOrganisms() {
            for (size_t i = 0; i < GetSize(); i++) {
                if (IsOrganismDead(i)) {
                    RemoveOrganism(i);
                }
            }
        }

        /**
         * @brief Move organisms randomly based on movement probability
         */
        void MoveOrganisms() {
            for (size_t i = 0; i < GetSize(); i++) {
                if (IsOccupied(i) && random.P(MOVE_PROBABILITY)) {
                    MoveOrganism(i);
                }
            }
        }

        /**
         * @brief Check if organism at position is dead (points <= 0)
         * @param pos Position to check
         * @return True if organism should be removed
         */
        bool IsOrganismDead(size_t pos) {
            return IsOccupied(pos) && pop[pos]->GetPoints() <= 0;
        }
};

#endif