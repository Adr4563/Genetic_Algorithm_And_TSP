#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm> // Para std::swap

struct City {
    int x;
    int y;
    int order;
};

void generate_routes(int** pop, int pop_size, int city_size) {
    // Generar una ruta aleatoria
    for (int i = 0; i < pop_size; ++i) {
        bool* city_used = new bool[city_size];
        for (int j = 0; j < city_size; ++j) {
            city_used[j] = false;
        }
        
        pop[i] = new int[city_size];
        
        for (int j = 0; j < city_size; ++j) {
            int index;
            do {
                index = rand() % city_size;
            } while (city_used[index]);
            
            city_used[index] = true;
            pop[i][j] = index;
        }
        
        delete[] city_used;
    }
}

void generate_coordinates(City* cities, int city_size, int max) {
    for (int i = 0; i < city_size; ++i) {
        cities[i].x = rand() % max;
        cities[i].y = rand() % max;
        cities[i].order = i;
    }
}

double between_cities(const City &city1, const City &city2) {
    int dx = city2.x - city1.x;
    int dy = city2.y - city1.y;
    return std::sqrt(dx * dx + dy * dy);
}

double calculate_total_distance(int* route, City* cities, int city_size) {
    double total_distance = 0.0;
    for (int i = 0; i < city_size - 1; ++i) {
        int city_idx1 = route[i];
        int city_idx2 = route[i + 1];
        total_distance += between_cities(cities[city_idx1], cities[city_idx2]);
    }
    int last_city_idx = route[city_size - 1];
    total_distance += between_cities(cities[last_city_idx], cities[route[0]]);
    return total_distance;
}

void calculate_fitness_distances(int** pop, float* fitness, double* distances, City* cities, int pop_size, int city_size) {
    double total_distance = 0.0;

    for (int i = 0; i < pop_size; ++i) {
        double route_distance = calculate_total_distance(pop[i], cities, city_size);
        distances[i] = route_distance;
        total_distance += route_distance;
    }

    for (int i = 0; i < pop_size; ++i) {
        fitness[i] = distances[i]*distances[i];
    }
}

void calculate_pselect(float* pselect, float* fitness, int pop_size) {
    float sum_fitness = 0.0f;
    for (int i = 0; i < pop_size; ++i) {
        sum_fitness += fitness[i];
    }

    for (int i = 0; i < pop_size; ++i) {
        pselect[i] = fitness[i] / sum_fitness;
    }
}


void select_best_indices(float* fitness, int pop_size, int* best_indices, int num_best) {
    // Encontrar los dos mejores fitness mediante los mejores fitness
    int best_index1 = 0;
    int best_index2 = 1;

    for (int i = 2; i < pop_size; ++i) {
        if (fitness[i] > fitness[best_index1]) {
            best_index2 = best_index1;
            best_index1 = i;
        } else if (fitness[i] > fitness[best_index2]) {
            best_index2 = i;
        }
    }

    // Se guardan los mejores indices con respecto a los mejores fitness
    best_indices[0] = best_index1;
    best_indices[1] = best_index2;
}

void crossover_half(int* father_1, int* father_2, int* son_1, int* son_2, int city_size) {
    // Partir a la mitada y generar el hijo 1 y hijo 2
    int half_size = round(city_size / 2);
    for (int i = 0; i < half_size; ++i) {
        son_1[i] = father_1[i];
        son_2[i] = father_2[i];
    }

    // Llenar la segunda mitad de cada hijo del padre
    int index1 = half_size;
    int index2 = half_size;
    
    // Para el hijo 1
    for (int i = 0; i < city_size; ++i) {
        bool found = false;
        for (int j = 0; j < half_size; ++j) {
            if (father_2[i] == son_1[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            son_1[index1++] = father_2[i];
        }
    }

    // Para el hijo 2
    for (int i = 0; i < city_size; ++i) {
        bool found = false;
        for (int j = 0; j < half_size; ++j) {
            if (father_1[i] == son_2[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            son_2[index2++] = father_1[i];
        }
    }
}

void mutate(int** pop, int pop_size, int city_size, double mutation_rate) {
    for (int i = 0; i < pop_size; ++i) {
    
        if ((rand() % 100 + 1) /100 < mutation_rate) {
            // Seleccionar dos posiciones aleatorias para intercambiar ciudades
            int city_1 = rand() % city_size;
            int city_2 = rand() % city_size;

            // Intercambiar las ciudades: city_1 y city_2
            std::swap(pop[i][city_1], pop[i][city_2]);
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    int city_size = 10;
    int max = 600;

    City* cities = new City[city_size];
    generate_coordinates(cities, city_size, max);

    int pop_size = 8;
    int** pop = new int*[pop_size];
    for (int i = 0; i < pop_size; ++i) {
        pop[i] = new int[city_size];
    }
    generate_routes(pop, pop_size, city_size);

    float* fitness = new float[pop_size];
    double* distances = new double[pop_size];
    calculate_fitness_distances(pop, fitness, distances, cities, pop_size, city_size);

    float* pselect = new float[pop_size];
    calculate_pselect(pselect, fitness, pop_size);

    // La mejor distancia encontrada
    int best_index = 0;
    double best_distance = distances[0];

    // Lista para almacenar la mejor ruta encontrada
    int* best_route = new int[city_size];
    for (int i = 0; i < city_size; ++i) {
        best_route[i] = pop[best_index][i];
    }

    int iterations = 300;
    int iter = 0;
    while (iter < iterations) {
        // Seleccionar los mejores individuos [121, 150, 160, 1245]
        int num_best = 2;
        int* best_indices = new int[num_best];
        select_best_indices(fitness, pop_size, best_indices, num_best);

        // Realizar crossover
        int* son_1 = new int[city_size];
        int* son_2 = new int[city_size];
        crossover_half(pop[best_indices[0]], pop[best_indices[1]], son_1, son_2, city_size);

        // Mutación
        double mutation_rate = 0.05; // Tasa de mutación del 5%
        mutate(pop, pop_size, city_size, mutation_rate);

        calculate_fitness_distances(pop, fitness, distances, cities, pop_size, city_size);
        for (int i = 0; i < pop_size; ++i) {
            // Encontrar la distancia óptima de todos los individuos
            if (distances[i] < best_distance) {
                best_distance = distances[i];
                best_index = i;
                // Si se ha encontrado la distancia óptima, se ha encontrado la ruta
                for (int j = 0; j < city_size; ++j) {
                    best_route[j] = pop[i][j];
                }
            }
        }
        iter++;
    }

    std::cout << "Distancia más óptima: " << best_distance << std::endl;

    std::cout << "Nueva ruta: ";
    for (int i = 0; i < city_size; ++i) {
        std::cout << best_route[i] << " ";
    }
    std::cout << std::endl;
    delete[] fitness;
    delete[] distances;
    delete[] pselect;
    delete[] cities;
    delete[] best_route;
    for (int i = 0; i < pop_size; ++i) {
        delete[] pop[i];
    }
    delete[] pop;

    return 0;
}
