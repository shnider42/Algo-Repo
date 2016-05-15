// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>
#include <windows.h>
#include <direct.h>

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;

string getcwd1();


int const NONE = -1;  // Used to represent a node that does not exist

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

int exhaustiveColoring(Graph&, int, int);

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

int main()
{
   char x;
   ifstream fin;
   string fileName;
   int numColors;
   
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

	  cout << "Num colors: " << numColors << endl;
      cout << "Num nodes: " << num_vertices(g) << endl;
      cout << "Num edges: " << num_edges(g) << endl;
      cout << endl;
      
      // cout << g;
   }
   catch (int e)
   {
	   cout << "Error occured: " << e << endl;
	   cin.get();
   }
   cin.get();
}

string getcwd1()
{
	char* a_cwd = _getcwd(NULL, 0);
	string s_cwd(a_cwd);
	free(a_cwd);
	return s_cwd;
}
int exhaustiveColoring(Graph& g, int numColors, int t)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr){
		
	}
	return 0;
}
