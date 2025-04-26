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
                emp::vector<size_t> neighbor_positions = world.GetValidNeighborOrgIDs(pos);
                
                // Find a mouse to eat
                for (size_t neighbor_pos : neighbor_positions) {
                    if (world.IsOccupied(neighbor_pos) && 
                        world.GetOrg(neighbor_pos).GetSpecies() == 0) {
                        
                        // Get 50% of the points from the mouse
                        double mouse_points = world.GetOrg(neighbor_pos).GetPoints() * 0.5;
                        AddPoints(mouse_points);
                        ate_mouse = true;
                        
                        // Move to the mouse's position and remove the mouse
                        emp::Ptr<Organism> owl = world.ExtractOrganism(pos);
                        world.RemoveOrganism(neighbor_pos); // Remove mouse
                        world.AddOrgAt(owl, neighbor_pos);  // Move owl to mouse position
                        
                        // Only eat one mouse per update
                        break;
                    }
                }
            }
            
            // If owls eat mice, then they lose less points
            if (!ate_mouse) {
                AddPoints(-15.0);
            } else {
                AddPoints(-5.0);
            }
            
            // if enough points, reproduce
            if (GetPoints() >= 1500.0) {
                emp::Ptr<Owl> offspring = new Owl(*this);
                offspring->SetPoints(400.0);
                
                SetPoints(GetPoints() - 1000.0);
                
                // Find empty spot for offspring
                emp::vector<size_t> neighbors = world.GetValidNeighborOrgIDs(pos);
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