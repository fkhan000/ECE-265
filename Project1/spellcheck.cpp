#include "hash.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace std::chrono;


//main processing function
//builds the hashtable using the dictionary given
//goes through input file and performs spell check
//outputting results to the output file
//takes in filename containing dictionary, input filename, and the pointer to the output file
void Processor(std::string dictionary, std::string input, FILE* fp)
{
	//creates new hashTable of initial size 1000
	//start the timer
	auto start = high_resolution_clock::now();

	hashTable *b = new hashTable(1000);

	//open the dictionary file
	ifstream myfile;
	myfile.open(dictionary);
	std::string word;
	int count = 0;
	//use getline to parse through file line by line
	while(getline (myfile, word))
	{
		//change the casing of the word to lowercase
		transform(word.begin(), word.end(), word.begin(), ::tolower);
		//insert it into the hashtable
		b -> insert(word);
	}
	//stop the timer
	//measure time took to load in the dictionary
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	cout << "Time taken to load in dictionary "
         << duration.count() << " microseconds" << endl;
	
	//close the file
	myfile.close();

	//start timer
	start = high_resolution_clock::now();
	//set line number of input file count
	int linenum = 1;

	std::string line;
	//open the input file
	myfile.open(input);
	//a string containing all the valid characters
	std::string const valid{ "1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM-'"};

	//parse through the input file line by line and feed it into the line variable
	while(std::getline(myfile, line))
	{
		//make a string stream from the line
		std::stringstream stream(line);
		//set up variables necessary to parse through the line by the multple delimiters
		//start stores the index first letter in line that isn't a special character 
		//pos stores the index of the first special character after start
		size_t start, pos = 0;
		//set start equal to the first non-special character after a certain index in the line, pos
		//exit once you reach end of line
		while((start = line.find_first_of(valid, pos)) != std::string::npos)
		{

			//find the first instance of a special character in the string after start
			pos = line.find_first_not_of(valid, start + 1);
			//the word is then whatever was between those two special characters
			word = line.substr(start, pos - start);
			//set the word to lowercase
			transform(word.begin(), word.end(), word.begin(), ::tolower);

			//check if word has a digit in it
			//if it does, skip it
			if(std::any_of(word.begin(), word.end(), ::isdigit))
			{
				continue;
			}
			////check if length of word is greater than 20
			if (word.length() > 20)
			{
				//if so write to file the first 20 characters of the word and the line it's on in the file
				fprintf(fp, "Long word at line %d, starts: %s", linenum, (word.substr(0, 20)).c_str());
				//if the end of file hasn't been reached
				//add a new line character to the file
				if (! myfile.eof())
				{
					fprintf(fp, "\n");
				}
			}
			//else if the word isn't in the hashtable
			else if (not (b -> contains(word)))
			{
				//print to the file the unknown word and the line it's on in the file
				fprintf(fp, "Unknown word at line %d: %s", linenum, word.c_str());

				//again if the end of file hasn't been reached, add a new line character to the file
				if (! myfile.eof())
				{
					fprintf(fp, "\n");
				}
			}
		}
		//once a line has been fully spell checked
		//increment the line number by one 
		linenum += 1;
	}

	//stop timer and measure how long it took to spellcheck
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken to spell check document "
         << duration.count() << " microseconds" << endl;
	//close the input file
	myfile.close();

}

//main driver function
//interacts with user to get file names
//then invokes Processor function
//prints out time taken by function in seconds
int main()
{
	//variables that will store user input
	char input[256], output[256], dictionary[256];


	//user prompting
	printf("Please enter the name of the dictionary file: ");
	scanf("%s", dictionary);
	printf("Please enter the name of the input file: ");
	//scan it in
	scanf("%s", input);
	//promp the user for the name of the output file
	printf("Please enter the name of the output file: ");
	//scan it in
	scanf("%s", output);

	//convert character array to c ++ string
	std::string diction(dictionary);
	std::string in(input);

	//creater file pointer for the output file
	FILE * fp;
	fp = fopen(output, "w");

	//set up the clock to time the Processor function
	auto start = high_resolution_clock::now();
	//give in the parameters
	Processor(diction, in, fp);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
  
  	//print out the time
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
     //close the output file
     fclose(fp);
	return 0;
}

