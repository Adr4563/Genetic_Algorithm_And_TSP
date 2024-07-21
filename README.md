# Genetic Algorithm & TSP

### **Contents** 📚
  - [Introduction 📖](#introduction-)
  - [Theoretical Framework 📘](#theoretical-framework-)
  - [Deploy in Scratch 🛠️](#deploy-in-scratch-)

# Introduction 📖

The **Traveling Salesman Problem (TSP)** is a classic problem in combinatorial optimization where the goal is to find the shortest possible route that visits a set of cities and returns to the origin city. The Genetic Algorithm is an optimization technique based on principles of natural selection and genetics to find approximate solutions to complex problems.

![image](https://github.com/user-attachments/assets/59d3f847-2cf4-4e72-a46f-1c9eeb549824)

In this document, we explore how a Genetic Algorithm can be used to solve the TSP. The implementation is done in C++, taking advantage of the language's efficiency and control for handling large datasets and performing complex calculations.

# Theoretical Framework 📘

The main steps in implementing the Genetic Algorithm for TSP include:

**Solution Route Encoding 🗺️:** 
Representing a solution to the TSP as a sequence of cities.

**Fitness Function 🏋️‍♂️:**
Evaluating the quality of a solution based on the total length of the tour.

**Crossover 🔄:**
Combining parts of two parent solutions to create new offspring solutions.

**Mutation 🧬:** 
Introducing random changes to a solution to maintain genetic diversity.

<details>
  <summary>Click👆</summary>
  <img src="https://github.com/user-attachments/assets/0f2161ab-747c-4f87-8eaa-b61f4e83f02e" alt="image">
</details>

### Process:

1. **Cities 🌆:** Initializes predetermined cities on a plane (x, y).
2. **Initialize Population 👥:** Initializes the population with random individuals.
3. **Calculate Route Distance 📏:** Calculates the distance of the route for each individual.
4. **Evaluate Fitness 🏋️‍♂️:** Evaluates the fitness of each individual in the population.
5. **Select Parents 👨‍👩‍👧‍👦:** Selects parents for reproduction based on fitness.
6. **Crossover 🔄:** Performs crossover between two parents to generate new individuals.
7. **Mutate 🧬:** Applies mutations to the new individuals.
8. **Update Population 🔄:** Updates the population with the new individuals.
9. **Generations 🔁:** Refers to new segments of the process that are repeated with the goal of achieving convergence. Each generation involves evaluating, selecting, and evolving the population to improve the solution over successive iterations until a satisfactory result is found or a convergence criterion is met.


# Deploy in Scratch 🛠️

Scratch is a visual programming language that allows users to create code blocks and simulate algorithms in an educational environment. Although it is not suitable for high-performance applications, it provides an accessible way to understand basic concepts of Genetic Algorithms.

![image](https://github.com/user-attachments/assets/dd79b350-0329-48e2-9f03-35e89a5ec022)


**-> Design the Problem:** Create a Scratch environment where the TSP problem and possible solutions can be represented visually.
**-> Implement the Algorithm:** Program genetic operators such as crossover and mutation using Scratch's code blocks.
**-> Visualize Results:** Display how solutions evolve and how the tour quality improves over generations.



### Genetic Algorithm Implementation in Scratch

To visualize the GA process, you can use Scratch's visual blocks to simulate the algorithm's evolution. The attached image illustrates a basic setup for implementing GA in Scratch, including the different stages and blocks used.

![image](https://github.com/user-attachments/assets/a3340536-4f28-44db-813c-fdad9b76b730)

For a hands-on example, you can explore the Scratch project linked below, which demonstrates a Genetic Algorithm implementation:

### [Explore Scratch Project](https://scratch.mit.edu/projects/1041854828/)
