[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/-GCLf3vW)

[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/-GCLf3vW)

# Artificial Ecology Simulation

A web-based predator-prey ecosystem simulation built using the Empirical library, featuring polymorphic organism behavior and dynamic population cycles.

## Overview

This project simulates a basic ecosystem with three components:
- **Grass**: The base of the food chain (shown as green squares)
- **Mice**: Primary consumers that eat grass (shown as gray squares)  
- **Owls**: Predators that hunt mice (shown as brown squares)

## How It Works

The simulation operates on the following principles:

1. **Mice (Species 0)**:
   - Gain energy from nearby grass cells (75 points per grass cell)
   - Lose 50 energy points per turn due to metabolism
   - Reproduce when reaching 800+ energy points
   - Offspring start with 300 energy, parent loses 700 energy
   - Die when energy reaches 0 or below

2. **Owls (Species 1)**:
   - Hunt mice to gain energy (20% of hunted mouse's energy)
   - Lose 100 energy points per turn when not hunting successfully
   - Lose 50 energy points per turn even when hunting successfully  
   - Reproduce when reaching 2500+ energy points
   - Offspring start with 400 energy, parent loses 2000 energy
   - Die when energy reaches 0 or below

3. **Ecosystem Dynamics**:
   - Organisms move randomly with 20% probability each turn
   - Toroidal world wrapping (edges connect to opposite sides)
   - Self-regulating population cycles through predator-prey interactions
   - Natural balance between birth rates, death rates, and resource availability

## Technical Features

- **Object-Oriented Design**: Polymorphic organism behavior using inheritance
- **Grid-Based World**: 20x20 toroidal grid for spatial interactions
- **Real-Time Visualization**: Interactive web interface with start/stop controls
- **Comprehensive Documentation**: Full API documentation with Doxygen-style comments

## Architecture

- `Org.h`: Abstract base class defining organism interface
- `Mouse.h`: Herbivore implementation with grass-eating behavior
- `Owl.h`: Carnivore implementation with mouse-hunting behavior  
- `World.h`: Ecosystem management and organism interactions
- `AEAnimate.cpp`: Visualization and user interface

## Running the Simulation

1. Compile the project using the Empirical library
2. Open the generated web page in a browser
3. Use the **Toggle** button to start/stop the simulation
4. Use the **Step** button to advance one frame at a time
5. Observe population dynamics and predator-prey cycles

## Population Parameters

- **Initial Mice**: ~100 (1/4 of grid cells)
- **Initial Owls**: ~10 (1/40 of grid cells)  
- **Movement**: 20% chance per organism per turn