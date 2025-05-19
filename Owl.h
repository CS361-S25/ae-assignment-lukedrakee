#ifndef OWL_H
#define OWL_H

#include "Org.h"
#include "World.h"

// Owl is species 1
class Owl : public Organism {
    public:
        Owl(emp::Ptr<emp::Random> _random, double _points=0.0, int _species=1) :
            Organism(_random, _points, _species) {;}

        void Process(double points_to_add=100.0) {
            AddPoints(points_to_add);
        }

        // Function to hunt for mice in nearby cells
        void Hunt(OrgWorld& world, size_t pos) {
            auto neighbors = world.CheckNeighbors(pos);
            int mouse_count = neighbors[0]; // Index 0 is mouse count
            
            bool ate_mouse = false;
            if (mouse_count > 0) {
                // Get list of neighboring positions
                std::vector<size_t> neighbor_positions = world.GetNeighborPositions(pos, 20, 20);
                
                // First, collect all positions that have mice
                std::vector<size_t> mouse_positions;
                for (size_t neighbor_pos : neighbor_positions) {
                    if (world.IsOccupied(neighbor_pos) && 
                        world.GetOrg(neighbor_pos).GetSpecies() == 0) {
                        mouse_positions.push_back(neighbor_pos);
                    }
                }
                
                // If we found mice, randomly select one to hunt
                if (!mouse_positions.empty()) {
                    size_t random_index = random->GetUInt(mouse_positions.size());
                    size_t target_mouse_pos = mouse_positions[random_index];
                    
                    // Get 20% of the points from the selected mouse
                    double mouse_points = world.GetOrg(target_mouse_pos).GetPoints() * 0.2;
                    AddPoints(mouse_points);
                    ate_mouse = true;
                    
                    // Move to the mouse's position and remove the mouse
                    emp::Ptr<Organism> owl = world.ExtractOrganism(pos);
                    world.RemoveOrganism(target_mouse_pos); // Remove selected mouse
                    world.AddOrgAt(owl, target_mouse_pos);  // Move owl to mouse position
                }
            }
            
            // Owls lose more points when they don't eat (increased starvation pressure)
            if (!ate_mouse) {
                AddPoints(-100.0);
            } else {
                AddPoints(-50.0); // Hunting still has cost
            }
            
            // Much higher reproduction threshold for owls (dramatically increased)
            if (GetPoints() >= 2500.0) {
                emp::Ptr<Owl> offspring = new Owl(*this);
                offspring->SetPoints(400.0);
                
                // Parent loses much more points (increased cost)
                SetPoints(GetPoints() - 2000.0); // Increased from 1000.0
                
                // Find empty spot for offspring
                std::vector<size_t> neighbors = world.GetNeighborPositions(pos, 20, 20);
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