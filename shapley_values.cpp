// ShapleyAI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include <list>
#include <vector>
#include <time.h>
#include <algorithm>
using namespace std;

int agentCount;
vector<vector<int>> permutations;


int GetFactorial(int num)
{
	int factorial = 1;
	for (int i = 1; i <= num; i++)
	{
		factorial *= i;
	}
	return factorial;
}

//list<int> CreateCharacterTable()
//{
// list<int> characterTable;
// int numOfAgents;
// cin >> numOfAgents;
// for (int i = 0; i < pow(numOfAgents, 2); i++)
// {
// characterTable.push_back((rand() % 200) - 100);
// }
// cout << characterTable[0];
// vector<int> characterTable(pow(numOfAgents, 2));
//}

vector<int> CreateRandomCharacterTable()
{
	agentCount = (rand() % 6) + 2;
	agentCount = 3;
	//cin >> numOfAgents;
	vector<int> characterTable(pow(2, agentCount));

	for (int i = 0; i < pow(2, agentCount); i++)
	{
		characterTable[i] = (rand() % 200) - 100;
	}

	return characterTable;
}

void GenerateAllPermutations(vector<int>& v, int size)
{
	// if size becomes 1 then adds on the obtained permutation
	if (size == 1) {
		permutations.push_back(v);
		return;
	}

	for (int i = 0; i < size; i++) {
		GenerateAllPermutations(v, size - 1);
		if (i < size - 1)
		{
			// if size is odd, swap first and last element
			if (size % 2 == 1)
			{
				iter_swap(v.begin(), v.begin() + size - 1);
			}
			// If size is even, swap ith and last element
			else
			{
				iter_swap(v.begin() + i, v.begin() + size - 1);
			}
		}
	}
}

vector<vector<int>> GenerateAllCombinations()
{
	vector<vector<int>> combinations = { {}, {0} };
	// this size value is necessary as otherwise when looping through the size of combinations, as i add more the size just increases. infinite loop. doesnt work
	int size = combinations.size();
	vector<int> temp;
	for (int i = 1; i < agentCount; i++)
	{
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < combinations[x].size(); y++)
			{
				temp.push_back(combinations[x][y]);
			}
			temp.push_back(i);

			combinations.push_back(temp);

			temp.clear();
		}
		size = combinations.size();
	}

	return combinations;
}

// hey dont forget that v1 isnt sorted here, so if you use this for something where v1 has to be an unsorted list, uhh, just remember that this is why its not working. also v1 and v2 is a reference, not a copy.
bool DoVectorsContainSameElements(vector<int>& v1, vector<int>& v2)
{
	if (v1.size() != v2.size())
	{
		return false;
	}

	//sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());

	return v1 == v2;
}

vector<float> CalculateShapleyValues(vector<int>& characterTable)
{
	permutations.clear();
	vector<int> temp;
	for (int i = 0; i < agentCount; i++)
	{
		temp.push_back(i);
	}
	GenerateAllPermutations(temp, agentCount);

	temp.clear();

	vector<vector<int>> combinations = GenerateAllCombinations();

	//for (int i = 0; i < combinations.size(); i++)
	//{
	// for (int x = 0; x < combinations[i].size(); x++)
	// {
	// cout << combinations[i][x] << ", ";
	// }
	// cout << endl;
	//}

	vector<int> currentCombination;
	vector<float> shapleyValues(agentCount);

	for (int i = 0; i < agentCount; i++)
	{
		for (int permutation_index = 0; permutation_index < permutations.size(); permutation_index++)
		{
			for (int y = 0; y < agentCount; y++)
			{
				currentCombination.push_back(permutations[permutation_index][y]);
				if (permutations[permutation_index][y] == i)
				{
					break;
				}
			}

			for (int y = 0; y < combinations.size(); y++)
			{
				if (DoVectorsContainSameElements(combinations[y], currentCombination))
				{
					currentCombination.pop_back();
					for (int z = 0; z < combinations.size(); z++)
					{
						if (DoVectorsContainSameElements(combinations[z], currentCombination))
						{
							shapleyValues[i] = shapleyValues[i] + (characterTable[y] - characterTable[z]);
							break;
						}

					}

					break;
				}
			}
			currentCombination.clear();
		}

		//shapleyValues[shapleyValues.size() - 1] = shapleyValues[shapleyValues.size() - 1] / Factorial(numOfAgents);
		shapleyValues[i] = shapleyValues[i] / float(Factorial(agentCount));
	}
	return shapleyValues;
}

//int main()
//{
// vector<int> v = { 0, 1, 2, 3 };
// GenerateAllPermutations(v, v.size());
// for (int i = 0; i < permutations.size(); i++)
// {
// for (int x = 0; x < permutations[i].size(); x++)
// {
// cout << permutations[i][x] << ", ";
// }
// cout << endl;
// }
//}

int main()
{
	vector<int> characterTable;
	srand(time(NULL));
	characterTable = CreateRandomCharacterTable();

	for (int i = 0; i < characterTable.size(); i++)
	{
		cout << characterTable[i] << " ";
	}
	cout << endl << endl;

	vector<float> shapleyValues = CalculateShapleyValues(characterTable);

	for (int i = 0; i < shapleyValues.size(); i++)
	{
		cout << shapleyValues[i] << " ";
	}
	cout << endl;

	//for (int i = 0; i < 0 + 1; i++)
	//{
	// cout << i;
	//}

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
// 1. Use the Solution Explorer window to add/manage files
// 2. Use the Team Explorer window to connect to source control
// 3. Use the Output window to see build output and other messages
// 4. Use the Error List window to view errors
// 5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
// 6. In the future, to open this project again, go to File > Open > Project and select the .sln file