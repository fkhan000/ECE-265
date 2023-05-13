//Fakharyar Khan
//DSA II
//Professor Sable
//Project #3: Dijkstra's Algorithm

#include "heap.h"
#include <list>
//#include <iostream>
#include <fstream>
//#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
using namespace std::chrono;
using namespace std;

//Graph class
//representation of the graph
class Graph
{
public:
	//maps vertex ids to pointers to them
	hashTable* vertices;
	//used to get the unknown vertex with the smallest dv in O(log|V|) time
	//each node in the heap also stores a pointer to the vertex
	heap* unknown;
	//a list of the ids of the vectors
	list<string> nodes;
    
	Graph()
	{
		//simply creates the hashtable for vertices
		vertices = new hashTable();
	}

	class Vertex;
    
    list<Vertex> junction;
	//Edge Class
	//representation of the directed edge between two vertices
	//stores the weight of the edge and the vertex the edge points to
	class Edge
	{
	public:
		int weight;
		Vertex* destination;
		Edge(int cost, Vertex* to)
		{
			weight = cost;
			destination = to;
		}
	};

	//Vertex Class
	//stores the id, the shortest known distance from source vertex to it
	//the previous vector in that shortest known path
	//and a list of pointers to outgoing edges from it
	class Vertex
	{
	public:
		string id;
        int dv = INT_MAX;
        bool known = false;
        Vertex* prev = nullptr;
        list<Edge> adjacent;
		Vertex(string name)
		{
			id = name;
		}
	};

//insert function
//returns false if the user is attempting to make a loop
//or if the weights entered either aren't positive or are greater than 1 million
//returns true on success
//inserts vertex id and pointer into the hashtable if not already there
//adds a pointer to the newly formed outgoing edge into v1's adjaency list

bool insert(const string &id1, const string &id2, int weight)
{
	if(id1.compare(id2) == 0 or weight < 0 or weight > 1000000)
	{
		return false;
	}
	//if the vertex hasn't been seen before
	if (not (vertices -> contains(id1)))
	{
		//create a pointer to the new vertex
        Vertex vert1(id1);
        junction.push_back(vert1);
		//Vertex* vert1 = new Vertex(id1, INT_MAX,  nullptr, list<Edge*>());
		//insert it into the hashtable
		vertices -> insert(id1, &junction.back());
		//insert its id into nodes
		nodes.push_back(vert1.id);
	}

	if (not (vertices -> contains(id2)))
	{
        Vertex vert2(id2);
        junction.push_back(vert2);
        Vertex* ptr2 = &junction.back();
        
        
		//Vertex * vert2 = new Vertex(id2, INT_MAX,  nullptr, list<Edge*>());
        vertices -> insert(id2, &junction.back());
		nodes.push_back(vert2.id);
	}
    
	//update v1's adjaency list
    
	(((Vertex*)(vertices -> getPointer(id1))) -> adjacent).push_back(Edge(weight, ((Vertex*)(vertices -> getPointer(id2)))));
    
	return true;
}


//finds shortest path from source vertex
//to every vertex in graph
//returns false if vertex given isn't in the graph
//true upon success
bool Dijkstra(Vertex * source)
{
	if(not(vertices -> contains(source -> id)))
	{
		return false;
	}

	//initialize the heap
    unknown = new heap(nodes.size());
    
	//insert every vertex id into the heap and set their keys to INT_MAX
    
	for (auto const& i: nodes)
	{
		unknown -> insert(i, INT_MAX, vertices ->getPointer(i));
	}

	//set its dv to 0
	source -> dv = 0;
	//fix the heap
	unknown -> setKey(source -> id, 0);

	Vertex* chosen;
    int nWeight;
    int length = nodes.size();
    list<Edge> replace;
	for(int iterations = 0; iterations <  length; iterations ++)
	{
		//extract from heap unknown vertex with smallest dv
		unknown -> deleteMin(nullptr, nullptr, &chosen);
        
        chosen -> known = true;
		//if the min dv in this heap is INT_MAX
		//the rest of the unknown vertices don't have a path to the source vertex
		//so we're done and we can break
		if(chosen -> dv == INT_MAX)
		{
			//continue;
			break;
		}
		//go through its adjaency list and update values
		for (auto const& next : chosen -> adjacent)
		{
            if(next.destination -> known)
            {
                continue;
            }
            replace.push_back(next);
			//if chosen_dv + next -> weight < next_dv
			//update dv value on the destination vertex and in the heap
			//make prev of destination vertex chosen
			//else do nothing
            
            nWeight = chosen -> dv + next.weight;
			if(nWeight < next.destination -> dv)
			{
                next.destination -> dv = nWeight;
				next.destination -> prev = chosen;
				unknown -> setKey(next.destination -> id, nWeight);
			}
        
        }
        
        chosen -> adjacent = replace;
        replace.clear();
        
	}
    delete unknown;
	return true;
}

//gets shortest path from given vertex to source
string getPath(Vertex* source)
{
	//if we reached the source vertex
	if(source -> dv == 0)
	{
		//return its id
		return  source -> id + ", ";
	}
	//find the shortest path from the vertex before the given vertex in the path
	return  getPath(source -> prev) + source -> id + ", ";
}

//returns shortest path from source vertex to every vertex in graph
string printSolution()
{
	string output = "";
	Vertex* source;
	string path = "";

	//iterate through list of vertices
	for(auto const& start: nodes)
	{
		//get the pointer to the vertex from the hashtable
		source = (Vertex*)vertices -> getPointer(start);
		output += start + ": ";

		//if the dv is still INT_MAX, there was no path from the source to it
		if(source -> dv == INT_MAX)
		{
			path = "NO PATH\n";
		}
		else
		{
			output += to_string(source -> dv) + " ";
			path = "[" + getPath(source);
			path.erase(path.size() - 2);
			path += "]\n";
		}
		output += path;
	}
	return output;
}



};


void Processor()
{
	//variables that will store user input
	string input, output, vertex;

	//prompt user for graph file name
	cout << "Enter name of graph file: ";
	//store it in input
	cin >> input;

	//make graph object
	Graph* chart = new Graph();

	//open input file
	ifstream myfile;
	myfile.open(input);
	string line, arg;

	//vector of arguments to be fed into insert
	vector<string> arguments;
	while(getline(myfile, line))
	{
		stringstream ss(line);
		//parse each line by space to get arguments
		while(getline(ss, arg, ' ' ))
		{
			arguments.push_back(arg);
		}
		//insert into the graph
		chart -> insert(arguments[0], arguments[1], stoi(arguments[2]));
		arguments.clear();
	}
	myfile.close();

	while(true)
	{
		//prompt user for start vertex
		cout<< "Enter name of starting vertex: ";
		cin >>  vertex;
		//if the vertex isn't in the graph
		//keep on prompting them until they give a valid one
		if(chart -> vertices -> contains(vertex))
		{
			break;
		}
	}

	//get the start vertex from the hashtable
	Graph::Vertex * source =  (Graph::Vertex *)(chart -> vertices -> getPointer(vertex));

	//time Dijkstra's algorithm
	auto start = high_resolution_clock::now();
	chart -> Dijkstra(source);
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<milliseconds>(stop - start);

	cout << "Total time (in seconds) to apply Dijkstra's algorithm: "
         << float(duration.count())/1000<< endl;
    
    //prompt user for output file
	cout << "Enter name of output file: ";
	cin >> output;
    
    string result = chart -> printSolution();
	//open output file
    delete chart;
	ofstream outfile;
	outfile.open(output);
	
	//write the results of the algorithm to the file
	outfile << result;
	outfile.close();

}

//just calls on Processor
int main()
{
	Processor();

	return 0;
}
