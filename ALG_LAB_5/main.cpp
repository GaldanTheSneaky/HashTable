#include"hashtable.h"
#include<iostream>
#include<string>
#include<fstream>


int main()
{
	std::ifstream file("file.txt");
	if (!file.is_open())
	{
		std::cout << "File cannot be open!" << std::endl;
	}
	else
	{
		HashTable<std::string, std::string> hashTable(50);
		std::string word;
		while (file >> word)
		{
			hashTable.insert(word);
		}
		hashTable.remove("velit");
		hashTable.remove("voluptate");
		hashTable.print();
		std::cout << "Enter the searched word: " << std::endl;
		std::cin >> word;
		std::cout << "Position of '" << word << "' : " << hashTable.search(word) << std::endl;
	}
	return 0;
}