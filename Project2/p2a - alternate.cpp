// Project 2a
// Greedy Knapsack
// Chris Holtsnider and Matt Bresett
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>
#include <tuple>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include <stack>
#include "knapsack.h"

// sort all items in order of value/weight ratio
// and store the ratios in the index at which they exist in knapsack
vector<pair<double, int>> sortItems(knapsack sack)
{
	vector<pair<double, int>> ratios;

	for (int i = 0; i < sack.getNumObjects(); i++)
	{
		//cout << "sack.getValue(" << i << ")" << " = " << sack.getValue(i) << endl;
		//cout << "sack.getValue(" << i << ")" << " = " << sack.getCost(i) << endl;
		double ratio;
		double val = sack.getValue(i);
		double cost = sack.getCost(i);
		ratio = val / cost;
		//cout << "Ratio: " << ratio;
		//cout << "Pushing " << ratio << " to index " << i << endl;
		pair <double, int> ratInd; // ratio + index
		ratInd.first = ratio;
		ratInd.second = i;
		ratios.push_back(ratInd);
		//cout << endl << endl << endl;
	}

	// this preserves the original ratios vector just in case (may remove it later)
	vector<pair<double,int>> sortedRatios = ratios;

	sort(sortedRatios.begin(), sortedRatios.end());
	reverse(sortedRatios.begin(), sortedRatios.end());

	/*
	for (int i = 0; i < ratios.size(); i++)
	{
		//cout << "Ratio: " << sortedRatios.at(i) << " at index " << i << endl;
		//cout << "" << sortedRatios.at(i) << endl;
	}*/
	cin.get();
	return sortedRatios;
}

knapsack greedyKnapsack(vector<pair<double,int>> sortedSack, knapsack sack)
{
	int sackIndex;

	// outer for loop is iterating through sortedSack ratios
	// look for highest ratio item
	for (int i = 0; i < sack.getNumObjects(); i++)
	{
		// use second value of sorted sack pair as index
		sackIndex = sortedSack.at(i).second;

		//get ratio for specific sack(sackIndex)
		double ratioKnap;
		double valKnap = sack.getValue(sackIndex);
		double costKnap = sack.getCost(sackIndex);
		ratioKnap = valKnap / costKnap;

		if (sack.getCost() + sack.getCost(sackIndex) <= sack.getCostLimit())
		{
			sack.select(sackIndex);
		}
	}

	for (int k = 0; k < sack.getNumObjects(); k++)
	{
		cout<<sack.isSelected(k)<<endl;
	}

	return sack;
}

void main()
{
	char x;
	ifstream fin;
	stack <int> moves;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = "knapsack16.input";

	cout << "Enter filename" << endl;
	//cin >> fileName;

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		cin.get();
		exit(1);
	}

	try
	{
		cout << "Reading knapsack instance" << endl;
		knapsack k(fin);

		vector<pair<double, int>> sortedSack = sortItems(k);

		knapsack newK = greedyKnapsack(sortedSack, k);

		cout << endl << "Best solution" << endl;
		newK.printSolution();
	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}

	cin.get();
}