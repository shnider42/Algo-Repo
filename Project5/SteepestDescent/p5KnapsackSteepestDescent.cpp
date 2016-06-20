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
#include <algorithm>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include <stack>
#include "knapsack.h"

// sort all items in order of value/weight ratio
// and store the ratios in the index at which they exist in knapsack
vector< pair<double, int> > sortItems(knapsack sack)
{
	vector< pair<double, int> > ratios;

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
	vector< pair<double,int> > sortedRatios = ratios;

	sort(sortedRatios.begin(), sortedRatios.end());
	reverse(sortedRatios.begin(), sortedRatios.end());
	
	return sortedRatios;
}

knapsack greedyKnapsack(vector< pair<double,int> > sortedSack, knapsack sack)
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

	return sack;
}

//picks which items inn the knapsack to select based on the provided code
//each item corresponds to an index in the vector - true will pick the item, false will not
void pickKnapsack(knapsack& sack, vector<bool> bits) {
	for (int i = 0; i < bits.size(); i++) {
		if (bits.at(i)) {
			sack.select(i);
		}
	}
	//cout << sack.getValue() << "\t" << sack.getCost() << endl;
}

//get best neighbor of flipping two items (selecting or unselecting)
knapsack getBestTwoOptNeighbor(knapsack sack, time_t startTime, int timeLimit) {
	knapsack best = sack;
	vector<bool> bits, newBits;
	for (int i = 0; i < sack.getNumObjects(); i++){
		if(sack.isSelected(i)) {
			bits.push_back(true);
		}
		else {
			bits.push_back(false);
		}
	}
	
	//first and second represent indicies to flip
	int first = 0;
	while(first != (sack.getNumObjects() - 2)) {
		time_t newTime;
		time(&newTime);
		
		//if the time threshold has been reached
		if (difftime(newTime, startTime) > timeLimit)
		{
			cout << "Time limit expired" << endl;
			break;
		}
		
		for (int second = first + 1; second < sack.getNumObjects(); second++) {
			time(&newTime);
		
			//if the time threshold has been reached
			if (difftime(newTime, startTime) > timeLimit)
			{
				cout << "Time limit expired" << endl;
				break;
			}
			
			knapsack temp = sack;
			newBits = bits;
			
			//flip our two indicies
			newBits[first] = !newBits[first];
			newBits[second] = !newBits[second];
			pickKnapsack(temp, newBits);
			if(temp.getValue() > best.getValue() && temp.getCost() < temp.getCostLimit()) {
				best = temp;
			}
		}
		first++;
	}
	return best;	 
}

//find solution using steepest descent
knapsack steepestDescent(knapsack sack, int timeLimit)
{
	time_t startTime;
	time(&startTime);
	
	knapsack champ = sack;
	bool done = false;
	while (!done) {
		time_t newTime;
		time(&newTime);
		
		//if the time threshold has been reached
		if (difftime(newTime, startTime) > timeLimit)
		{
			cout << "Time limit expired" << endl;
			break;
		}
		
		//get the best immediate neighbor
		knapsack neighbor = getBestTwoOptNeighbor(champ, startTime, timeLimit);
		
		//if its better, update and loop
		if (neighbor.getValue() > champ.getValue()) {
			champ = neighbor;
		}
		//else we are done
		else {
			done = true;
		}
	}
	return champ;	
}

//uses an outside ofstream variable to output the best knapsack solution, rather than using cout
void outputSolutionToFile(ofstream& fout, knapsack k) 
{
   fout << "Best Solution: " << endl;
   fout << "Total value: " << k.getValue() << endl;
   fout << "Total cost: " << k.getCost() << endl << endl;

   // Print out objects in the solution
   for (int i = 0; i < k.getNumObjects(); i++)
      if (k.isSelected(i))
	 fout << i << "  " << k.getValue(i) << " " << k.getCost(i) << endl;

   fout << endl;
}

int main()
{
   char x;
   ifstream fin;
   string fileName;
   ofstream outputFile;
   int numColors, numConflicts;
   
    cout << "Enter filename" << endl;
    cin >> fileName;
   
   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
	  cin.get();
      exit(1);
   }
   
   try
    {
        string outFile = fileName.substr(0, fileName.length() - 5) + "output";
   	    outputFile.open(outFile.c_str());
		cout << "Reading knapsack instance" << endl;
		knapsack k(fin);

		vector< pair<double, int> > sortedSack = sortItems(k);

		knapsack greedyK = greedyKnapsack(sortedSack, k);
		knapsack newK = steepestDescent(greedyK, 300);

		cout << endl << "Best solution" << endl;
		newK.printSolution();	
        outputSolutionToFile(outputFile, newK);
        outputFile.close();
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
