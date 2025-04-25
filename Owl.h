// will be species 1
// if a mouse is in an adjacent cell, then owl will select one and eat it
// have owl move to the cell of the mouse it eats

#ifndef OWL_H
#define OWL_H

#include "Org.h"
#include "World.h"

class Owl : public Organism {
    private:
        double points;
        emp::Ptr<emp::Random> random; // is this needed? copilot
        int species;

    public:
        Owl(emp::Ptr<emp::Random> _random, double _points=0.0, int _species=1) :
            Organism(_random, _points, _species) {;}

        void Process(double points_to_add=100.0) {
            AddPoints(points_to_add);
        }

        emp::Ptr<Owl> CheckReproduction() {
            if (GetPoints() >= 1000.0) {
                emp::Ptr<Owl> offspring = new Owl(*this);
                offspring->SetPoints(0.0); // Reset offspring points to 0

                // Parent pays the cost of reproduction
                SetPoints(GetPoints() - 1000.0);

                return offspring;
            }
            return nullptr; // Return nullptr if no reproduction occurs
        }

        // Function to eat a mouse
        void EatMouse(Organism* mouse) {
            if (mouse->GetSpecies() == 0) { // Assuming species 0 is mouse
                AddPoints(mouse->GetPoints()); // Add mouse points to owl
                // Remove mouse from the world
                // Assuming you have a reference to the world
                // world.RemoveOrganism(mouse); // This function should be defined in your world class
            }
        }
};
    

#endif