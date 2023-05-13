#include "hash.h"
#include <typeinfo>

//returns a prime number depending on size required for hash table
//supports a size of up to one million
unsigned int hashTable::getPrime(int size)
{
	if (size <= 10000) { return 12373;}
	if (size <= 20000) {return 25867;}
	if (size <= 50000) {return 51479;}
	if (size <= 100000) {return 109211;}
	if (size <= 200000) {return 202717;}
	if (size <= 400000) {return 410299;}
	if (size <= 800000) {return 803549;}
	if (size <= 1600000) {return 2001101;}
	return 3200983;
}

// constructor for hashTable class
//takes initial size input and makes size of table from getPrime
//sets capacity to be initially the size of the hash table
hashTable::hashTable(int size)
{
	int num = getPrime(size);
	length = num;
	data.resize(num);
	capacity = length;
	filled = 0;
}



//hash function
//uses linear probing in the event of a collision
//which is where the function goes from the index
//where the initial hash sent the string to and 
//goes down the table until it either finds a free space
//or it finds the hashitem with that key value
int hashTable::hash(const std::string &key)
{
	int hashVal = 0;
	for(char ch: key)
	{
		hashVal = 37*hashVal + ch;
	}
	int i = 0;

	if (data[hashVal % length].isOccupied)
	{
		while( data[(hashVal + i) % length].isOccupied and 
			(data[(hashVal + i)% length].key).compare(key) != 0)
		{
			i += 1;
		}
	}
	return (hashVal + i) % length;
}

//insert function inserts key into position given by hash function
//changes filled and capacity number
//returns 0 on success, 1 when the key has already been inserted
//and 2 if there was a memory allocation error in the rehash function
//if the loading size exceeds 0.5, it rehashes
//this means that at any given time, the hashtable only uses a 1/3 of its actual size
//which is necessary in order to decrease the likelihood of collisions
int hashTable::insert(const std::string &key, void *pv)
{
	//if the key has already been inserted return 1
	//but if the hash item has been deleted, we could insert this key in its place
	//i didn't put this before, this could be what was missing from my code
	int index = hash(key);
	if (((data[index].key).compare(key) == 0) && not(data[index].isDeleted))
	{
		return 1;
	}
    
    if(data[index].isDeleted)
    {
        filled -= 1;
        capacity += 1;
    }
    
    data[index] = hashItem();
    data[index].isOccupied = true;
    data[index].isDeleted = false;
    data[index].key = key;
    data[index].pv = pv;
    filled += 1;
    capacity -= 1;
    
	if (filled/capacity > 0.5)
	{
		if (not rehash())
		{
			return 2;
		}
	}
	return 0;
}

//multiplies the size of the table by 2
//resizes it based on getPrime of new size
//uses a try catch block on the portion where
//the table is resized
//returns false if memory allocation error occurs
//true if successful
bool hashTable::rehash()
{
	int m = data.size();

	std::vector<hashItem> temp(data);
	//make temporary vector to contain key values
	//temp.resize(m);
	//temp = data;
	//get new size of hashtable
	int size = getPrime(m*2);
	//clear the contents of the hashtable
	data.clear();
	//try to double the size of the hashtable
	try
	{
		data.resize(size);
	}
	//catch any potential memory allocation error
	catch (std::bad_alloc)
	{
		return false;
	}
	//set capacity to size and filled to 0
	capacity = size;
	filled = 0;
	length = size;
	//insert the key into the resized and empty hashtable
	for(int i= 0; i < m; i++)
	{
		//only add in items that weren't deleted
		if(not temp[i].isDeleted && temp[i].isOccupied)
		{
			insert(temp[i].key);
			setPointer(temp[i].key, temp[i].pv);
			filled += 1;
			capacity -= 1;
		}
	}
	//deallocate memory from the temporary vector
	//std::vector<hashItem>().swap(temp);
	return true;
}

//checks if key is in hash table
bool hashTable::contains(const std::string &key)
{
	int index  = hash(key);
	return (data[index]).isOccupied and not((data[index]).isDeleted);
}


//sets pointer of hashItem associated with the key
//to input
int hashTable::setPointer(const std::string &key, void *pv)
{
    int index = hash(key);
    if(not ((data[index]).isOccupied and not((data[index]).isDeleted)))
    {
        return 1;
    }
    data[index].pv = pv;
    return 0;
}

//retrieves pointer in hashItem from hashtable
//also writes to a boolean pointer whether or not key is in the table
//returns nullptr if not in the table
void* hashTable::getPointer(const std::string &key, bool *b)
{
    int index = hash(key);
    bool con = data[index].isOccupied and not data[index].isDeleted;
    	//bug
	b = &con;
	if(not con)
	{
		return nullptr;
	}
	return data[index].pv;
}



//removes an item from the hashtable
 //via lazy deletion
bool hashTable::remove(const std::string &key)
{
    int index = hash(key);
	if(not ((data[index]).isOccupied and not((data[index]).isDeleted)))
	{
		return false;
	}
	data[index].isDeleted = true;
	return true;
}





