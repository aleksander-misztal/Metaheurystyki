#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>
#include <fstream>
#include <sstream>
#include <cmath>

// Generuje podzbiór o zadanym rozmiarze na podstawie danego zbioru
std::vector<int> generateSubset(const std::vector<int> &set, int subsetSize)
{
    std::vector<int> subset(set);
    std::shuffle(subset.begin(), subset.end(), std::mt19937(std::time(0))); // Przetasowanie elementów w celu wygenerowania losowego podzbioru
    subset.resize(subsetSize);
    return subset;
}

// Oblicza sumę elementów w podzbiorze
int calculateSubsetSum(const std::vector<int> &subset)
{
    int sum = 0;
    for (int num : subset)
    {
        sum += num;
    }
    return sum;
}

// Znajduje losowego sąsiada aktualnego podzbioru na podstawie zbioru wejściowego
std::vector<int> findRandomNeighbor(const std::vector<int> &currentSubset, const std::vector<int> &set, int targetSum)
{
    std::vector<int> bestNeighbor = currentSubset;
    int bestSum = calculateSubsetSum(currentSubset);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, currentSubset.size() - 1);

    std::vector<std::vector<int>> neighbors;
    for (int i = 0; i < currentSubset.size(); i++)
    {
        std::vector<int> neighbor = currentSubset;
        neighbor[i] = (neighbor[i] == 0) ? 1 : 0; // Zamiana wartości na przeciwną (0 na 1 i odwrotnie)

        neighbors.push_back(neighbor);
    }

    std::shuffle(neighbors.begin(), neighbors.end(), gen); // Przetasowanie sąsiadów w celu wyboru losowego

    for (const auto &neighbor : neighbors)
    {
        int neighborSum = calculateSubsetSum(neighbor);
        if (std::abs(neighborSum - targetSum) < std::abs(bestSum - targetSum))
        {
            bestNeighbor = neighbor;
            bestSum = neighborSum;
        }
    }

    return bestNeighbor;
}

// Generuje losowy punkt roboczy na podstawie aktualnego podzbioru
std::vector<int> getRandomWorkingPoint(const std::vector<int> &currentSubset, const std::vector<int> &set, int targetSum)
{
    std::vector<int> workingPoint = currentSubset;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dis(0.5, 0.1); // Rozkład normalny

    for (int i = 0; i < currentSubset.size(); i++)
    {
        double randomValue = dis(gen);
        workingPoint[i] = (randomValue >= 0.5) ? 1 : 0; // Wybór losowej wartości (0 lub 1) na podstawie rozkładu normalnego
    }

    return workingPoint;
}

// Odczytuje zbiór liczb z pliku tekstowego
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

// Rozwiązuje problem podzbioru o sumie
std::vector<int> solveSubsetSumProblem(const std::vector<int> &set, int targetSum, double initialTemperature, double coolingRate)
{
    std::vector<int> currentSubset = generateSubset(set, set.size());
    std::vector<int> bestSubset = currentSubset;

    double temperature = initialTemperature;

    while (temperature > 0.1)
    {
        std::vector<int> nextSubset = findRandomNeighbor(currentSubset, set, targetSum);
        int currentSum = calculateSubsetSum(currentSubset);
        int nextSum = calculateSubsetSum(nextSubset);

        double acceptanceProbability = std::exp((nextSum - currentSum) / temperature); // Oblicza prawdopodobieństwo akceptacji nowego podzbioru

        if (std::abs(nextSum - targetSum) < 0.0001 || acceptanceProbability > static_cast<double>(std::rand()) / RAND_MAX)
        {
            currentSubset = nextSubset; // Aktualizuje podzbiór na podstawie wybranego sąsiada
        }

        if (std::abs(nextSum - targetSum) < std::abs(calculateSubsetSum(bestSubset) - targetSum))
        {
            bestSubset = nextSubset; // Aktualizuje najlepszy podzbiór, jeśli znaleziono lepsze rozwiązanie
        }

        temperature *= coolingRate; // Obniża temperaturę
    }

    return bestSubset;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "Użycie: ./program <targetSum> <initialTemperature> <coolingRate>" << std::endl; // Wyświetla komunikat o poprawnym użyciu programu w przypadku braku argumentów
        return 0;
    }

    int targetSum = std::stoi(argv[1]);
    double initialTemperature = std::stod(argv[2]);
    double coolingRate = std::stod(argv[3]);

    std::vector<int> set = readSetFromFile("set.txt");

    std::vector<int> subset = solveSubsetSumProblem(set, targetSum, initialTemperature, coolingRate);

    if (subset.empty())
    {
        std::cout << "Nie można znaleźć podzbioru o żądanej sumie." << std::endl; // Wyświetla komunikat informujący o niemożności znalezienia podzbioru o zadanej sumie
    }
    else
    {
        std::cout << "Podzbiór: ";
        for (int num : subset)
        {
            if (num != 0)
            {
                std::cout << num << " "; // Wyświetla znalezione liczby z podzbioru
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
