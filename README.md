[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/-GCLf3vW)

# Artificial Ecology Simulation

A simple web-based predator-prey simulation built using the Empirical library.

## Overview

This project simulates a basic ecosystem with three components:
- **Grass**: The base of the food chain (shown as green squares)
- **Mice**: Primary consumers that eat grass (shown as gray squares)
- **Owls**: Predators that hunt mice (shown as brown squares)

## How It Works

The simulation runs on the following principles:

1. **Mice**:
   - Gain energy from nearby grass
   - Reproduce when they accumulate enough energy
   - Lose energy over time and die if they run out (overpopulation)

2. **Owls**:
   - Hunt mice to gain energy
   - Reproduce when they accumulate enough energy
   - Lose energy over time and die if they run out (no mice/overpopulation)

3. **Ecosystem Balance**:
   - The system self-regulates through predator-prey dynamics
   - Population cycles emerge naturally as resources fluctuate
   - The simulation includes mechanisms to prevent extinction
