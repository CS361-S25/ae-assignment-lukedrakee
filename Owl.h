#ifndef OWL_H
#define OWL_H

#include "Org.h"
#include "World.h"

/**
 * @brief Owl class representing carnivore predator species
 * 
 * Owls are species 1 in the ecosystem. They hunt mice for energy
 * and have higher reproduction thresholds than mice, maintaining
 * the predator-prey balance in the simulation.
 */
class Owl : public Organism {
    private:
        static constexpr double HUNT_SUCCESS_RATE = 0.2;       ///< Fraction of mouse energy gained when hunting
        static constexpr double STARVATION_COST = 100.0;       ///< Energy lost when no prey found
        static constexpr double HUNTING_COST = 50.0;           ///< Energy cost of successful hunt
        static constexpr double REPRODUCTION_THRESHOLD = 2500.0; ///< Energy needed to reproduce
        static constexpr double OFFSPRING_ENERGY = 400.0;      ///< Starting energy for offspring
        static constexpr double REPRODUCTION_COST = 2000.0;    ///< Energy cost of reproduction

    public:
        /**
         * @brief Construct a new Owl
         * @param _random Pointer to random number generator
         * @param _points Initial energy points
         * @param _species Species identifier (should be 1 for owl)
         */
        Owl(emp::Ptr<emp::Random> _random, double _points=0.0, int _species=1) :
            Organism(_random, _points, _species) {}

        /**
         * @brief Process owl behavior in the world
         * @param world Reference to the world
         * @param pos Current position of the owl
         */
        void ProcessInWorld(OrgWorld& world, size_t pos) override {
            // Find nearby mice to hunt
            std::vector<size_t> nearby_mice = FindNearbyMice(world, pos);
            
            bool ate_mouse = false;
            if (!nearby_mice.empty()) {
                // Randomly select a mouse to hunt
                size_t random_index = random->GetUInt(nearby_mice.size());
                size_t target_mouse_pos = nearby_mice[random_index];
                ate_mouse = HuntMouse(world, pos, target_mouse_pos);
            }
            
            // Apply energy costs based on hunting success
            if (!ate_mouse) {
                AddPoints(-STARVATION_COST);
            } else {
                AddPoints(-HUNTING_COST);
            }
            
            // Handle reproduction if conditions are met
            if (ShouldReproduce()) {
                emp::Ptr<Organism> offspring = CreateOffspring();
                if (PlaceOffspring(world, pos, offspring)) {
                    // Deduct reproduction cost from parent
                    AddPoints(-GetReproductionCost());
                }
            }
        }

        /**
         * @brief Check if owl should reproduce
         * @return True if energy >= reproduction threshold
         */
        bool ShouldReproduce() const override {
            return GetPoints() >= REPRODUCTION_THRESHOLD;
        }

        /**
         * @brief Create an owl offspring
         * @return Pointer to new owl with starting energy
         */
        emp::Ptr<Organism> CreateOffspring() override {
            return emp::Ptr<Owl>(new Owl(random, OFFSPRING_ENERGY, species));
        }

        /**
         * @brief Get reproduction cost for owls
         * @return Energy cost of creating offspring
         */
        double GetReproductionCost() const override {
            return REPRODUCTION_COST;
        }

        /**
         * @brief Apply metabolism (handled during hunting)
         */
        void ApplyMetabolism() override {
            // Metabolism is handled in hunting behavior
        }

        /**
         * @brief Function to hunt for mice in nearby cells
         * @param world Reference to the world
         * @param pos Current position of the owl
         * @deprecated Use ProcessInWorld instead
         */
        void Hunt(OrgWorld& world, size_t pos) {
            // Legacy method - functionality moved to ProcessInWorld
            ProcessInWorld(world, pos);
        }

    private:
        /**
         * @brief Find all mice in neighboring cells
         * @param world Reference to the world
         * @param pos Current position of the owl
         * @return Vector of positions containing mice
         */
        std::vector<size_t> FindNearbyMice(OrgWorld& world, size_t pos) {
            std::vector<size_t> mouse_positions;
            std::vector<size_t> neighbor_positions = world.GetNeighborPositions(pos, 20, 20);
            
            for (size_t neighbor_pos : neighbor_positions) {
                if (world.IsOccupied(neighbor_pos) && 
                    world.GetOrg(neighbor_pos).GetSpecies() == 0) {
                    mouse_positions.push_back(neighbor_pos);
                }
            }
            
            return mouse_positions;
        }

        /**
         * @brief Attempt to hunt a specific mouse
         * @param world Reference to the world
         * @param owl_pos Current position of the owl
         * @param mouse_pos Position of target mouse
         * @return True if hunt was successful
         */
        bool HuntMouse(OrgWorld& world, size_t owl_pos, size_t mouse_pos) {
            // Calculate energy gained from the mouse
            double mouse_energy = world.GetOrg(mouse_pos).GetPoints();
            double energy_gained = CalculateHuntReward(mouse_energy);
            AddPoints(energy_gained);
            
            // Move owl to mouse position and remove the mouse
            emp::Ptr<Organism> owl = world.ExtractOrganism(owl_pos);
            world.RemoveOrganism(mouse_pos);
            world.AddOrgAt(owl, mouse_pos);
            
            return true;
        }

        /**
         * @brief Calculate energy gained from hunting a mouse
         * @param mouse_energy Energy of the hunted mouse
         * @return Energy points gained by the owl
         */
        double CalculateHuntReward(double mouse_energy) const {
            return mouse_energy * HUNT_SUCCESS_RATE;
        }

        /**
         * @brief Attempt to place offspring in nearby empty cell
         * @param world Reference to the world
         * @param pos Current position of parent
         * @param offspring Pointer to offspring to place
         * @return True if offspring was successfully placed
         */
        bool PlaceOffspring(OrgWorld& world, size_t pos, emp::Ptr<Organism> offspring) {
            // Try to place offspring in neighboring cells
            std::vector<size_t> neighbors = world.GetNeighborPositions(pos, 20, 20);
            
            for (size_t neighbor_pos : neighbors) {
                if (!world.IsOccupied(neighbor_pos)) {
                    world.AddOrgAt(offspring, neighbor_pos);
                    return true;
                }
            }
            
            // If no empty neighbors found, reproduction fails
            return false;
        }
};

#endif