#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>
#include <fstream>
#include <sstream>

// Funkcja generująca losowy podzbiór o określonym rozmiarze
std::vector<int> generateSubset(const std::vector<int> &set, int subsetSize)
{
    std::vector<int> subset(set);
    std::shuffle(subset.begin(), subset.end(), std::mt19937(std::time(0))); // Użycie generatora mt do mieszania
    subset.resize(subsetSize);
    return subset;
}

// Funkcja obliczająca sumę elementów podzbioru
int calculateSubsetSum(const std::vector<int> &subset)
{
    int sum = 0;
    for (int num : subset)
    {
        sum += num;
    }
    return sum;
}

// Funkcja znajdująca losowego sąsiada spośród otoczenia przez zmianę jednego elementu na raz
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
        neighbor[i] = (neighbor[i] == 0) ? 1 : 0; // Zmiana jednego elementu

        neighbors.push_back(neighbor);
    }

    std::shuffle(neighbors.begin(), neighbors.end(), gen); // Losowe zamieszanie kolejności sąsiadów

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

// Funkcja odczytująca zbiór z pliku
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

// Funkcja rozwiązująca problem sumy podzbioru przy użyciu algorytmu hill climbing
std::vector<int> solveSubsetSumProblem(const std::vector<int> &set, int targetSum, int maxIterations)
{
    std::vector<int> currentSubset = generateSubset(set, set.size());

    int iterations = 0;
    while (calculateSubsetSum(currentSubset) != targetSum && iterations < maxIterations)
    {
        std::vector<int> nextSubset = findRandomNeighbor(currentSubset, set, targetSum);
        if (calculateSubsetSum(nextSubset) >= calculateSubsetSum(currentSubset))
        {
            break; // Osiągnięto lokalne optimum, zatrzymaj
        }
        currentSubset = nextSubset;
        iterations++;
    }

    if (calculateSubsetSum(currentSubset) == targetSum)
    {
        // Usuwanie zer z podzbioru
        std::vector<int> finalSubset;
        for (int num : currentSubset)
        {
            if (num != 0)
            {
                finalSubset.push_back(num);
            }
        }
        return finalSubset;
    }
    else
    {
        return {}; // Zwróć pusty wektor, aby wskazać, że nie można znaleźć podzbioru o żądanej sumie
    }
}

int main(int argc, char *argv[])
{
    // Sprawdzenie, czy targetSum i maxIterations są podane jako argumenty wiersza poleceń
    if (argc < 3)
    {
        std::cout << "Podaj żądaną sumę i maksymalną liczbę iteracji: ./program <targetSum> <maxIterations>" << std::endl;
        return 0;
    }

    // Parsowanie targetSum i maxIterations z argumentów wiersza poleceń
    int targetSum = std::stoi(argv[1]);
    int maxIterations = std::stoi(argv[2]);

    // Odczytanie zbioru z pliku
    std::vector<int> set = readSetFromFile("set.txt");

    // Przykładowe użycie
    std::vector<int> subset = solveSubsetSumProblem(set, targetSum, maxIterations);

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
