//Project 1b: using exhaustive search to color a graph.
// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>
#include <windows.h>
#include <direct.h>
#include <time.h>
#include <string> 
#include <math.h>

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;


int const NONE = -1;  // Used to represent a node that does not exist

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

string getcwd1();
int exhaustiveColoring(Graph&, int, int);
void colorGraph(Graph&, vector<int>, int);
int countConflicts(Graph&);
vector<int> incrementColorNumber(vector<int>, int, int);
bool matchingColors(Graph& g, Graph::vertex_iterator v);

//Create a struct to hold the properties of each vertex
struct VertexProperties
{
   pair<int,int> cell; // maze cell (x,y) value
   Graph::vertex_descriptor pred;
   bool visited;
   bool marked;
   int weight;
   int color;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
   int weight;
   bool visited;
   bool marked;
};

int initializeGraph(Graph &g, ifstream &fin)
// Initialize g using data from fin.  
{
   int n, e;
   int j,k;
   int numColors;

   fin >> numColors;
   fin >> n >> e;
   
   // Add nodes.
   for (int i = 0; i < n; i++)
      add_vertex(g);
   
   for (int i = 0; i < e; i++)
   {
      fin >> j >> k;
      add_edge(j,k,g);  // Assumes vertex list is type vecS
      add_edge(k,j,g);
   }
   
   return numColors;
}

void setNodeWeights(Graph &g, int w)
// Set all node weights to w.
{
   pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
   
   for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
   {
      g[*vItr].weight = w;
   }
}

void setNodeColors(Graph &g, int w)
// Set all node colors to w.
{
   pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
   
   for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
   {
      g[*vItr].color = w;
   }
}

ostream &operator<<(ostream &ostr, const Graph &g)
// Output operator for the Graph class. Prints out all nodes and their colors
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
   
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        ostr << *vItr << "\t" << g[*vItr].color << endl;
    }
    return ostr;
}

int main()
{
   char x;
   ifstream fin;
   string fileName;
   ofstream outputFile;
   int numColors, numConflicts;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.

   /*string CWD;
   CWD = getcwd1();
   cout << CWD;
   cout << "\n\n";

   // appending file name to load correctly
   fileName = CWD + "\\\color12-4.input";
   cout << fileName;
   cin.get();*/
   
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
      cout << "Reading graph" << endl;
      Graph g;
      numColors = initializeGraph(g,fin);
      setNodeColors(g, 0);
	  cout << "Num colors: " << numColors << endl;
      cout << "Num nodes: " << num_vertices(g) << endl;
      cout << "Num edges: " << num_edges(g) << endl;
      cout << endl;
      
      //get the fewest number of conflicts in the time allowed
      //note that third parameter is time to search in seconds
      numConflicts = exhaustiveColoring(g, numColors, 600);
      outputFile << "best solution had " << numConflicts << " conflicts." << endl; 
      outputFile << g << endl;
      outputFile.close();
      // cout << g;
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
}

//get the current working directory to allow for smoother filename finding
string getcwd1()
{
	char* a_cwd = _getcwd(NULL, 0);
	string s_cwd(a_cwd);
	free(a_cwd);
	return s_cwd;
}

int exhaustiveColoring(Graph& g, int numColors, int t)
//brute force search of the minimum amount of color conflicts
{
	//code for coloring a graph
	vector<int> colorNumber;
	for (int i = 0; i < num_vertices(g); i++) {
		colorNumber.push_back(0);
	}
	// the final number is when all nodes are colored the "highest" colornumber
	//i.e. if we have 3 colors, the final vector will be made of all 2's
	vector<int> finalNumber;
	for (int i = 0; i < num_vertices(g); i++) {
		finalNumber.push_back(numColors - 1);
	}
	
	//this boolean will track when we have exhausted all possibilities of colorings
	//when false, there are more colorings to check
	bool done = false;
	
	//initialize the best number of conflicts to max and best graph to the initial, non-colored graph
	int bestConflicts = num_vertices(g);
	Graph bestGraph = g;
	
	// Get start time to be used in while loop
	time_t startTime;
	time(&startTime);
	
	while (!done) {
		time_t newTime;
		time(&newTime);
		
		//if we have reached the tim threshold
		if (difftime(newTime, startTime) > t)
		{
			cout << "Time limit expired" << endl;;
			break;
		}
		
		//if we have reached the last coloring, execute one more time, then stop
		if (colorNumber == finalNumber || 0 == bestConflicts)
		{
			done = true;
		}
		
		colorGraph(g, colorNumber, numColors);
		
		//if we have a better solution
		if (countConflicts(g) < bestConflicts)
		{
			bestConflicts = countConflicts(g);
			bestGraph = g;
						
			//if we have reached the best possible solution of any graph, stop now
			if (0 == bestConflicts)
			{
				break;
			}
		}
		
		//get the next coloring code
		colorNumber = incrementColorNumber(colorNumber, colorNumber.size() - 1, numColors);
	}
	g = bestGraph;
	return bestConflicts;
}

//count the number of conflicts in the graph
//conflicts are found by a node having a color of 0 or if any pairs of nodes that are neighbors have the same color
int countConflicts(Graph& g)
{
	int numConflicts = 0;
	
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
   
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
    	//a conflict occurs if a graph has a color of 0 or has the same color as a neighbor
        if (0 == g[*vItr].color || matchingColors(g, vItr))
        {
        	numConflicts++;
		}
    }
    
    return numConflicts;
}

//checks if a vertex has the same color as any of its neighbors
bool matchingColors(Graph& g, Graph::vertex_iterator v)
{
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange = adjacent_vertices(*v, g);
	for (Graph::adjacency_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		if(g[*v].color == g[*vItr].color)
		{
			return true;
		}
	}
	
	//we have looped through all of the neighbors, so none have the same color
	return false;
	
}

//colors the graph - the second parameter is the "code" for how to color the graph
//the code is a vector of ints, where the int represents the color - 1
//i.e., if the code for 3 vertices is 201, then they will be colored 3, 1, and 2 respectively 
void colorGraph(Graph& g, vector<int> colorNumber, int numColors)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	int i = 0;
   
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        g[*vItr].color = colorNumber.at(i) + 1;
        i++;
    }
}

//this increments the color code
//i.e. 000 becomes 001, assuming 3 colors 202 becomes 210, etc
vector<int> incrementColorNumber(vector<int> colorNumber, int index, int numColors) 
{
	colorNumber.at(index) += 1;
	if (colorNumber.at(index) >= numColors && index > 0)
	{
		colorNumber.at(index) = 0;
		colorNumber = incrementColorNumber(colorNumber, index - 1, numColors);
	}
	return colorNumber;
}
