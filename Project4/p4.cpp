// Project 1a: Solving knapsack using exhaustive search
// Authors: Matt Bresett and Chris Holtsnider

//Credit for design attributed to EECE4452 in-class discussion on May 16, 2016 

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

void branchAndBound(knapsack& k, int timeLimit) 
{
	// Get start time to be used in while loop
	time_t startTime;
	time(&startTime);
	
	deque<knapsack> subproblems;
	
	subproblems.push_front(k);
	
	//initialize our best solution to an empty knapsack
	knapsack bestSolution = k;
	
	//pair with double representing 
	pair<double, bool> currentBound, sub1Bound, sub2Bound, bestBound;
	bestBound = bestSolution.bound();

	// a while loop that expires when time limit is complete by checking difference of 
	// start time and current time at every loop
	while (!subproblems.empty())
	{
		// Get current time and stop when greater than input time limit
		time_t newTime;
		time(&newTime);
		
		//if the time threshold has been reached
		if (difftime(newTime, startTime) > timeLimit)
		{
			cout << "Time limit expired" << endl;
			break;
		}
		
		//get the front problem - note that this means we use a DFS approach
		knapsack currentProblem = subproblems.front();
		subproblems.pop_front();
		
		
		
		//get the current bound and best bound
		currentBound = currentProblem.bound();
		
		/*check if the current problem is fathomed
		knapsack is not over the weight limit
		either the best solution is not integral, or if it is, this bound is better than the solution's value
		and we have not exhausted all of the items in the backpack */
		if(currentProblem.getCost() < currentProblem.getCostLimit() && ((bestBound.second && currentBound.first > bestSolution.getValue()) || !bestBound.second) && currentProblem.getNum() < currentProblem.getNumObjects()) {
			
			//subproblem 1 represents taking the current index item
			knapsack subproblem1 = currentProblem;
			subproblem1.select(subproblem1.getNum());
			
			//subprolem 2 represents not taking the current index item
			knapsack subproblem2 = currentProblem;
			
			
			//calculate our bounds
			//note that bounds return values, but reset the backpack to the same state as before the calculation
			sub1Bound = subproblem1.bound();
			sub2Bound = subproblem2.bound();
			
			//if subproblem 2 is not fathomed, add it to our list
			//note we do not need to check if this is an integral solution because it is the same as the current solution, just an incremented num index
			if (subproblem2.getCost() < subproblem2.getCostLimit() && ((bestBound.second && sub2Bound.first > bestSolution.getValue()) || !bestBound.second)) {
				subproblems.push_front(subproblem2);
				subproblems[0].setNum(subproblems[0].getNum()+1);
			}
			if (sub1Bound.second) {
				if (subproblem1.getValue() > bestSolution.getValue()) {
					bestSolution = subproblem1;
					bestBound = bestSolution.bound();
				}
			}
			else {
				if (subproblem1.getCost() < subproblem1.getCostLimit() && ((bestBound.second && sub1Bound.first > bestSolution.getValue()) || !bestBound.second)) {
					subproblems.push_front(subproblem1);
					subproblems[0].setNum(subproblems[0].getNum()+1);
				}
			}
		}
	}
	k = bestSolution;
}


int main()
{
   char x;
   ifstream fin;
   stack <int> moves;
   string fileName;
   ofstream outputFile;

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
	  branchAndBound(k, 600);
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

