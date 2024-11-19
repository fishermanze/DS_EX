#pragma once
#include<iostream>
class file_handle
{
public:
	//随机数生成函数
	void generateFile(const std::string& filename, int capacity, int min, int max);
	//读文件显示到屏幕上
	void readFile(const std::string& filename ,int count);
	//将文件所有内容显示在屏幕上
	void readALL(const std::string& filename);

};