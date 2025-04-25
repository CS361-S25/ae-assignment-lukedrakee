// will be species 0
// grows fast on its own if there is green space next to it?
// would it need a grass class, or would it be able to just identify the green space?
// grows faster if more green space is next to it

#ifndef MOUSE_H
#define MOUSE_H

#include "Org.h"
#include "World.h"

class Mouse : public Organism {
    private:
        double points;
        emp::Ptr<emp::Random> random; // is this needed? copilot
        int species;
    public:
        Mouse(emp::Ptr<emp::Random> _random, double _points=0.0, int _species=0) :
            Organism(_random, _points, _species) {;}

        void Process(double points_to_add=100.0) {
            AddPoints(points_to_add);
        }

        emp::Ptr<Mouse> CheckReproduction() {
            if (GetPoints() >= 1000.0) {
                emp::Ptr<Mouse> offspring = new Mouse(*this);
                offspring->SetPoints(0.0); // Reset offspring points to 0

                // Parent pays the cost of reproduction
                SetPoints(GetPoints() - 1000.0);

                return offspring;
            }
            return nullptr; // Return nullptr if no reproduction occurs
        }

        // Function to grow faster if there is green space next to it
        void eatGrass() {
            // Depending on how much grass is next to it, it will grow faster
        }
};


#endif