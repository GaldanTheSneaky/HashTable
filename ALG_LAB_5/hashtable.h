#pragma once
#include<iostream>
#include<vector>
#include<functional>


template<typename data_type, typename key_type>
class HashTable
{
private:
	std::vector<data_type> table;
	std::vector<bool> isOccupiedTable;
	std::vector<bool> wasDeletedTable;
	std::vector<int> attemptsTable;
	std::function<key_type(data_type)> keyReceivingFunction;
	std::function<int(key_type)> hashFunction;
	key_type keyReceivingFunctionDefault(data_type data);

public:
	HashTable();
	HashTable(int size, 
		std::function<key_type(data_type)> keyReceivingFunction = nullptr,
		std::function<int(key_type)> hashFunction = nullptr);

	~HashTable();
	bool insert(data_type data);
	int search(key_type searchKey);
	data_type get(key_type searchKey);
	void remove(key_type searchKey);
	void print();
};

template<typename data_type, typename key_type>
HashTable<data_type, key_type>::HashTable() {}

template<typename data_type, typename key_type>
HashTable<data_type, key_type>::HashTable(int size, std::function<key_type(data_type)> keyReceivingFunction, std::function<int(key_type)> hashFunction)
{
	std::vector<data_type> tableBuffer(size);
	this->table = tableBuffer;

	std::vector<bool> isOccupiedTableBuffer(size);
	this->isOccupiedTable = isOccupiedTableBuffer;

	std::vector<bool> wasDeletedTableBuffer(size);
	this->wasDeletedTable = wasDeletedTableBuffer;

	std::vector<int> attemptsTableBuffer(size);
	this->attemptsTable = attemptsTableBuffer;

	this->hashFunction = hashFunction;
	this->keyReceivingFunction = keyReceivingFunction;
}

template<typename data_type, typename key_type>
HashTable<data_type, key_type>::~HashTable() {}

template<typename data_type, typename key_type>
key_type HashTable<data_type, key_type>::keyReceivingFunctionDefault(data_type data) { return data; }

template<typename data_type, typename key_type>
bool HashTable<data_type, key_type>::insert(data_type data)
{
	key_type key = (keyReceivingFunction) ? keyReceivingFunction(data) : keyReceivingFunctionDefault(data);
	int hash = (hashFunction) ? hashFunction(key) : std::hash<key_type>{}(key);
	for (int attempt = 0; attempt < table.size(); attempt++)
	{
		int hashValue = (hash + attempt * attempt) % table.size();
		if (!isOccupiedTable[hashValue])
		{
			table[hashValue] = data;
			isOccupiedTable[hashValue] = true;
			attemptsTable[hashValue] = attempt + 1;

			return true;
		}
	}
	return false;
}

template<typename data_type, typename key_type>
int HashTable<data_type, key_type>::search(key_type searchKey)
{
	int hash = (hashFunction) ? hashFunction(searchKey) : std::hash<key_type>{}(searchKey);

	for (int attempt = 0; attempt < table.size(); attempt++)
	{
		int hashValue = (hash + attempt * attempt) % table.size();
		if (isOccupiedTable[hashValue])
		{
			key_type checkedKey = (keyReceivingFunction) ? keyReceivingFunction(table[hashValue]) : keyReceivingFunctionDefault(table[hashValue]);
			if (checkedKey == searchKey)
			{
				#ifdef _DEBUG
					std::cout << "attempts: " << attemptsTable[hashValue] << std::endl;
				#endif
				return hashValue;
			}
		} else if (!wasDeletedTable[hashValue]) break;
	}
	throw(std::exception("Object not found"));
}

template<typename data_type, typename key_type>
data_type HashTable<data_type, key_type>::get(key_type searchKey)
{
	return table[search(searchKey)];
}

template<typename data_type, typename key_type>
void HashTable<data_type, key_type>::remove(key_type searchKey)
{
	int hashValue = search(searchKey);
	data_type defaultObject;
	table[hashValue] = defaultObject;
	attemptsTable[hashValue] = 0;
	isOccupiedTable[hashValue] = false;
	wasDeletedTable[hashValue] = true;
}

template<typename data_type, typename key_type>
void HashTable<data_type, key_type>::print()
{
	for (int i = 0; i < table.size(); i++)
	{
		std::cout << i << " - " << attemptsTable[i] << " : " << table[i] << std::endl;
	}

}