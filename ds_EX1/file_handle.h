#pragma once
#include<iostream>
class file_handle
{
public:
	//��������ɺ���
	void generateFile(const std::string& filename, int capacity, int min, int max);
	//���ļ���ʾ����Ļ��
	void readFile(const std::string& filename ,int count);
	//���ļ�����������ʾ����Ļ��
	void readALL(const std::string& filename);

};