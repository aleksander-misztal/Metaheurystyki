#include <iostream>
#include <vector>
#include <random>

using namespace std;

// Jednopunktowe krzyżowanie
vector<int> crossoverSinglePoint(const vector<int> &parent1, const vector<int> &parent2, int crossoverPoint)
{
    vector<int> child(parent1.begin(), parent1.begin() + crossoverPoint);
    child.insert(child.end(), parent2.begin() + crossoverPoint, parent2.end());
    return child;
}

// Dwupunktowe krzyżowanie
vector<int> crossoverTwoPoints(const vector<int> &parent1, const vector<int> &parent2, int crossoverPoint1, int crossoverPoint2)
{
    vector<int> child(parent1.begin(), parent1.begin() + crossoverPoint1);
    child.insert(child.end(), parent2.begin() + crossoverPoint1, parent2.begin() + crossoverPoint2);
    child.insert(child.end(), parent1.begin() + crossoverPoint2, parent1.end());
    return child;
}

// Mutacja jednopunktowa
void mutateSinglePoint(vector<int> &individual, int mutationPoint)
{
    individual[mutationPoint] = !individual[mutationPoint];
}

// Mutacja losowa
void mutateRandom(vector<int> &individual)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, individual.size() - 1);
    int mutationPoint = dis(gen);
    individual[mutationPoint] = !individual[mutationPoint];
}

// Sprawdza, czy suma elementów w podzbiorze równa się oczekiwanej wartości
bool isSubsetSum(vector<int> &subset, int targetSum)
{
    int currentSum = 0;
    for (int element : subset)
    {
        currentSum += element;
        if (currentSum == targetSum)
            return true;
    }
    return false;
}

// Warunek zakończenia: Osiągnięcie maksymalnej liczby pokoleń
bool isMaxGenerationReached(int currentGeneration, int maxGeneration)
{
    return currentGeneration >= maxGeneration;
}

// Test dla jednopunktowego krzyżowania
void testCrossoverSinglePoint()
{
    vector<int> parent1 = {1, 2, 3, 4, 5};
    vector<int> parent2 = {6, 7, 8, 9, 10};
    int crossoverPoint = 3;

    vector<int> child = crossoverSinglePoint(parent1, parent2, crossoverPoint);

    cout << "Parent 1: ";
    for (int gene : parent1)
        cout << gene << " ";
    cout << endl;

    cout << "Parent 2: ";
    for (int gene : parent2)
        cout << gene << " ";
    cout << endl;

    cout << "Child: ";
    for (int gene : child)
        cout << gene << " ";
    cout << endl;
}

// Test dla dwupunktowego krzyżowania
void testCrossoverTwoPoints()
{
    vector<int> parent1 = {1, 2, 3, 4, 5};
    vector<int> parent2 = {6, 7, 8, 9, 10};
    int crossoverPoint1 = 2;
    int crossoverPoint2 = 4;

    vector<int> child = crossoverTwoPoints(parent1, parent2, crossoverPoint1, crossoverPoint2);

    cout << "Parent 1: ";
    for (int gene : parent1)
        cout << gene << " ";
    cout << endl;

    cout << "Parent 2: ";
    for (int gene : parent2)
        cout << gene << " ";
    cout << endl;

    cout << "Child: ";
    for (int gene : child)
        cout << gene << " ";
    cout << endl;
}

// Test dla mutacji jednopunktowej
void testMutateSinglePoint()
{
    vector<int> individual = {1, 0, 1, 0, 1};
    int mutationPoint = 2;

    cout << "Before mutation: ";
    for (int gene : individual)
        cout << gene << " ";
    cout << endl;

    mutateSinglePoint(individual, mutationPoint);

    cout << "After mutation: ";
    for (int gene : individual)
        cout << gene << " ";
    cout << endl;
}

// Test dla mutacji losowej
void testMutateRandom()
{
    vector<int> individual = {1, 0, 1, 0, 1};

    cout << "Before mutation: ";
    for (int gene : individual)
        cout << gene << " ";
    cout << endl;

    mutateRandom(individual);

    cout << "After mutation: ";
    for (int gene : individual)
        cout << gene << " ";
    cout << endl;
}

// Test dla warunku zakończenia: osiągnięcie zadanego dopasowania
void testIsSubsetSum()
{
    vector<int> subset = {1, 3, 5, 7, 9};
    int targetSum = 12;

    bool result = isSubsetSum(subset, targetSum);

    cout << "Subset: ";
    for (int element : subset)
        cout << element << " ";
    cout << endl;

    cout << "Target sum: " << targetSum << endl;
    cout << "Result: " << (result ? "Subset sum achieved" : "Subset sum not achieved") << endl;
}

// Test dla warunku zakończenia: osiągnięcie maksymalnej liczby pokoleń
void testIsMaxGenerationReached()
{
    int currentGeneration = 10;
    int maxGeneration = 20;

    bool result = isMaxGenerationReached(currentGeneration, maxGeneration);

    cout << "Current generation: " << currentGeneration << endl;
    cout << "Max generation: " << maxGeneration << endl;
    cout << "Result: " << (result ? "Max generation reached" : "Max generation not reached") << endl;
}

int main()
{
    cout << "CrossoverSinglePoint:" << endl;
    testCrossoverSinglePoint();
    cout << endl;
    cout << "CrossoverTwoPoints:" << endl;
    testCrossoverTwoPoints();
    cout << endl;
    cout << "MutateSinglePoint:" << endl;
    testMutateSinglePoint();
    cout << endl;
    cout << "MutateRandom:" << endl;
    testMutateRandom();
    cout << endl;
    cout << "IsSubsetSum:" << endl;
    testIsSubsetSum();
    cout << endl;
    cout << "isMaxGenerationReached:" << endl;
    testIsMaxGenerationReached();
    cout << endl;

    return 0;
}
