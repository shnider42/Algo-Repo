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
string binaryString(int, int);
void pickKnapsack(knapsack&, string);
void clearKnapsack(knapsack&);

void outputSolutionToFile(ofstream& fout, knapsack k) 
{
   fout << "------------------------------------------------" << endl;

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
//note that third parameter is max search time in seconds
void exhaustiveKnapsack(knapsack& sack, int timeLimit)
{
	// Get start time to be used in while loop
	time_t startTime;
	time(&startTime);

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

//get the current working directory to allow for smoother filename finding
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
