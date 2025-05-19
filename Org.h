#ifndef ORG_H
#define ORG_H

#include "emp/math/Random.hpp"
#include "emp/tools/string_utils.hpp"
#include <array>

// Forward declaration
class OrgWorld;

/**
 * @brief Base class for all organisms in the ecosystem
 * 
 * This abstract base class defines the common interface and behavior
 * for all organisms including mice and owls. It manages points (energy),
 * species identification, and provides virtual methods for processing.
 */
class Organism {
    protected:
        double points;                   ///< Energy/health points of the organism
        emp::Ptr<emp::Random> random;   ///< Random number generator for stochastic behaviors
        int species;                    ///< Species identifier (0=mouse, 1=owl)

    public:
        /**
         * @brief Construct a new Organism
         * @param _random Pointer to random number generator
         * @param _points Initial energy points
         * @param _species Species identifier
         */
        Organism(emp::Ptr<emp::Random> _random, double _points=0.0, int _species=0) :
            points(_points), random(_random), species(_species) {}

        virtual ~Organism() = default;

        // Getters and setters
        /**
         * @brief Set organism's energy points
         * @param _in New energy value
         */
        void SetPoints(double _in) { points = _in; }
        
        /**
         * @brief Add points to organism's energy
         * @param _in Points to add (can be negative)
         */
        void AddPoints(double _in) { points += _in; }
        
        /**
         * @brief Get current energy points
         * @return Current energy value
         */
        double GetPoints() const { return points; }
        
        /**
         * @brief Get species identifier
         * @return Species ID (0=mouse, 1=owl)
         */
        int GetSpecies() const { return species; }

        /**
         * @brief Process organism behavior within the world context
         * @param world Reference to the world
         * @param pos Current position in the world
         */
        virtual void ProcessInWorld(OrgWorld& world, size_t pos) = 0;

        /**
         * @brief Check if organism should reproduce
         * @return True if organism has enough energy to reproduce
         */
        virtual bool ShouldReproduce() const = 0;

        /**
         * @brief Create offspring of this organism
         * @return Pointer to new organism offspring
         */
        virtual emp::Ptr<Organism> CreateOffspring() = 0;

        /**
         * @brief Get the energy cost of reproduction
         * @return Energy points deducted from parent during reproduction
         */
        virtual double GetReproductionCost() const = 0;

        /**
         * @brief Apply metabolism effects each turn
         */
        virtual void ApplyMetabolism() = 0;
};

#endif