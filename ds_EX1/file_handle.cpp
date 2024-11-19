#include"file_handle.h"
#include<iostream>
#include <fstream>
#include <random>
#include <sstream> 

void file_handle::generateFile(const std::string& filename, int capacity, int min, int max)
{
	std::ofstream file;
	file.open(filename, std::ios::out);
	std::mt19937 rand_engine;
	std::uniform_int_distribution<int> dist(min, max);
	for (int i = 0; i < capacity; i++)
	{
		file<< dist(rand_engine)<<"\n ";
	}
	file.close();
}

void file_handle::readFile(const std::string& filename, int count)
{
	std::ifstream file;
	file.open(filename, std::ios::in);
	for (int i = 0; i < count; i++)
	{
		int temp = 0;
		file >> temp;
		std::cout << temp << " ";
	}
	std::cout << std::endl;
	file.close();
}

void file_handle::readALL(const std::string& filename)
{
	std::ifstream file(filename);
	std::stringstream ss;
	ss << file.rdbuf();
	std::string contents(ss.str());
	std::cout << contents << std::endl;
	file.close();
}