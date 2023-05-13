#include "heap.h"

//constructor for heap object
//capacity: measure of how much space in heap is left
//size: measure of how many elements (nodes) in heap
//map is a hashTable that allows for constant time access to pointers to nodes from their ids
//creates a hashTable
heap::heap(int length)
{
	data.resize(length + 1);
	capacity = length;
	size = 0;
	map = new hashTable(2*capacity);
}

//constructor for node object
//has an id which is used by the hashTable
//a key which is used to determine its position in the heap
heap::node::node(std::string identi, int pass, void* pvData)
{
	id = identi;
	key = pass;
	pData = pvData;
}

int heap::insert(const std::string &id, int key, void *pv)
{
	//if the heap is almost full, return 1
	if (size == capacity)
	{
		return 1;
	}
	//if the id is already in use, return 2
	if(map -> contains(id))
	{
		return 2;
	}
	int index = size + 1;
	node element(id, key, pv);
	data[index] = element;
	map -> insert(id, &data[index]);

	percolateUp(index);
	size += 1;
	return 0;
}

void heap::percolateDown(int posCur)
{
	int index = posCur;
	int child;
	node element = data[index];

	while(index*2 <= size)
	{
		child = index *2;
		if(child != size and data[child + 1].key < (data[child].key))
    	{
    		child += 1;
    	}

    	if(data[child].key < element.key)
    	{
    		data[index] = data[child];
    		map -> setPointer(data[index].id, &data[index]);
    		index = child;
    	}
    	else
    	{
    		break;
    	}
	}
	data[index] = element;
	map -> setPointer(data[index].id, &data[index]);
}


void heap::percolateUp(int posCur)
{
	int index = posCur;
	node element = data[index];

	while(index > 1 and element.key < data[index /2].key)
	{
		data[index] = data[index / 2];
		map -> setPointer(data[index].id, &data[index]);
        index /= 2;
	}
	data[index] = element;
	map -> setPointer(data[index].id, &data[index]);
}

int heap::setKey(const std::string &id, int key)
{
	if (not map -> contains(id))
	{
		return 1;
	}

	node *element = ((node*)(map -> getPointer(id)));
	int index = getPos(element);
	data[index].key = key;
	percolateUp(index);
    if(index == getPos(element))
    {
        percolateDown(index);
    }

	return 0;
}


int heap::remove(const std::string &id, int *pKey, void *ppData)
{
	if(size == 0 or not(map -> contains(id)))
	{
		return 1;
	}
	node * pv = ((node*)(map -> getPointer(id)));
	int index = getPos(pv);
	if(pKey != nullptr)
	{
		*pKey = data[index].key;
	}
	if(ppData != nullptr)
	{
		*(static_cast<void **> (ppData)) = data[index].pData;
	}
	setKey(id, -INT_MAX);
	deletion(id);
	return 0;
}

int heap::deleteMin(std::string *pId, int *pKey, void *ppData)
{
	if(size == 0)
	{
		return 1;
	}
	if(pId != nullptr)
	{
		*pId = data[1].id;
	}
	if(pKey != nullptr)
	{
		*pKey = data[1].key;
	}
	if(ppData != nullptr)
	{
		*(static_cast<void **> (ppData)) = data[1].pData;
	}
	deletion(data[1].id);

	return 0;
}

void heap::deletion(std::string id)
{
	node * pv = ((node*)(map -> getPointer(id)));
	int index = getPos(pv);
	node removed = data[index];
	data[index] = data[size];

	//data[size] = removed;
	map -> remove(removed.id);
	size -= 1;
	percolateDown(index);
	percolateUp(index);

}



