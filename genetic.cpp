#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

// Function to generate a random population of individuals
vector<vector<int>> initializePopulation(int populationSize, int subsetSize)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);

    vector<vector<int>> population(populationSize, vector<int>(subsetSize));

    for (int i = 0; i < populationSize; ++i)
    {
        for (int j = 0; j < subsetSize; ++j)
        {
            population[i][j] = dis(gen);
        }
    }

    return population;
}

// Function to evaluate the fitness of an individual (subset) based on its sum
int evaluateFitness(const vector<int>& individual, const vector<int>& set, int targetSum)
{
    int currentSum = 0;
    for (int i = 0; i < individual.size(); ++i)
    {
        if (individual[i])
            currentSum += set[i];
    }
    return abs(targetSum - currentSum);
}

// Function to perform single-point crossover between two parents
vector<int> performSinglePointCrossover(const vector<int>& parent1, const vector<int>& parent2)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, min(parent1.size(), parent2.size()) - 1);

    int crossoverPoint = dis(gen);

    vector<int> child(parent1.begin(), parent1.begin() + crossoverPoint);
    child.insert(child.end(), parent2.begin() + crossoverPoint, parent2.end());

    return child;
}

vector<int> performTwoPointsRandomCrossover(const vector<int>& parent1, const vector<int>& parent2)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, min(parent1.size(), parent2.size()) - 2);

    int crossoverPoint1 = dis(gen);
    int crossoverPoint2 = dis(gen);

    if (crossoverPoint1 > crossoverPoint2)
        swap(crossoverPoint1, crossoverPoint2);

    vector<int> child(parent1.begin(), parent1.begin() + crossoverPoint1);
    child.insert(child.end(), parent2.begin() + crossoverPoint1, parent2.begin() + crossoverPoint2);
    child.insert(child.end(), parent1.begin() + crossoverPoint2, parent1.end());

    return child;
}

// Function to perform mutation on an individual (subset)
void performMutation(vector<int>& individual)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, individual.size() - 1);
    int mutationPoint = dis(gen);

    if (mutationPoint >= 0 && mutationPoint < individual.size())
        individual[mutationPoint] = !individual[mutationPoint];
}

// Function to perform flipTwoRandomPointsMutation on an individual (subset)
void performFlipTwoRandomPointsMutation(vector<int>& individual)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, individual.size() - 1);
    int mutationPoint1 = dis(gen);
    int mutationPoint2 = dis(gen);

    if (mutationPoint1 >= 0 && mutationPoint1 < individual.size() &&
        mutationPoint2 >= 0 && mutationPoint2 < individual.size())
    {
        // Flip the values at the mutation points
        individual[mutationPoint1] = !individual[mutationPoint1];
        individual[mutationPoint2] = !individual[mutationPoint2];
    }
}

// Function to check if the termination condition of finding the subset is met
bool isSubsetFoundTermination(const vector<int>& populationFitness)
{
    // Check if any individual in the population has a fitness of 0 (subset with target sum found)
    return find(populationFitness.begin(), populationFitness.end(), 0) != populationFitness.end();
}

// Function to check if the termination condition of reaching the maximum number of generations is met
bool isMaxGenerationsTermination(int currentGeneration, int maxGenerations)
{
    // Check if the maximum number of generations has been reached
    return currentGeneration >= maxGenerations;
}

// Function to select parents for crossover using tournament selection
pair<int, int> selectParents(const vector<int>& populationFitness)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, populationFitness.size() - 1);

    int parent1 = dis(gen);
    int parent2 = dis(gen);

    return make_pair(parent1, parent2);
}

// Function to perform the genetic algorithm for subset sum problem
vector<int> solveSubsetSum(const vector<int>& set, int targetSum, int populationSize, int maxGenerations,
                           int crossoverMethod, int mutationMethod, int terminationCondition)
{
    int subsetSize = set.size();

    // Initialize population
    vector<vector<int>> population = initializePopulation(populationSize, subsetSize);

    // Main loop
    for (int generation = 0; generation < maxGenerations; ++generation)
    {
        // Evaluate fitness for each individual in the population
        vector<int> populationFitness(populationSize);
        for (int i = 0; i < populationSize; ++i)
        {
            populationFitness[i] = evaluateFitness(population[i], set, targetSum);
        }

        // Check termination condition
        if (terminationCondition == 1 && isSubsetFoundTermination(populationFitness))
            break;
        else if (terminationCondition == 2 && isMaxGenerationsTermination(generation, maxGenerations))
            break;

        // Selection and reproduction
        vector<vector<int>> newPopulation(populationSize);
        for (int i = 0; i < populationSize; ++i)
        {
            // Select parents
            pair<int, int> parents = selectParents(populationFitness);
            int parent1Index = parents.first;
            int parent2Index = parents.second;

            // Perform crossover
            vector<int> child;
            if (crossoverMethod == 1)
                child = performSinglePointCrossover(population[parent1Index], population[parent2Index]);
            else if (crossoverMethod == 2)
                child = performTwoPointsRandomCrossover(population[parent1Index], population[parent2Index]);

            // Perform mutation
            if (mutationMethod == 1)
                performMutation(child);
            else if (mutationMethod == 2)
                performFlipTwoRandomPointsMutation(child);

            // Add child to the new population
            newPopulation[i] = child;
        }

        // Replace the old population with the new population
        population = newPopulation;
    }

    // Find the individual with the minimum fitness (closest to the target sum)
    int bestFitness = INT_MAX;
    int bestIndividualIndex = -1;
    for (int i = 0; i < populationSize; ++i)
    {
        int fitness = evaluateFitness(population[i], set, targetSum);
        if (fitness < bestFitness)
        {
            bestFitness = fitness;
            bestIndividualIndex = i;
        }
    }

    // Return the best individual (subset)
    return population[bestIndividualIndex];
}

// Function to read a set from a file
vector<int> readSetFromFile(string filename)
{
    vector<int> set;

    ifstream inputFile(filename);
    if (!inputFile)
    {
        cout << "Failed to open the file." << endl;
        return set;
    }

    string line;
    if (getline(inputFile, line))
    {
        stringstream ss(line);
        string number;
        while (getline(ss, number, ','))
        {
            set.push_back(stoi(number));
        }
    }
    inputFile.close();

    return set;
}

int main(int argc, char* argv[])
{
    // Check if the required arguments are provided
    if (argc < 6)
    {
        cout << "Usage: ./program <targetSum> <populationSize> <crossoverMethod> <mutationMethod> <terminationCondition>"
             << endl;
        return 0;
    }

    // Parse the arguments from the command line
    int targetSum = stoi(argv[1]);
    int populationSize = stoi(argv[2]);
    int crossoverMethod = stoi(argv[3]);
    int mutationMethod = stoi(argv[4]);
    int terminationCondition = stoi(argv[5]);

    // Subset Sum problem inputs
    vector<int> set = readSetFromFile("set.txt");

    // Genetic algorithm parameters
    int maxGenerations = 1000;

    // Solve Subset Sum problem using genetic algorithm
    vector<int> subset = solveSubsetSum(set, targetSum, populationSize, maxGenerations,
                                        crossoverMethod, mutationMethod, terminationCondition);

    // Print the resulting subset
    cout << "Subset: ";
    for (int element : subset)
        cout << element << " ";
    cout << endl;

    return 0;
}
