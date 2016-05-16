// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>
#include <windows.h>
#include <direct.h>
#include <time.h>
#include <string> 

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
void colorGraph(Graph&, int, int);
string colorString(int, int, int);
int countConflicts(Graph&);
void printGraph(Graph&);
bool matchingColors(Graph& g, Graph::vertex_iterator v);

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

int main()
{
   char x;
   ifstream fin;
   string fileName;
   int numColors, numConflicts;
   
   // Read the name of the graph from the keyboard or
   // hard code it here for testing.

   string CWD;
   CWD = getcwd1();
   cout << CWD;
   cout << "\n\n";

   // appending file name to load correctly
   fileName = CWD + "\\\color12-3.input";
   cout << fileName;
   cin.get();
   
   //   cout << "Enter filename" << endl;
   //   cin >> fileName;
   
   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
	  cin.get();
      exit(1);
   }
   
   try
    {
      cout << "Reading graph" << endl;
      Graph g;
      numColors = initializeGraph(g,fin);
      setNodeColors(g, 0);
	  cout << "Num colors: " << numColors << endl;
      cout << "Num nodes: " << num_vertices(g) << endl;
      cout << "Num edges: " << num_edges(g) << endl;
      cout << endl;
      numConflicts = exhaustiveColoring(g, numColors, 6);
      cout << "best solution had " << numConflicts << " conflicts." << endl; 
      printGraph(g);
      // cout << g;
   }
   catch (int e)
   {
	   cout << "Error occured: " << e << endl;
	   cin.get();
   }
}

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
	int colorNumber = 0;
	int bestConflicts = num_vertices(g);
	Graph bestGraph = g;
	// Get start time to be used in while loop
	time_t startTime;
	time(&startTime);
	
	while (true) {
		time_t newTime;
		time(&newTime);
		if (difftime(newTime, startTime) > t)
		{
			cout << "Time limit expired" << endl;;
			break;
		}
		
		if (colorNumber == pow(4, num_vertices(g)))
		{
			break;
		}
		
		colorGraph(g, colorNumber, numColors);
		
		if (countConflicts(g) < bestConflicts)
		{
			bestConflicts = countConflicts(g);
			if (0 == bestConflicts)
			{
				break;
			}
			bestGraph = g;
		}
		colorNumber++;
	}
	g = bestGraph;
	return bestConflicts;
}

int countConflicts(Graph& g)
{
	int numConflicts = 0;
	
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
   
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        if (0 == g[*vItr].color || matchingColors(g, vItr))
        {
        	numConflicts++;
		}
    }
    
    return numConflicts;
}

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
	return false;
	
}


void printGraph(Graph& g) {
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
   
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        cout << *vItr << "\t" << g[*vItr].color << endl;
    }
}

void colorGraph(Graph& g, int colorNumber, int numColors)
{
	string pickedGraph = colorString(num_vertices(g), colorNumber, numColors);
	cout <<colorNumber<<"     "<< pickedGraph<<endl;;
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	int i = 0;
   
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
    {
        g[*vItr].color = (pickedGraph.at(i) - '0') + 1;
        cout<<g[*vItr].color;
    }
    cout<<endl<<endl<<endl;
}

string colorString(int numVertices, int colorNumber, int numColors)
{
	string ret = "";
	for (int i = 0; i < numVertices; i++) {
		ostringstream convert;
		int holder = (pow(numColors, numVertices - i - 1));
		convert << colorNumber / holder;
		ret += convert.str();
		colorNumber -= (colorNumber / holder) * holder;
	}
	return ret;
}
