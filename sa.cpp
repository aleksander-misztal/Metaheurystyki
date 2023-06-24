#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>
#include <fstream>
#include <sstream>
#include <cmath>
#include <random>

void printVector(std::vector<int> v)
{
    std::cout << std::endl;
    for (int e : v)
    {
        std::cout << e << ", ";
    }
    std::cout << std::endl;
}

int calculateSum(const std::vector<int> &set, const std::vector<int> &neighbor)
{
    int sum = 0;
    for (size_t i = 0; i < neighbor.size(); ++i)
    {
        sum += set[i] * neighbor[i];
    }
    return sum;
}

double calculateCostDifference(int currentSum, int newSum, int targetSum, double temperature)
{
    return (currentSum - newSum) / temperature;
}

std::vector<int> findBestNeighbor(const std::vector<int> &set, const std::vector<int> &subset, int targetSum)
{
    std::vector<int> newNeighbor;
    std::vector<int> bestNeighbor;
    int bestSum = calculateSum(set, subset);
    int neighborSum;

    for (int i = 0; i < set.size(); i++)
    {
        newNeighbor = subset;
        newNeighbor[i] = (subset[i] == 0) ? 1 : 0;
        neighborSum = calculateSum(set, newNeighbor);

        if (std::abs(neighborSum - targetSum) < std::abs(bestSum - targetSum))
        {
            bestSum = neighborSum;
            bestNeighbor = newNeighbor;
        }
    }

    return bestNeighbor;
}

// Funkcje temperatury dla różnych schematów
double logarithmicTemperature(int k)
{
    return 1.0 / std::log(k);
}

double linearTemperature(int k)
{
    return 1.0 / k;
}

double geometricTemperature(int k, double a)
{
    return std::pow(a, k);
}

std::vector<int> solveSubsetSumProblem(const std::vector<int> &set, int targetSum, double initialTemperature, double coolingRate, int temperatureScheme)
{
    int setLength = set.size();
    std::vector<int> bestSubset(setLength, 1);
    std::vector<int> currentSubset = bestSubset;

    int currentSum = calculateSum(set, currentSubset);
    double currentCost = std::numeric_limits<double>::max();

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    double temperature = initialTemperature;
    int iteration = 1;

    while (temperature > 0.1)
    {
        for (int i = 0; i < 100; ++i)
        {
            std::vector<int> newNeighbor = findBestNeighbor(set, currentSubset, targetSum);
            int newSum = calculateSum(set, newNeighbor);
            double costDifference = calculateCostDifference(currentSum, newSum, targetSum, temperature);

            if (costDifference > 0 || std::exp(costDifference / temperature) > distribution(generator))
            {
                currentSubset = newNeighbor;
                currentSum = newSum;
                currentCost = costDifference;
            }

            if (currentSum == targetSum)
            {
                return currentSubset;
            }
        }

        // Calculate new temperature based on the selected scheme
        if (temperatureScheme == 1)
        {
            temperature = logarithmicTemperature(iteration);
        }
        else if (temperatureScheme == 2)
        {
            temperature = linearTemperature(iteration);
        }
        else if (temperatureScheme == 3)
        {
            temperature = geometricTemperature(iteration, coolingRate);
        }
        iteration++;
    }

    return currentSubset;
}

std::vector<int> readSetFromFile(const std::string &filename)
{
    std::vector<int> set;

    std::ifstream inputFile(filename);
    if (!inputFile)
    {
        std::cout << "Nie udało się otworzyć pliku." << std::endl;
        return set;
    }

    std::string line;
    if (std::getline(inputFile, line))
    {
        std::stringstream ss(line);
        std::string number;
        while (std::getline(ss, number, ','))
        {
            set.push_back(std::stoi(number));
        }
    }
    inputFile.close();

    return set;
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cout << "Podaj żądaną sumę: ./program <targetSum> <initialTemperature> <coolingRate> <temperatureScheme>" << std::endl;
        return 0;
    }

    int targetSum = std::stoi(argv[1]);
    double initialTemperature = std::stod(argv[2]);
    double coolingRate = std::stod(argv[3]);
    int temperatureScheme = std::stod(argv[4]);

    std::vector<int> set = readSetFromFile("set.txt");

    std::vector<int> subset = solveSubsetSumProblem(set, targetSum, initialTemperature, coolingRate, temperatureScheme);

    if (subset.empty())
    {
        std::cout << "Nie można znaleźć podzbioru o żądanej sumie." << std::endl;
    }
    else
    {
        std::cout << "Podzbiór: ";
        for (int num : subset)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
