#include "emp/math/Random.hpp"
#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "World.h"
#include "Org.h"
#include "Mouse.h"
#include "Owl.h"

emp::web::Document doc{"target"};

/**
 * @brief Main animation class for the Artificial Ecology simulation
 * 
 * Manages the visualization and control of the predator-prey ecosystem,
 * providing an interactive web-based interface for observing population
 * dynamics between mice (prey) and owls (predators).
 */
class AEAnimator : public emp::web::Animate {
    private:
        // Seed to control random number generation
        static constexpr int SEED = 6;    ///< Seed for random number generator

        // Arena dimensions and visual parameters
        static constexpr int NUM_H_BOXES = 20;    ///< Grid height in cells
        static constexpr int NUM_W_BOXES = 20;    ///< Grid width in cells
        static constexpr double RECT_SIDE = 20;   ///< Size of each cell in pixels
        static constexpr double WIDTH = NUM_W_BOXES * RECT_SIDE;  ///< Canvas width
        static constexpr double HEIGHT = NUM_H_BOXES * RECT_SIDE; ///< Canvas height

        // Population density parameters
        static constexpr int MOUSE_DENSITY_RATIO = 4;   ///< Fraction of cells with mice (1/4)
        static constexpr int OWL_DENSITY_RATIO = 40;    ///< Fraction of cells with owls (1/40)

        // Initial energy values
        static constexpr double INITIAL_MOUSE_ENERGY = 600.0;
        static constexpr double INITIAL_OWL_ENERGY = 500.0;

        emp::web::Canvas canvas;           ///< Canvas for rendering the simulation
        emp::Random random_generator;      ///< Random number generator
        OrgWorld world;                    ///< The ecosystem world

        // Visualization colors
        const std::string grass_color = "green";
        const std::string mouse_color = "gray";
        const std::string owl_color = "brown";

    public:
        /**
         * @brief Construct the animator and set up the simulation
         */
        AEAnimator() : canvas(WIDTH, HEIGHT, "canvas"), 
                      random_generator(SEED), 
                      world(random_generator) {
            SetupInterface();
            InitializeWorld();
        }

        /**
         * @brief Process one frame of the simulation
         * 
         * Updates the visualization and advances the ecosystem by one step.
         * Note: UpdateEcology logic has been moved to World.h for better organization.
         */
        void DoFrame() override {
            DrawWorld();
            world.UpdateEcology();
        }

    private:
        /**
         * @brief Set up the web interface with instructions and controls
         */
        void SetupInterface() {
            doc << "<h2>Artificial Ecology Simulation</h2>";
            doc << "<p>This simulation demonstrates a simple predator-prey ecosystem:</p>";
            doc << "<ul>";
            doc << "<li><span style='color: green;'>■</span> <b>Green squares</b>: Grass - provides food for mice</li>";
            doc << "<li><span style='color: gray;'>■</span> <b>Gray squares</b>: Mice - eat grass and reproduce, prey for owls</li>";
            doc << "<li><span style='color: brown;'>■</span> <b>Brown squares</b>: Owls - hunt mice to survive and reproduce</li>";
            doc << "</ul>";
            doc << "<p>Watch how populations cycle as predators and prey interact!</p>";
            
            doc << canvas;
            doc << "<br>";
            doc << GetToggleButton("Toggle");
            doc << " ";
            doc << GetStepButton("Step");
        }

        /**
         * @brief Initialize the world with organisms in a grid structure
         */
        void InitializeWorld() {
            world.SetPopStruct_Grid(NUM_W_BOXES, NUM_H_BOXES);
            PopulateWithMice();
            PopulateWithOwls();
        }

        /**
         * @brief Add mice to random positions in the world
         */
        void PopulateWithMice() {
            const int target_mice = NUM_W_BOXES * NUM_H_BOXES / MOUSE_DENSITY_RATIO;
            for (int i = 0; i < target_mice; i++) {
                size_t mouse_pos = random_generator.GetUInt(NUM_W_BOXES * NUM_H_BOXES);
                if (!world.IsOccupied(mouse_pos)) {
                    emp::Ptr<Mouse> new_mouse = new Mouse(&random_generator, INITIAL_MOUSE_ENERGY, 0);
                    world.AddOrgAt(new_mouse, mouse_pos);
                }
            }
        }

        /**
         * @brief Add owls to random positions in the world
         */
        void PopulateWithOwls() {
            const int target_owls = NUM_W_BOXES * NUM_H_BOXES / OWL_DENSITY_RATIO;
            for (int i = 0; i < target_owls; i++) {
                size_t owl_pos = random_generator.GetUInt(NUM_W_BOXES * NUM_H_BOXES);
                if (!world.IsOccupied(owl_pos)) {
                    emp::Ptr<Owl> new_owl = new Owl(&random_generator, INITIAL_OWL_ENERGY, 1);
                    world.AddOrgAt(new_owl, owl_pos);
                }
            }
        }

        /**
         * @brief Render the current state of the world
         */
        void DrawWorld() {
            canvas.Clear();
            
            for (int x = 0; x < NUM_W_BOXES; x++) {
                for (int y = 0; y < NUM_H_BOXES; y++) {
                    size_t pos = y * NUM_W_BOXES + x;
                    std::string color = GetCellColor(pos);
                    DrawCell(x, y, color);
                }
            }
        }

        /**
         * @brief Draw a single cell on the canvas
         * @param x X coordinate in grid
         * @param y Y coordinate in grid
         * @param color Fill color for the cell
         */
        void DrawCell(int x, int y, const std::string& color) {
            canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, color, "black");
        }

        /**
         * @brief Get the color for a cell based on its contents
         * @param pos Position in the world grid
         * @return Color string for the cell
         */
        std::string GetCellColor(size_t pos) {
            if (!world.IsOccupied(pos)) {
                return grass_color;
            }
            
            int species = world.GetOrg(pos).GetSpecies();
            return (species == 0) ? mouse_color : owl_color;
        }
};

// Global animator instance
AEAnimator animator;

/**
 * @brief Main entry point for the simulation
 * @return Exit status
 */
int main() {
    // Start with one step
    animator.Step();
    return 0;
}