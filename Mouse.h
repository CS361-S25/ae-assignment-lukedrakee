#ifndef MOUSE_H
#define MOUSE_H

#include "Org.h"
#include "World.h"

// Mouse is species 0
class Mouse : public Organism {
    public:
        Mouse(emp::Ptr<emp::Random> _random, double _points=0.0, int _species=0) :
            Organism(_random, _points, _species) {;}

        void Process(double points_to_add=100.0) {
            AddPoints(points_to_add);
        }

        // Function to grow faster if there is green space next to it
        void EatGrass(OrgWorld& world, size_t pos) {
            // Check how much grass is nearby
            auto neighbors = world.CheckNeighbors(pos);
            int grass_count = neighbors[2]; // Index 2 is grass count
            
            // The more grass, the more points gained
            if (grass_count > 0) {
                double grass_bonus = grass_count * 75.0;
                AddPoints(grass_bonus);
            }

            // Mice use energy each round for existing
            AddPoints(-50.0);
            
            if (GetPoints() >= 800.0) {  
                emp::Ptr<Mouse> offspring = new Mouse(*this);
                offspring->SetPoints(300.0);
                
                // Parent keeps some points
                SetPoints(GetPoints() - 700.0);
                
                // Add offspring to a nearby cell if possible using direct coordinate calculation
                std::vector<size_t> neighbors = world.GetNeighborPositions(pos, 20, 20); // Use grid dimensions
                for (size_t neighbor_pos : neighbors) {
                    if (!world.IsOccupied(neighbor_pos)) {
                        world.AddOrgAt(offspring, neighbor_pos);
                        break;
                    }
                }
            }
        }
};

#endif