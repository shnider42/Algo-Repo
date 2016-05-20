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
vector<double> sortItems(knapsack sack)
{
	vector<double> ratios;

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
		ratios.push_back(ratio);
		//cout << endl << endl << endl;
	}

	vector<double> sortedRatios = ratios;

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

knapsack greedyKnapsack(vector<double> sortedSack, knapsack sack)
{
	// outer for loop is iterating through sortedSack ratios
	// look for highest ratio item
	for (int i = 0; i < sack.getNumObjects(); i++)
	{
		// inner for loop is each item in sack
		// not a big fan of the nested for loop here, but it's a temporary solution
		for (int k = 0; k < sack.getNumObjects(); k++)
		{
			//get ratio for specific sack(k)
			double ratioKnap;
			double valKnap = sack.getValue(k);
			double costKnap = sack.getCost(k);
			ratioKnap = valKnap / costKnap;

			// if the high ratio value(i) is the item that k is currently on, select it
			// also check if there is a conflict (cost of (k) + total selected cost <= costLimit)
			// also check if we're on the last value
			if (sortedSack.at(i) == ratioKnap && (sack.getCost() + sack.getCost(k) <= sack.getCostLimit()))
			{
				sack.select(k);
			}
			// could shorten the for loop here by first counting how many duplicate ratios there are then 
				// breaking after that many iterations for that particular ratio
			// will still have to get to the end of the vector
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

		vector<double> sortedSack = sortItems(k);

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