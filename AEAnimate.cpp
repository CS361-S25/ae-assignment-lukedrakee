#include "emp/math/Random.hpp"
#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "World.h"
#include "Org.h"
#include "Mouse.h"
#include "Owl.h"

emp::web::Document doc{"target"};

class AEAnimator : public emp::web::Animate {

    // arena width and height
    const int num_h_boxes = 20;
    const int num_w_boxes = 20;
    const double RECT_SIDE = 20;
    const double width{num_w_boxes * RECT_SIDE};
    const double height{num_h_boxes * RECT_SIDE};

    emp::web::Canvas canvas{width, height, "canvas"};
    emp::Random random_generator{5}; // Random seed
    OrgWorld world{random_generator};
    
    // Colors for visualization
    std::string grass_color = "green";
    std::string mouse_color = "gray";
    std::string owl_color = "brown";

public:

    AEAnimator() {
        // shove canvas into the div
        // along with control buttons and basic explanation
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
        
        // Set up the world
        InitializeWorld();
    }
    
    void InitializeWorld() {
        // Configure the world to have a structured grid
        world.SetPopStruct_Grid(num_w_boxes, num_h_boxes);
        
        // Add mice (more than owls)
        for (int i = 0; i < num_w_boxes * num_h_boxes / 4; i++) {  // Many more mice
            size_t mouse_pos = random_generator.GetUInt(num_w_boxes * num_h_boxes);
            if (!world.IsOccupied(mouse_pos)) {
                emp::Ptr<Mouse> new_mouse = new Mouse(&random_generator, 600.0, 0);
                world.AddOrgAt(new_mouse, mouse_pos);
            }
        }
        
        // Add owls (fewer than mice)
        for (int i = 0; i < num_w_boxes * num_h_boxes / 40; i++) {
            size_t owl_pos = random_generator.GetUInt(num_w_boxes * num_h_boxes);
            if (!world.IsOccupied(owl_pos)) {
                emp::Ptr<Owl> new_owl = new Owl(&random_generator, 500.0, 1);
                world.AddOrgAt(new_owl, owl_pos);
            }
        }
    }

    void DrawWorld() {
        canvas.Clear();
        
        // Draw all cells
        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
                size_t pos = y * num_w_boxes + x;
                
                if (world.IsOccupied(pos)) {
                    int species = world.GetOrg(pos).GetSpecies();
                    if (species == 0) {
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, mouse_color, "black");
                    } else { // species == 1, so owl
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, owl_color, "black");
                    }
                } else {
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, grass_color, "black");
                }
            }
        }
    }

    void UpdateEcology() {
        // Handle owl predation and mouse grass eating
        emp::vector<size_t> action_schedule = emp::GetPermutation(random_generator, world.GetSize());
        for (size_t i : action_schedule) {
            if (!world.IsOccupied(i)) continue;
            
            // If it's an owl, let it hunt mice
            if (world.GetOrg(i).GetSpecies() == 1) {
                emp::Ptr<Owl> owl = dynamic_cast<Owl*>(&world.GetOrg(i));
                if (owl) owl->Hunt(world, i);
                
                // Remove owl if it starves
                if (world.IsOccupied(i) && world.GetOrg(i).GetPoints() <= 0) {
                    world.RemoveOrganism(i);
                }
            }
            
            // If it's a mouse, check for grass nearby
            if (world.IsOccupied(i) && world.GetOrg(i).GetSpecies() == 0) {
                emp::Ptr<Mouse> mouse = dynamic_cast<Mouse*>(&world.GetOrg(i));
                if (mouse) mouse->EatGrass(world, i);
                
                // Remove mouse if it starves
                if (world.IsOccupied(i) && world.GetOrg(i).GetPoints() <= 0) {
                    world.RemoveOrganism(i);
                }
            }
        }
        
        // Move some organisms randomly
        for (size_t i = 0; i < world.GetSize(); i++) {
            if (world.IsOccupied(i) && random_generator.P(0.2)) {
                world.MoveOrganism(i);
            }
        }
    }

    void DoFrame() override {
        DrawWorld();
        UpdateEcology();
    }
};

AEAnimator animator;

int main() {
    // Start with one step
    animator.Step();
}