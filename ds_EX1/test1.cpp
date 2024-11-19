#include<iostream>
#include"buffer.h"
#include"ExternalMergeSorter.h"
#include"file_handle.h"

int main()
{
    const std::string inputFileName = "./input.txt";
    const std::string outputFileName = "./output.txt";
    const size_t bufferSize = 100;  // 缓冲区大小
    file_handle handle;

   /* std::ifstream file(inputFileName);
    while (!file.eof())
    {
        int temp = 0;
        file >> temp;
        std::cout << temp<<" ";
    }
    std::cout << std::endl;*/
    handle.generateFile(inputFileName, 300, 1, 1000);
    //handle.readALL(inputFileName);
    try {
        ExternalMergeSorter sorter(bufferSize, inputFileName, outputFileName);
        sorter.sort(); // 外部归并排序
        std::cout << "Sorting completed!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

	return 0;
}
