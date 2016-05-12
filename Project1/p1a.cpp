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

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "knapsack.h"

void exhaustiveKnapsack(knapsack, int);
string booleanString(int, int);
void pickKnapsack(knapsack, string);
void clearKnapsack(knapsack);

int main()
{
   //char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.
   
   fileName = "knapsack8.input";

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

      exhaustiveKnapsack(k, 6);

      cout << endl << "Best solution" << endl;
      k.printSolution();
   }    
   catch (indexRangeError &ex) 
   { 
      cout << ex.what() << endl; exit(1);
   }
   catch (rangeError &ex)
   {
      cout << ex.what() << endl; exit(1);
   }
	
   cin.get(); // pause to read output
}

void exhaustiveKnapsack(knapsack sack, int timeLimit)
{
	// Get start time to be used in while loop
	time_t startTime;
	time(&startTime);

	int currentNumber = 1;
	int bestValue = 0;
	int bestNumber = 0;
	string bitString = "";
	// a while loop that expires when time limit is complete by checking difference of 
	// start time and current time at every loop
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

		bitString = booleanString(currentNumber, sack.getNumObjects());
		pickKnapsack(sack, bitString);
		if (sack.getValue() > bestValue) {
			bestValue = sack.getValue();
			bestNumber = currentNumber;
		}
		clearKnapsack(sack);
		currentNumber++;
	}
	bitString = booleanString(bestNumber, sack.getNumObjects());
	pickKnapsack(sack, bitString);
}

string booleanString(int number, int bits) 
{
	string ret = "";
	for (int i = 0; i < bits; i++) {
		int holder = (pow(2, bits - i));
		if (number > holder) {
			ret += "1";
			number -= holder;
		}
		else {
			ret += "0";
		}
	}
	return ret;
}

void pickKnapsack(knapsack sack, string bits) {
	for (int i = 0; i < bits.length(); i++) {
		if ((bits.at(i) - '0') == 1) {
			sack.select(i + 1);
		}
	}
}

void clearKnapsack(knapsack sack) {
	for (int i = 1; i <= sack.getNumObjects(); i++) {
		sack.unSelect(i);
	}
}