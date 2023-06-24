#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>
#include <fstream>
#include <sstream>
#include <unordered_set>

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

std::vector<int> findBestNeighbor(const std::vector<int> &set, const std::vector<int> &subset, int targetSum, const std::unordered_set<std::string>& tabuList)
{
    std::vector<int> newNeighbor;
    std::vector<int> bestNeighbor;
    int bestSum = calculateSum(set, subset);
    int neighborSum;

    for (int i = 0; i < set.size(); ++i) {
        newNeighbor = subset;
        newNeighbor[i] = (subset[i] == 0) ? 1 : 0;

        // Convert the newNeighbor vector to a string representation
        std::stringstream ss;
        for (int num : newNeighbor) {
            ss << num;
        }
        std::string neighborString = ss.str();

        // Check if the neighbor is in the tabu list
        if (tabuList.find(neighborString) != tabuList.end()) {
            continue;  // Skip this neighbor if it's in the tabu list
        }

        neighborSum = calculateSum(set, newNeighbor);

        if (std::abs(neighborSum - targetSum) < std::abs(bestSum - targetSum)) {
            bestSum = neighborSum;
            bestNeighbor = newNeighbor;
        }
    }

    return bestNeighbor;
}

std::vector<int> solveSubsetSumProblem(const std::vector<int> &set, int targetSum, int tabuSize)
{
    int set_length = set.size();
    std::vector<int> bestSubset(set_length, 1);

    std::unordered_set<std::string> tabuList;
    int iterations = 0;

    while (calculateSum(set, bestSubset) != targetSum) {
        std::vector<int> bestNeighbor = findBestNeighbor(set, bestSubset, targetSum, tabuList);
        bestSubset = bestNeighbor;

        // Add the current best neighbor to the tabu list
        std::stringstream ss;
        for (int num : bestNeighbor) {
            ss << num;
        }
        std::string neighborString = ss.str();
        tabuList.insert(neighborString);

        // Remove the oldest tabu elements if the tabu list exceeds the tabu size
        if (tabuList.size() > tabuSize && tabuSize != -1) {
            auto it = tabuList.begin();
            tabuList.erase(it);
        }

        iterations++;

        // Add a stopping condition to prevent infinite loops
        if (iterations >= 1000) {
            break;
        }
    }

    return bestSubset;
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
    // Sprawdzenie, czy targetSum i tabuSize są podane jako argumenty wiersza poleceń
    if (argc < 3)
    {
        std::cout << "Podaj żądaną sumę oraz rozmiar tabu: ./program <targetSum> <tabuSize>" << std::endl;
        return 0;
    }

    // Parsowanie targetSum i tabuSize
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
        std::cout << "Podzbiór: ";
        for (int num : subset)
        {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
