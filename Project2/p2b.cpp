// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>
#include <windows.h>
#include <direct.h>
#include <time.h>

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;


int const NONE = -1;  // Used to represent a node that does not exist

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;


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
   Graph::vertex_descriptor v;
   
   // Add nodes.
   for (int i = 0; i < n; i++)
      v = add_vertex(g);
   
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

//finds the lowest color number that the provided vertex can be without causing a confict
//note that isf no olor can be chose, it will return numColors + 1, which will be caught as a conflict
int lowestColorNumber(Graph& g, Graph::vertex_descriptor v, int numColors)
{
	int color = 1;
	bool colorFound = false;
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange = adjacent_vertices(v, g);
	while (color <= numColors && !colorFound)
	{
		for (Graph::adjacency_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
		{
			if(color == g[*vItr].color)
			{
				color++;
				break;
			}
			if (vItr == vItrRange.second - 1) {
				colorFound = true;
			}
		}
	}
	
	return color;
}

//count the number of conflicts in the graph
//conflicts are found by a node having a color of 0 or if any pairs of nodes that are neighbors have the same color
int countConflicts(Graph& g)
{
	//if  node is not colored, count a conflict
	//note that we never have a negative color or a color higher than the number of colors due to our design
	int numConflicts = 0;
	
	// track if neighbors have the same color
	//note that this will count 2 conflicts for each pair, so we divide by 2 at the end
	int neighborConflicts = 0;
	
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
   
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
    	//a conflict occurs if a graph has a color of 0 or has the same color as a neighbor
        if (0 == g[*vItr].color)
        {
        	numConflicts++;
		}
		
		if(matchingColors(g, vItr)) {
			neighborConflicts++;
		}
    }
    
    return numConflicts + (neighborConflicts / 2);
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

//returns a pair where the first element is the number of neighboring vertices for the vertex
//and the corresponding vertex is the second element
//also note that this will return a sorted vector increasingly by the number of neighbors - least amount of neighbors appears first
vector< pair<int, Graph::vertex_descriptor> > countNeighbors(Graph& g) {
	vector< pair<int, Graph::vertex_descriptor> > neighbors;
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	
	//loop over each vertex in the graph
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
		int numNeighbors = 0;
		pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange2 = adjacent_vertices(*vItr, g);
		
		//loop to count number of neighbors
		for (Graph::adjacency_iterator vItr2= vItrRange2.first; vItr2 != vItrRange2.second; ++vItr2) {
			numNeighbors++;
		}
		
		pair<int, Graph::vertex_descriptor> help;
		help.first = numNeighbors;
		help.second = *vItr;
		neighbors.push_back(help);
	}
	
	//deafults to sorting by the first element increasingly - least amount of neighbors appear first
	sort(neighbors.begin(), neighbors.end());
	return neighbors;
}


int greedyColoring(Graph& g, int numColors)
//greedy search of the minimum amount of color conflicts
{
	vector< pair<int, Graph::vertex_descriptor> > neighbors = countNeighbors(g);
	for (int i = 0; i < neighbors.size(); i++) {
		g[neighbors[i].second].color = lowestColorNumber(g, neighbors[i].second, numColors);
		cout<<neighbors[i].second<<"\t"<<g[neighbors[i].second].color<<endl;
	}
	return countConflicts(g, numColors);
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
      cout << "Reading graph" << endl;
      Graph g;
      numColors = initializeGraph(g,fin);
      setNodeColors(g, 0);
	  cout << "Num colors: " << numColors << endl;
      cout << "Num nodes: " << num_vertices(g) << endl;
      cout << "Num edges: " << num_edges(g) << endl;
      cout << endl;
      numConflicts = greedyColoring(g, numColors);
      cout << "best solution had " << numConflicts << " conflicts." << endl; 
      cout << g << endl;
      outputFile << "best solution had " << numConflicts << " conflicts." << endl; 
      outputFile << g << endl;
      outputFile.close();
   }
   catch (int e)
   {
	   cout << "Error occured: " << e << endl;
	   cin.get();
   }
}
