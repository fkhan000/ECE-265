
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

bool graph[1001][1001];


// determines whether or not
// C is a valid merge of A and B
// does this by solving subproblems of the main problem
// checks if the first i + j  characters of C
// is a valid merge of the first i, j characters of A and B respectively
// which in turn can be solved by looking at its subproblems
// we can denote these subproblems as (i,j) and represent their
// answers in a 2D boolean array 
bool isMerge(string A, string B, string C)
{
	//retrieve size of strings A and B
	int M = A.size();
	int N = B.size();

	//if the size of C is not M + N
	//C definitely can't be a valid merge
	if(M + N != C.size())
	{
		return false;
	}

	for (int i = 0; i < M + 1; i++) 
	{
		for(int j = 0; j < N + 1; j++) 
		{
			//this is done to clear out the array from previous calls
			//to isMerge
			graph[i][j] = false;

			// an empty string is a valid merge of two empty strings
			if (i == 0 and j == 0) { graph[i][j] = true; }

			//if A' is empty
			else if(i == 0) 
			{
				//in order to be a valid merge, B[j-1] == C[j-1]
				if (B[j-1] == C[j-1]) 
				{
					//and if so, the problem reduces to the (i, j-1) subproblem
					graph[i][j] = graph[i][j-1];
				}
			}

			//same idea if B' is empty
			else if(j == 0)
			{
				if (A[i-1] == C[i-1]) { graph[i][j] = graph[i-1][j];}
			}

			// in order to be a valid merge, C[i + j - 1] = A[i-1] or B[i-1]

			//if former case,
			else if (A[i - 1] == C[i + j - 1] and B[j - 1] != C[i + j - 1])
			{
				//we now have to look at the (i-1, j) subproblem
				graph[i][j] = graph[i-1][j];
			}

			//in the latter case,
			else if (A[i - 1] != C[i + j - 1] and B[j - 1] == C[i + j - 1])
			{
				//look at the (i, j-1) subproblem
				graph[i][j] = graph[i][j-1];
			}

			//if both cases are satisfied
			else if (A[i - 1] == C[i + j - 1] and B[j - 1] == C[i + j - 1])
			{
				//we have a choice and the (i,j) subproblem
				//we can either have C[i + j - 1] be a "B" letter or an "A" letter
				graph[i][j] = (graph[i - 1][j] or graph[i][j - 1]);
			}
		}
	}
	return graph[M][N];
}

// assuming the existence of such a path
// the function finds the valid merge of A and B
// in C such that the letters of A occur as soon as possible in C
// and returns C with the letters of A in that merge capitalized
// does this by looking at the array and retraces the steps
// to find out which letters are "A" letters

string findPath(string A, string B)
{
	string output = "";
	//get the size of A and B
	int M = A.size();
	int N = B.size();
	//index variables
	//starts from the bottom, right corner so that
	//it's guaranteed to find a path to the top, left corner
	int i = M;
	int j = N;

	//capitalize the letters of A
	for(int k = 0; k < M; k++)
	{
		A[k] = toupper(A[k]);
	}
	//starts from
	while(j > 0)
	{
		//if graph[i][j-1] is true
		//C[i+j-1] was a "B" letter
		//because of the ordering of the if statements
		//if the function reaches a case where above and to the left of it
		//are both true, it will prioritize going to the left first
		//meaning the B letters will get finished up as soon as possible
		if(graph[i][j-1])
		{
			output = B[j - 1] + output;
			//move one to the left in the matrix
			j -= 1;
		}
		else
		{
			//else, because we're guaranteed that a path exists
			//the slot above us must be true
			//which implies that C[i+j-1] is an A letter
			output = A[i-1]+ output;
			//move one up in the matrix
			i -= 1;
		}
	}
	//since we tried to get to the left wall of the matrix
	//as fast as possible, it's possible that we hit the wall
	//but haven't reached the corner yet
	while(i > 0)
	{
		//if so, we're guaranteed that C[i+j-1] are A letters
		output = A[i-1] + output;
		i -= 1;
	}
	return output;
}


int main()
{
	string in;
	string out;
	cout << "Enter the name of input file: ";
	cin >> in;
	cout << "Enter the name of output file: ";
	cin >> out;

	ifstream myfile;
	myfile.open(in);

	ofstream outfile;
	outfile.open(out);
	string line;
	vector<string> arguments;
	int count = 0;

	while(getline(myfile, line))
	{
		//fill the vector with the arguments
		arguments.push_back(line);
		count ++;
		if(count == 3)
		{
			//once three arguments have been given, reset counter
			count = 0;
			//if it's a valid merge
			if(isMerge(arguments[0], arguments[1], arguments[2]))
			{
				//write out one possible valiid merge
				outfile << findPath(arguments[0], arguments[1]) << "\n";
			}
			else
			{
				//else indicate that it's not a valid merge
				outfile << "*** NOT A MERGE ***" << "\n";
			}
			arguments.clear();
		}
	}
	myfile.close();
	outfile.close();
	return 0;
}

