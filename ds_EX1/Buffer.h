#pragma once
#include <vector>
#include <fstream>
#include <stdexcept>
#include <string>

class Buffer {
private:
    std::vector<int> data;  // 缓冲区数据存储
    size_t capacity;        // 缓冲区容量
    size_t currentIndex;    // 当前读取或写入位置
    std::ifstream inputFile;
    std::ofstream outputFile;
    bool isInput;

    // 文件元数据
    size_t numItems;        // 当前块数字数量
    size_t startPos;        // 起始位置
    size_t endPos;          // 结束位置

public:
    Buffer(size_t bufferSize, const std::string& fileName, bool inputMode);
    ~Buffer();

    int getNextItem();
    void setNextItem(int item);
    bool hasMoreData();
    bool hasMoreDataInBlock();
    void loadBlock();
    void writeBlock();

    // 设置元数据
    void writeMetadata(size_t numItems, size_t start, size_t end);
    void readMetadata(size_t& numItems, size_t& start, size_t& end);
};
