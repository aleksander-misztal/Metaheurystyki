#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>
#include <fstream>
#include <sstream>

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

std::vector<int> findBestNeighbor(const std::vector<int> &set, const std::vector<int> &subset, int targetSum)
{
    std::vector<int>newNeighbor;
    std::vector<int>bestNeighbor;
    int bestSum = calculateSum(set, subset);
    int neighborSum;

    for(int i=0 ; i<set.size() ; i++){
        std::cout<<"Neighbour:"<<i;
        newNeighbor = subset;
        newNeighbor[i] = (subset[i] == 0) ? 1 : 0;
        printVector(newNeighbor);
        neighborSum = calculateSum(set, newNeighbor);
        std::cout<<"SUMA: "<<neighborSum<<std::endl;

        if (std::abs(neighborSum - targetSum) < std::abs(bestSum - targetSum)){
            bestSum = neighborSum;
            bestNeighbor = newNeighbor;
        }
    }
    std::cout<<std::endl;
    std::cout<<"BEST SUM : "<<bestSum<<std::endl;
    std::cout<<"BEST SUBSET"<<std::endl;
    printVector(bestNeighbor);
    std::cout<<std::endl;
    std::cout<<std::endl;
    return bestNeighbor;
}

// Funkcja rozwiązująca problem sumy podzbioru przy użyciu algorytmu hill climbing
std::vector<int> solveSubsetSumProblem(const std::vector<int> &set, int targetSum)
{
    int set_length = set.size();
    std::vector<int>bestSubset = set;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1); // Distribution for generating random numbers 0 or 1

    for (int i = 0; i < set_length; i++) {
        bestSubset[i] = dis(gen); // Set element at index i to a random number (0 or 1)
    }

    while(calculateSum(set, bestSubset) != targetSum){
        std::vector<int> bestNeighbor = findBestNeighbor(set, bestSubset, targetSum);
        bestSubset = bestNeighbor;

    }
    return bestSubset;
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

int main(int argc, char *argv[])
{
    // Sprawdzenie, czy targetSum jest podane jako argument wiersza poleceń
    if (argc < 2)
    {
        std::cout << "Podaj żądaną sumę: ./program <targetSum>" << std::endl;
        return 0;
    }

    // Parsowanie targetSum z argumentu wiersza poleceń
    int targetSum = std::stoi(argv[1]);

    // Odczytanie zbioru z pliku
    std::vector<int> set = readSetFromFile("set.txt");

    // Przykładowe użycie
    std::vector<int> subset = solveSubsetSumProblem(set, targetSum);

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
