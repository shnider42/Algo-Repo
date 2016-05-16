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

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

void exhaustiveKnapsack(knapsack&, int);
string getcwd1();
string binaryString(int, int);
void pickKnapsack(knapsack&, string);
void clearKnapsack(knapsack&);

int main()
{
   //char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.

   string CWD;
   CWD = getcwd1();
   cout << CWD;
   cout << "\n\n";
   
   // appending file name to load correctly
   fileName = CWD + "\\\knapsack8.input";
   cout << fileName;
   cin.get();

   cout << "Enter filename" << endl;
   //cin >> fileName;
   //fin.open(fileName.c_str());
   fin.open(fileName);
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

      exhaustiveKnapsack(k, 6);

      cout << endl << "Best solution" << endl;
      k.printSolution();
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

void exhaustiveKnapsack(knapsack& sack, int timeLimit)
{
	// Get start time to be used in while loop
	time_t startTime;
	time(&startTime);

	cin.get();

	//currentNumber is used to count through possible solutions
	int currentNumber = 1;

	//bestValue is the current best value of the solutions checked
	int bestValue = 0;

	//bestNumber represents the solution that has the best value
	int bestNumber = 0;

	//bitString is used to map a number to a backpack
	string bitString = "";

	// a while loop that expires when time limit is complete by checking difference of 
	// start time and current time at every loop
	//cout << sack.getCostLimit() << endl;
	while (true)
	{
		// Get current time and stop when greater than input time limit
		time_t newTime;
		time(&newTime);
		if (difftime(newTime, startTime) > timeLimit)
		{
			cout << "Time limit expired";
			break;
		}

		//if all of the feasible solutions have been checked
		if (currentNumber >= pow(2, sack.getNumObjects())) {
			break;
		}

		bitString = binaryString(currentNumber, sack.getNumObjects());

		//cout << currentNumber << "\t" << bitString << "\t";

		pickKnapsack(sack, bitString);

		//if the newly picked knapsack is under the weight limit and has a better value, update our variables
		if (sack.getValue() > bestValue && sack.getCost() <= sack.getCostLimit()) {
			bestValue = sack.getValue();
			bestNumber = currentNumber;
		}

		clearKnapsack(sack);
		currentNumber++;
	}

	//set the knapsack to what was found as the best solution
	bitString = binaryString(bestNumber, sack.getNumObjects());
	pickKnapsack(sack, bitString);
}

string getcwd1()
{
	char* a_cwd = _getcwd(NULL, 0);
	string s_cwd(a_cwd);
	free(a_cwd);
	return s_cwd;
}

//map a number to a string of the number in binary form
string binaryString(int number, int bits) 
{
	string ret = "";
	for (int i = 0; i < bits; i++) {
		//get the current bit value, i.e. 2^x
		int holder = (pow(2, bits - i - 1));
		if (number >= holder) {
			ret += "1";
			number -= holder;
		}
		else {
			ret += "0";
		}
	}
	return ret;
}

//picks which items inn the knapsack to select based on the provided string
void pickKnapsack(knapsack& sack, string bits) {
	for (int i = 0; i < bits.length(); i++) {
		if ((bits.at(i) - '0') == 1) {
			sack.select(i);
		}
	}
	//cout << sack.getValue() << "\t" << sack.getCost() << endl;
}

//unslects all items in the knapsack
void clearKnapsack(knapsack& sack) {
	for (int i = 0; i < sack.getNumObjects(); i++) {
		sack.unSelect(i);
	}
}
