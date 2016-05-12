
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

int main()
{
   char x;
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


		// Put main code here

		// Update current weight in sack every iteration of while loop
		int itemsSelected = 0;
		int currentWeight = 0;
		for (int i = 0; i < sack.getNumObjects(); i++)
		{
			itemsSelected++;
			currentWeight = sack.getCost(i) + currentWeight;
		}

		// Create random index to put into sack
		srand(time(NULL));
		int randNum = rand() % sack.getNumObjects();
		// if item isn't selected, and there is enough space, then select item
		if ((sack.isSelected(randNum) == false) && sack.getCost(randNum) + currentWeight <= sack.getCostLimit())
		{
			sack.select(randNum);
		}
		// Need an exit case where there is no more items that can fit in the sack
		else if (sack.getCost(randNum) + currentWeight > sack.getCostLimit())
		{
			// need to record sack weight and value and items when this happens (and there are 
			// no more items that can legally be added)
		}
		

	}
}