#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <cmath> // Added for std::abs

// Struktura reprezentująca ruch w algorytmie Tabu
struct TabuMove
{
    int index;
    int value;
};

// Funkcja haszująca dla std::vector<int>
struct VectorHasher
{
    std::size_t operator()(const std::vector<int> &vec) const
    {
        std::size_t seed = vec.size();
        for (const int &i : vec)
        {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

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

// Funkcja znajdująca najlepszego sąsiada przez zmianę całego podzbioru o rozmiarze 1
std::vector<int> findBestNeighbor(const std::vector<int> &currentSubset, const std::vector<int> &set, int targetSum, const std::unordered_set<std::vector<int>, VectorHasher> &tabuList)
{
    std::vector<int> bestNeighbor = currentSubset;
    int bestSum = calculateSubsetSum(currentSubset);

    for (int i = 0; i < currentSubset.size(); i++)
    {
        std::vector<int> neighbor = currentSubset;
        neighbor.erase(neighbor.begin() + i); // Usunięcie elementu o danym indeksie

        if (tabuList.find(neighbor) == tabuList.end()) // Sprawdzenie, czy sąsiad nie jest na liście tabu
        {
            // Sprawdzenie, czy sąsiad nie zawiera powtórzonych liczb
            std::unordered_set<int> uniqueNums(neighbor.begin(), neighbor.end());
            if (uniqueNums.size() == neighbor.size())
            {
                int neighborSum = calculateSubsetSum(neighbor);
                if (std::abs(neighborSum - targetSum) < std::abs(bestSum - targetSum))
                {
                    bestNeighbor = neighbor;
                    bestSum = neighborSum;
                }
            }
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
    while (std::getline(inputFile, line))
    {
        std::stringstream ss(line);
        std::string numStr;
        while (std::getline(ss, numStr, ','))
        {
            int num = std::stoi(numStr);
            set.push_back(num);
        }
    }

    return set;
}

// Funkcja rozwiązująca problem sumy podzbioru za pomocą algorytmu Tabu Search
std::vector<int> solveSubsetSumProblem(const std::vector<int> &set, int targetSum, int tabuSize)
{
    std::vector<int> currentSubset = generateSubset(set, set.size() / 2); // Początkowy podzbiór - losowa połowa elementów
    std::unordered_set<std::vector<int>, VectorHasher> tabuList;
    int tabuCount = 0;
    std::vector<int> bestSubset = currentSubset;
    int bestSum = calculateSubsetSum(currentSubset);

    while (true)
    {
        std::vector<int> nextSubset = findBestNeighbor(currentSubset, set, targetSum, tabuList);
        if (calculateSubsetSum(nextSubset) == targetSum)
        {
            return nextSubset; // Znaleziono podzbiór o żądanej sumie
        }

        tabuList.insert(nextSubset);
        if (tabuList.size() > tabuSize)
        {
            auto it = tabuList.begin();
            tabuList.erase(it);
        }

        if (std::abs(calculateSubsetSum(nextSubset) - targetSum) < std::abs(bestSum - targetSum))
        {
            bestSubset = nextSubset;
            bestSum = calculateSubsetSum(nextSubset);
        }

        currentSubset = nextSubset;

        tabuCount++;
        if (tabuCount >= tabuSize || tabuList.find(bestSubset) != tabuList.end())
        {
            break; // Osiągnięto limit tabu lub znaleziono najlepszy podzbiór, zatrzymaj
        }
    }

    return bestSubset;
}

int main(int argc, char *argv[])
{
    // Sprawdzenie, czy targetSum i tabuSize są podane jako argumenty wiersza poleceń
    if (argc < 3)
    {
        std::cout << "Podaj żądaną sumę i rozmiar tabu: ./program <targetSum> <tabuSize>" << std::endl;
        return 0;
    }

    // Parsowanie targetSum i tabuSize z argumentów wiersza poleceń
    int targetSum = std::stoi(argv[1]);
    int tabuSize = std::stoi(argv[2]);

    // Odczytanie zbioru z pliku
    std::vector<int> set = readSetFromFile("set.txt");

    // Przykładowe użycie
    std::vector<int> subset = solveSubsetSumProblem(set, targetSum, tabuSize);

    if (subset.empty())
    {
        std::cout << "Nie można znaleźć podzbioru o żądanej sumie." << std::endl;
    }
    else
    {
        int subsetSum = calculateSubsetSum(subset);
        std::cout << "Suma podzbioru: " << subsetSum << std::endl;

        if (subsetSum == targetSum)
        {
            std::cout << "Podzbiór: ";
            for (int num : subset)
            {
                if (num != 0)
                {
                    std::cout << num << " ";
                }
            }
            std::cout << std::endl;
        }
        else
        {
            std::cout << "Nie można znaleźć podzbioru o żądanej sumie." << std::endl;
        }
    }

    return 0;
}
