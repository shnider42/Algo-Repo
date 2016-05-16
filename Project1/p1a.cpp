// Project 1a: Solving knapsack using exhaustive search
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <stack>
#include <time.h>
#include <windows.h>
#include <direct.h>
#include <math.h>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

void exhaustiveKnapsack(knapsack&, int);
string getcwd1();
void pickKnapsack(knapsack&, vector<bool>);
void clearKnapsack(knapsack&);
vector<bool> incrementSack(vector<bool>, int);
bool allTrue(vector<bool>);

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
   //char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   ofstream outputFile;
   
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.

   /*string CWD;
   CWD = getcwd1();
   cout << CWD;
   cout << "\n\n";
   
   // appending file name to load correctly
   fileName = CWD + "\\\knapsack32.input";
   cout << fileName;
   cin.get();*/

   cout << "Enter filename" << endl;
   cin >> fileName;
   fin.open(fileName.c_str());
   //fin.open(fileName);
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

      exhaustiveKnapsack(k, 600);
	  cout << "Weight limit was: " << k.getCostLimit() << endl;
      cout << endl << "Best solution" << endl;
      k.printSolution();
      outputSolutionToFile(outputFile, k);
      outputFile.close();
   }    
   catch (indexRangeError &ex) 
   { 
      cout << ex.what() << endl; 
	  cin.get();
	  exit(1);
   }
   catch (rangeError &ex)
   {
      cout << ex.what() << endl; 
	  cin.get();
	  exit(1);
   }
	
   cin.get(); // pause to read output
}

//brute force search of the best-valued backpack that remains under the weight limit
//note that second parameter is max search time in seconds
void exhaustiveKnapsack(knapsack& sack, int timeLimit)
{
	// Get start time to be used in while loop
	time_t startTime;
	time(&startTime);

	//bestValue is the current best value of the solutions checked
	//initialize it to 0
	int bestValue = 0;
	
	//this vector represents the code for picking items for a solution - each boolean maps to a possible item
	//true means an item will be picked, false is not picked
	vector<bool> currentNumber;
	for (int i = 0; i < sack.getNumObjects(); i++) {
		currentNumber.push_back(false);
	}
	
	//boolean that decides when to stop the while loop
	//will be set to true when we are on the last feasible solution
	bool done = false;
	
	//store the bestNumber so we can re-map the knapsack at the end for printing
	//initialize it to the first number
	vector<bool> bestNumber = currentNumber;

	// a while loop that expires when time limit is complete by checking difference of 
	// start time and current time at every loop
	while (!done)
	{
		// Get current time and stop when greater than input time limit
		time_t newTime;
		time(&newTime);
		
		//if the time threshold has been reached
		if (difftime(newTime, startTime) > timeLimit)
		{
			cout << "Time limit expired";
			break;
		}

		//if all of the feasible solutions have been checked
		if (allTrue(currentNumber)) {
			done = true;
		}

		pickKnapsack(sack, currentNumber);

		//if the newly picked knapsack is under the weight limit and has a better value, update our variables
		if (sack.getValue() > bestValue && sack.getCost() <= sack.getCostLimit()) {
			bestValue = sack.getValue();
			bestNumber = currentNumber;
		}

		//unselect everything from the knapsack
		clearKnapsack(sack);
		
		//increment our code to the next code
		currentNumber = incrementSack(currentNumber, currentNumber.size() - 1);
	}

	//set the knapsack to what was found as the best solution
	pickKnapsack(sack, bestNumber);
}

//get the current working directory to allow for smoother filename finding
string getcwd1()
{
	char* a_cwd = _getcwd(NULL, 0);
	string s_cwd(a_cwd);
	free(a_cwd);
	return s_cwd;
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

//increments the current code to the next code
//works like adding 1 to a binary number
vector<bool> incrementSack(vector<bool> currentNumber, int index) {
	
	//same as adding one to a binary number
	currentNumber.at(index) = !currentNumber.at(index);
	
	//if we went from true to false (1 to 0), we need to flip the previous index
	//the same as if we carry a 1 to the next bit in binary addition
	if (!currentNumber.at(index) && index > 0)
	{
		currentNumber = incrementSack(currentNumber, index - 1);
	}
	return currentNumber;
}

//unslects all items in the knapsack
void clearKnapsack(knapsack& sack) {
	for (int i = 0; i < sack.getNumObjects(); i++) {
		sack.unSelect(i);
	}
}

//checks if all the items in the vector parameter are true
//used in checking if all feasible solutions have been checkeds
bool allTrue(vector<bool> currentNumber) {
	for (int i = 0; i < currentNumber.size(); i++) {
		if (!currentNumber.at(i)) {
			return false;
		}
	}
	return true;
}
