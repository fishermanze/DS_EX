#include "buffer.h"
#include <sstream>
#include <limits>
#include<iostream>

// 构造函数
Buffer::Buffer(size_t bufferSize, const std::string& fileName, bool inputMode)
    : capacity(bufferSize), currentIndex(0), isInput(inputMode) {
    data.resize(bufferSize);
    if (inputMode) {
        inputFile.open(fileName);
        if (!inputFile.is_open()) {
            throw std::runtime_error("Failed to open input file: " + fileName);
        }
        readMetadata(numItems, startPos, endPos); // 读取元数据
    }
    else {
        outputFile.open(fileName, std::ios::out);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Failed to open output file: " + fileName);
        }
    }

}

// 析构函数
Buffer::~Buffer() {
    if (inputFile.is_open()) {
        inputFile.close();
    }
    if (outputFile.is_open()) {
        writeBlock(); // 确保剩余数据被写入
        outputFile.close();
    }
}

// 写入元数据到文件
void Buffer::writeMetadata(size_t numItems, size_t start, size_t end) {
    if (!outputFile.is_open()) {
        throw std::runtime_error("Output file is not open!");
    }
    outputFile << numItems << " " << start << " " << end << "\n";
}

// 读取元数据
void Buffer::readMetadata(size_t& numItems, size_t& start, size_t& end) {
    if (!inputFile.is_open()) {
        throw std::runtime_error("Input file is not open!");
    }
    inputFile >> numItems >> start >> end;
    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 跳过元数据行
}

// 从缓冲区获取下一个元素
int Buffer::getNextItem() {
    if (!isInput) {
        throw std::runtime_error("Buffer is in output mode!");
    }
    if (currentIndex >= data.size()) {
        loadBlock();
        if (data.empty()) {
            throw std::runtime_error("No more data to read.");
        }
    }
    return data[currentIndex++];
}


// 写入下一个元素到缓冲区
void Buffer::setNextItem(int item) {
    if (isInput) {
        throw std::runtime_error("Buffer is in input mode!");
    }
    if (currentIndex >= capacity) {
        writeBlock(); // 写满后写入文件
    }
    data[currentIndex++] = item;
}

// 加载一个数据块到缓冲区
void Buffer::loadBlock() {
    if (!isInput) {
        throw std::runtime_error("Buffer is in output mode!");
    }
    if (!inputFile.is_open()) {
        throw std::runtime_error("Input file is not open!");
    }

    data.clear();
    currentIndex = 0;

    int value;
    while (data.size() < capacity && inputFile >> value) {
        //std::cout << "Read value: " << value << std::endl;
        data.push_back(value);
        
    }

    if (inputFile.fail() && !inputFile.eof()) {
        throw std::runtime_error("Error reading from input file.");
    }

    if (data.size() == 0 && !inputFile.eof()) {
        throw std::runtime_error("Failed to load data block from input file.");
    }

    
}



// 将缓冲区写入到文件
void Buffer::writeBlock() {
    if (isInput) {
        throw std::runtime_error("Buffer is in input mode!");
    }
    for (size_t i = 0; i < currentIndex; ++i) {
        outputFile << data[i];
        if (i < currentIndex - 1) {
            outputFile << " ";
        }
    }
    outputFile << "\n";
    currentIndex = 0;
}

// 判断是否还有数据
bool Buffer::hasMoreData() {
    return !isInput || currentIndex < data.size() || !inputFile.eof();
}

// 判断当前块是否还有数据
bool Buffer::hasMoreDataInBlock() {
    return currentIndex < data.size();
}
