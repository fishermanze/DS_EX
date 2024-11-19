#pragma once
#include <vector>
#include <fstream>
#include <stdexcept>
#include <string>

class Buffer {
private:
    std::vector<int> data;  // ���������ݴ洢
    size_t capacity;        // ����������
    size_t currentIndex;    // ��ǰ��ȡ��д��λ��
    std::ifstream inputFile;
    std::ofstream outputFile;
    bool isInput;

    // �ļ�Ԫ����
    size_t numItems;        // ��ǰ����������
    size_t startPos;        // ��ʼλ��
    size_t endPos;          // ����λ��

public:
    Buffer(size_t bufferSize, const std::string& fileName, bool inputMode);
    ~Buffer();

    int getNextItem();
    void setNextItem(int item);
    bool hasMoreData();
    bool hasMoreDataInBlock();
    void loadBlock();
    void writeBlock();

    // ����Ԫ����
    void writeMetadata(size_t numItems, size_t start, size_t end);
    void readMetadata(size_t& numItems, size_t& start, size_t& end);
};
