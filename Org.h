#ifndef ORG_H
#define ORG_H

#include "emp/math/Random.hpp"
#include "emp/tools/string_utils.hpp"

class Organism {
    private:
        double points;
        emp::Ptr<emp::Random> random;
        int species;

    public:
    Organism(emp::Ptr<emp::Random> _random, double _points=0.0, int _species=0) :
        points(_points), random(_random), species(_species) {;}

    void SetPoints(double _in) {points = _in;}
    void AddPoints(double _in) {points += _in;}
    double GetPoints() const {return points;}
    int GetSpecies() const {return species;}

    void Process(double points_to_add=100.0) {
        AddPoints(points_to_add);
    }

    emp::Ptr<Organism> CheckReproduction() {
        if (points >= 1000.0) {
            emp::Ptr<Organism> offspring = new Organism(*this);
            offspring->SetPoints(0.0); // Reset offspring points to 0
            
            // Parent pays the cost of reproduction
            points -= 1000.0;
            
            return offspring;
        }
        return nullptr; // Return nullptr if no reproduction occurs
    }
};
#endif