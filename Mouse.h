#ifndef MOUSE_H
#define MOUSE_H

#include "Org.h"
#include "World.h"

/**
 * @brief Mouse class representing herbivore prey species
 * 
 * Mice are species 0 in the ecosystem. They feed on grass (empty cells)
 * and serve as prey for owls. They reproduce more frequently than owls
 * and require grass to gain energy.
 */
class Mouse : public Organism {
    private:
        static constexpr double GRASS_BONUS_PER_CELL = 75.0;  ///< Energy gained per grass cell
        static constexpr double METABOLISM_COST = 50.0;       ///< Energy lost per turn
        static constexpr double REPRODUCTION_THRESHOLD = 800.0; ///< Energy needed to reproduce
        static constexpr double OFFSPRING_ENERGY = 300.0;      ///< Starting energy for offspring
        static constexpr double REPRODUCTION_COST = 700.0;     ///< Energy cost of reproduction

    public:
        /**
         * @brief Construct a new Mouse
         * @param _random Pointer to random number generator
         * @param _points Initial energy points
         * @param _species Species identifier (should be 0 for mouse)
         */
        Mouse(emp::Ptr<emp::Random> _random, double _points=0.0, int _species=0) :
            Organism(_random, _points, _species) {}

        /**
         * @brief Process mouse behavior in the world
         * @param world Reference to the world
         * @param pos Current position of the mouse
         */
        void ProcessInWorld(OrgWorld& world, size_t pos) override {
            // Check for nearby grass and gain energy
            auto neighbors = world.CheckNeighbors(pos);
            int grass_count = neighbors[2]; // Index 2 is grass count
            
            if (grass_count > 0) {
                double grass_bonus = CalculateGrassBonus(grass_count);
                AddPoints(grass_bonus);
            }

            // Apply metabolism cost
            ApplyMetabolism();
            
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
         * @brief Check if mouse should reproduce
         * @return True if energy >= reproduction threshold
         */
        bool ShouldReproduce() const override {
            return GetPoints() >= REPRODUCTION_THRESHOLD;
        }

        /**
         * @brief Create a mouse offspring
         * @return Pointer to new mouse with starting energy
         */
        emp::Ptr<Organism> CreateOffspring() override {
            return emp::Ptr<Mouse>(new Mouse(random, OFFSPRING_ENERGY, species));
        }

        /**
         * @brief Get reproduction cost for mice
         * @return Energy cost of creating offspring
         */
        double GetReproductionCost() const override {
            return REPRODUCTION_COST;
        }

        /**
         * @brief Apply metabolism (energy loss per turn)
         */
        void ApplyMetabolism() override {
            AddPoints(-METABOLISM_COST);
        }

        /**
         * @brief Function to grow faster if there is green space next to it
         * @param world Reference to the world
         * @param pos Current position of the mouse
         * @deprecated Use ProcessInWorld instead
         */
        void EatGrass(OrgWorld& world, size_t pos) {
            // Legacy method - functionality moved to ProcessInWorld
            ProcessInWorld(world, pos);
        }

    private:
        /**
         * @brief Calculate energy gained from nearby grass
         * @param grass_count Number of grass cells adjacent to mouse
         * @return Energy points gained from grass
         */
        double CalculateGrassBonus(int grass_count) const {
            return grass_count * GRASS_BONUS_PER_CELL;
        }

        /**
         * @brief Attempt to place offspring in nearby empty cell
         * @param world Reference to the world
         * @param pos Current position of parent
         * @param offspring Pointer to offspring to place
         * @return True if offspring was successfully placed
         */
        bool PlaceOffspring(OrgWorld& world, size_t pos, emp::Ptr<Organism> offspring) {
            // Try to place offspring in neighboring cells - use grid dimensions
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