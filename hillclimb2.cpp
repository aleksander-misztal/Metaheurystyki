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

std::vector<int> deleteValueFromVector(std::vector<int> &vec, int value)
{
    std::vector<int> newDeletedItems;
    for (int n : vec)
    {
        if (n != value)
            newDeletedItems.push_back(n);
    }
    return newDeletedItems;
}

std::pair<std::vector<int>, std::vector<int>> generateRandomNeighbor(std::vector<int> randomNeighbor, std::vector<int>deletedItems){
   
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, randomNeighbor.size() - 1);

    bool randomBool = dist(gen) % 2 == 0;
    int randomIndex = dist(gen);

    if (randomBool && deletedItems.empty())
        randomBool = false;
    
    if (!randomBool && !(std::count(deletedItems.begin(), deletedItems.end(), randomNeighbor[randomIndex]) > 0))
    {
        if(randomNeighbor[randomIndex] == 0){
            randomBool = true;
        }
        else{
        deletedItems.push_back(randomNeighbor[randomIndex]);
        randomNeighbor[randomIndex] = 0;
        std::cout << "DELETE & NIE ISTNIEJE JESZCZE" << std::endl;
        }
    }

    if (randomBool)
    {    
        // 1. pobierz randomowy element z deleted
        std::uniform_int_distribution<int> dist(0, deletedItems.size() - 1);
        int randomDeletedIndex = dist(gen);

        int item_from_deleted = deletedItems[randomDeletedIndex];
        int item_from_current = randomNeighbor[randomIndex];

        std::cout << "PICKED DELETED : " << item_from_deleted << std::endl;
        // 2. Usun wybranca z usunietych
        deletedItems = deleteValueFromVector(deletedItems, item_from_deleted);
        
        // 2. jezeli element !=0 i nie ma go w deleted to dodaj do deleted
        if (randomNeighbor[randomIndex] != 0)
            deletedItems.push_back(item_from_current);
            
        // 3. podmien na randomowy usuniety
        randomNeighbor[randomIndex] = item_from_deleted;
    }

    return std::make_pair(randomNeighbor, deletedItems);
}

// Funkcja rozwiązująca problem sumy podzbioru przy użyciu algorytmu hill climbing
std::vector<int> solveSubsetSumProblem(const std::vector<int> &set, int targetSum, int max_iterations)
{
    std::vector<int> currentSubset = generateSubset(set, set.size());
    std::vector<int> deletedItems;
    int iteration = 0;
    while (iteration < max_iterations)
    {

        std::pair<std::vector<int>, std::vector<int>>  randomPair= generateRandomNeighbor(currentSubset, deletedItems);
        std::vector<int> randomNeighbor = randomPair.first;
        std::vector<int> newDeletedItems = randomPair.second;

        currentSubset = randomNeighbor;
        deletedItems = newDeletedItems;

        std::cout << iteration + 1 << std::endl;
        std::cout << "CURRENT : ";
        for (int c : currentSubset)
        {
            std::cout << c << ", ";
        }
        std::cout << std::endl;
     
        std::cout << "DELETED : ";
        for (int d : deletedItems)
        {
            std::cout << d << ", ";
            if(d == 0){
                return {};
            }
        }
        std::cout << std::endl;
        std::cout <<"SUM : "<< calculateSubsetSum(currentSubset)<< std::endl;


        if(calculateSubsetSum(currentSubset) == targetSum){
            return currentSubset;
        }

        iteration++;
        std::cout << std::endl;
        std::cout << std::endl;
    }

    return {};
}

int main(int argc, char *argv[])
{
    // Sprawdzenie, czy targetSum jest podane jako argument wiersza poleceń
    if (argc < 3)
    {
        std::cout << "Podaj żądaną sumę: ./program <targetSum> <numOfIterations>" << std::endl;
        return 0;
    }

    // Parsowanie targetSum z argumentu wiersza poleceń
    int targetSum = std::stoi(argv[1]);
    int maxIter = std::stoi(argv[2]);

    // Odczytanie zbioru z pliku
    std::vector<int> set = readSetFromFile("set.txt");

    // Przykładowe użycie
    std::vector<int> subset = solveSubsetSumProblem(set, targetSum, maxIter);

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
