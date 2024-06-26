#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib> // Para rand() y srand()
#include <limits>
#include <algorithm> // Para std::swap
//city
struct City {
    int x;
    int y;
    int order;
};

void generate_coordinates_ordered(City* cities, int city_size, int max) {
    for (int i = 0; i < city_size; ++i) {
        cities[i].x = rand() % max;
        cities[i].y = rand() % max;
        cities[i].order = i;
    }
}

void generate_bits(bool** pop, int pop_size, int bit_length) {
    for (int i = 0; i < pop_size; ++i) {
        for (int j = 0; j < bit_length; ++j) {
            pop[i][j] = rand() % 2;
        }
    }
}

double between_cities(const City &city1, const City &city2) {
    int dx = city2.x - city1.x;
    int dy = city2.y - city1.y;
    return std::sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(bool* bits, City* cities, int city_size) {
    double total_distance = 0.0;
    for (int i = 0; i < city_size - 1; ++i) {
        int city_idx1 = bits[i];
        int city_idx2 = bits[i + 1];
        total_distance += between_cities(cities[city_idx1], cities[city_idx2]);
    }
    // Agregar la distancia de regreso al primer punto
    int last_city_idx = bits[city_size - 1];
    total_distance += between_cities(cities[last_city_idx], cities[bits[0]]);
    return total_distance;
}

void calculate_fitness(bool** pop, float* fitness, double* distances, City* cities, int pop_size, int city_size) {
    for (int i = 0; i < pop_size; ++i) {
        double total_distance = calculate_total_distance(pop[i], cities, city_size);
        double fitness_value = total_distance * total_distance; // Valor cuadrático de la distancia
        fitness[i] = static_cast<float>(fitness_value);
        distances[i] = total_distance;
    }
}

void calculate_pselect(float* fitness, float* pselect, int pop_size) {
    // Calcular suma total de fitness
    float sum_fitness = 0.0f;
    for (int i = 0; i < pop_size; ++i) {
        sum_fitness += fitness[i];
    }

    // Calcular pselect
    float sum_pselect = 0.0f;
    for (int i = 0; i < pop_size; ++i) {
        pselect[i] = fitness[i] / sum_fitness;
        sum_pselect += pselect[i];
    }
}

void crossover(bool** pop, int pop_size, int bit_length) {
    for (int i = 0; i < pop_size - 1; i += 2) {
        int crossover_point = bit_length / 2; // Punto de cruce siempre en la mitad
        for (int j = crossover_point; j < bit_length; ++j) {
            std::swap(pop[i][j], pop[i + 1][j]);
        }
    }
}

void mutation(bool** pop, int pop_size, int bit_length, int mutation_rate) {
    for (int i = 0; i < pop_size; ++i) {
        for (int j = 0; j < bit_length; ++j) {
            if (rand() % 100 < mutation_rate) { // Suponiendo mutation_rate en porcentaje (0-100)
                pop[i][j] != pop[i][j]; // Mutación complementaria (0 a 1 o 1 a 0)
            }
        }
    }
}

void print_best_individual(bool** pop, float* fitness, double* distances, int pop_size, int city_size) {
    int best_idx = 0;
    float best_fitness = fitness[0];
    for (int i = 1; i < pop_size; ++i) {
        if (fitness[i] < best_fitness) {
            best_fitness = fitness[i];
            best_idx = i;
        }
    }
    std::cout << "Mejor fitness encontrado: " << best_fitness << std::endl;
    std::cout << "Distancia total del recorrido: " << distances[best_idx] << std::endl;
    std::cout << "Ruta: ";
    for (int i = 0; i < city_size; ++i) {
        std::cout << pop[best_idx][i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr))); // Inicializar la semilla para rand() con el tiempo actual

    int city_size = 5;
    int max = 600; // 600 * 600
    City* cities = new City[city_size];
    generate_coordinates_ordered(cities, city_size, max);

    int pop_size = 10;
    int bit_length = city_size; // Cada individuo en la población tiene 'city_size' bits
    bool** pop = new bool*[pop_size];
    for (int i = 0; i < pop_size; ++i) {
        pop[i] = new bool[bit_length];
    }
    generate_bits(pop, pop_size, bit_length);

    double* distances = new double[pop_size];
    float* fitness = new float[pop_size];
    float* pselect = new float[pop_size];

    int max_iterations = 400;
    int convergence_threshold = 5;

    int iteration = 0;
    float best_fitness = std::numeric_limits<float>::max(); 
    int same_fitness_count = 0;

    while (iteration < max_iterations && same_fitness_count < convergence_threshold) {
        calculate_fitness(pop, fitness, distances, cities, pop_size, city_size);
        calculate_pselect(fitness, pselect, pop_size);

        float current_best_fitness = fitness[0]; // Inicializar con el primer valor
        for (int i = 1; i < pop_size; ++i) {
            if (fitness[i] < current_best_fitness) {
                current_best_fitness = fitness[i];
            }
        }
        if (current_best_fitness >= best_fitness) {
            same_fitness_count++;
        } else {
            best_fitness = current_best_fitness;
            same_fitness_count = 0;
        }

        crossover(pop, pop_size, bit_length);
        mutation(pop, pop_size, bit_length, 0.1);

        iteration++;
    }

    print_best_individual(pop, fitness, distances, pop_size, city_size);

    // Liberar memoria
    for (int i = 0; i < pop_size; ++i) {
        delete[] pop[i];
    }
    delete[] pop;
    delete[] distances;
    delete[] fitness;
    delete[] pselect;
    delete[] cities;

    return 0;
}
