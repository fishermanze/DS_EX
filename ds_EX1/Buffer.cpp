#include "buffer.h"
#include <sstream>
#include <limits>
#include<iostream>

// ���캯��
Buffer::Buffer(size_t bufferSize, const std::string& fileName, bool inputMode)
    : capacity(bufferSize), currentIndex(0), isInput(inputMode) {
    data.resize(bufferSize);
    if (inputMode) {
        inputFile.open(fileName);
        if (!inputFile.is_open()) {
            throw std::runtime_error("Failed to open input file: " + fileName);
        }
        readMetadata(numItems, startPos, endPos); // ��ȡԪ����
    }
    else {
        outputFile.open(fileName, std::ios::out);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Failed to open output file: " + fileName);
        }
    }

}

// ��������
Buffer::~Buffer() {
    if (inputFile.is_open()) {
        inputFile.close();
    }
    if (outputFile.is_open()) {
        writeBlock(); // ȷ��ʣ�����ݱ�д��
        outputFile.close();
    }
}

// д��Ԫ���ݵ��ļ�
void Buffer::writeMetadata(size_t numItems, size_t start, size_t end) {
    if (!outputFile.is_open()) {
        throw std::runtime_error("Output file is not open!");
    }
    outputFile << numItems << " " << start << " " << end << "\n";
}

// ��ȡԪ����
void Buffer::readMetadata(size_t& numItems, size_t& start, size_t& end) {
    if (!inputFile.is_open()) {
        throw std::runtime_error("Input file is not open!");
    }
    inputFile >> numItems >> start >> end;
    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ����Ԫ������
}

// �ӻ�������ȡ��һ��Ԫ��
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


// д����һ��Ԫ�ص�������
void Buffer::setNextItem(int item) {
    if (isInput) {
        throw std::runtime_error("Buffer is in input mode!");
    }
    if (currentIndex >= capacity) {
        writeBlock(); // д����д���ļ�
    }
    data[currentIndex++] = item;
}

// ����һ�����ݿ鵽������
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



// ��������д�뵽�ļ�
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

// �ж��Ƿ�������
bool Buffer::hasMoreData() {
    return !isInput || currentIndex < data.size() || !inputFile.eof();
}

// �жϵ�ǰ���Ƿ�������
bool Buffer::hasMoreDataInBlock() {
    return currentIndex < data.size();
}
