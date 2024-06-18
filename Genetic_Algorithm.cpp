#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <limits>
#include <algorithm>

struct City{
    int x;
    int y;
    int order;
};

void generate_coordinates_ordered(City *cities, int city_size, int max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, max - 1);

    for (int i = 0; i < city_size; ++i)
    {
        cities[i].x = dist(gen);
        cities[i].y = dist(gen);
        cities[i].order = i;
    }
}

void generate_bits(float **pop, int pop_size){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    for (int i = 0; i < pop_size; ++i)
    {
        pop[i] = new float[6];
        for (int j = 0; j < 5; ++j)
        {
            pop[i][j] = static_cast<float>(dist(gen));
        }
        pop[i][5] = pop[i][0] * 16.0f + pop[i][1] * 8.0f + pop[i][2] * 4.0f + pop[i][3] * 2.0f + pop[i][4];
    }
}

void calculate_pselect_expected_actual(float **pop, float *fitness, float *pselect, float *expected_count, float *actual_count, int pop_size){
    // Calcular fitness "Revisar esto"
    for (int i = 0; i < pop_size; ++i)
    {
        fitness[i] = pop[i][5] * pop[i][5]; // f(x) = x^2
    }
    // Calcular la suma total de fitness
    float sum_fitness = 0.0;
    for (int i = 0; i < pop_size; ++i)
    {
        sum_fitness += fitness[i];
    }

    // Calcular pselect y expected count
    for (int i = 0; i < pop_size; ++i)
    {
        pselect[i] = fitness[i] / sum_fitness;
        expected_count[i] = pselect[i] * pop_size;
    }

    // Simular la selección por ruleta para calcular actual count
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0, 1.0);

    for (int i = 0; i < pop_size; ++i)
    {
        actual_count[i] = 0;
    }

    for (int i = 0; i < pop_size; ++i)
    {
        float rand_num = dist(gen);
        float cumulative_prob = 0.0;
        for (int j = 0; j < pop_size; ++j)
        {
            cumulative_prob += pselect[j];
            if (rand_num <= cumulative_prob)
            {
                actual_count[j]++;
                break;
            }
        }
    }
}

void crossover(float **pop, int pop_size, int crossover_rate){
    // Punto de cruce en el rango [0, 5] para un arreglo de tamaño 6 [Esta Mal]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 5); 


    for (int i = 0; i < pop_size - 1; i += 2)
    {
        if (dist(gen) < crossover_rate)
        {
            int crossover_point = dist(gen);
            for (int j = crossover_point; j < 6; ++j)
            {
                std::swap(pop[i][j], pop[i + 1][j]);
            }
        }
    }
}

void mutation(float** pop, int pop_size, int mutation_rate)
{
    // Posición de mutación en el rango [0, 5]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 5); 

    for (int i = 0; i < pop_size; ++i)
    {
        if (dist(gen) < mutation_rate)
        {
            int mutation_point = dist(gen);
            // Mutación complementaria (0 a 1 o 1 a 0)
            pop[i][mutation_point] = 1 - pop[i][mutation_point]; 
        }
    }
}

double between_cities(const City &city1, const City &city2)
{
    int dx = city2.x - city1.x;
    int dy = city2.y - city1.y;
    return std::sqrt(dx * dx + dy * dy);
}

void calDistance(City *cities, int city_size, double **distances)
{
    for (int i = 0; i < city_size; ++i)
    {
        distances[i] = new double[city_size];
        for (int j = 0; j < city_size; ++j)
        {
            if (i == j)
            {
                distances[i][j] = 0.0;
            }
            else
            {
                double dist = between_cities(cities[i], cities[j]);
                distances[i][j] = dist;
                distances[j][i] = dist;
            }
        }
    }
}

int main()
{
    int city_size = 5;
    // 600 * 600 
    int max = 600; 
    City *cities = new City[city_size];
    generate_coordinates_ordered(cities, city_size, max);

    int pop_size = 10;
    float **pop = new float *[pop_size];
    generate_bits(pop, pop_size);

    double **distances = new double *[city_size];
    for (int i = 0; i < city_size; ++i)
    {
        distances[i] = new double[city_size];
    }
    calDistance(cities, city_size, distances);

    // Array para fitness, pselect, expected count y actual count
    float **array_float = new float *[4];
    for (int i = 0; i < 4; ++i)
    {
        array_float[i] = new float[pop_size];
    }
    float *fitness = new float[pop_size];
    float *pselect = new float[pop_size];
    float *expected_count = new float[pop_size];
    float *actual_count = new float[pop_size];

    int max_iterations = 200;
    int convergence_threshold = 5; // Converge en 5 generaciones

    int iteration = 0;
    float best_fitness = std::numeric_limits<float>::max(); // Inicializar con un valor muy grande
    int same_fitness_count = 0;

    while (iteration < max_iterations && same_fitness_count < convergence_threshold)
    {
        calculate_pselect_expected_actual(pop, fitness, pselect, expected_count, actual_count, pop_size);

        // Convergencia por fitness
        float current_best_fitness = *std::max_element(fitness, fitness + pop_size);
        if (current_best_fitness == best_fitness)
        {
            same_fitness_count++;
        }
        else
        {
            best_fitness = current_best_fitness;
            same_fitness_count = 0;
        }

        // Aplicación del crossover y mutación
        int crossover_rate = 80; // 80% de probabilidad
        int mutation_rate = 5;   // 5% de probabilidad

        crossover(pop, pop_size, crossover_rate);
        mutation(pop, pop_size, mutation_rate);

        iteration++;
    }

    std::cout << "Mejor fitness encontrado: " << best_fitness << std::endl;
    std::cout << "Última generación evaluada: " << iteration << std::endl;
    return 0;
}