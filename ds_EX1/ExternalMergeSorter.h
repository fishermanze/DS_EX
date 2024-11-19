#pragma once
#include <string>
#include <vector>
#include <queue>
#include "buffer.h"

class ExternalMergeSorter {
private:
    size_t bufferSize;               // 缓冲区大小
    std::string inputFileName;       // 输入文件
    std::string outputFileName;      // 输出文件
    std::vector<std::string> tempFiles; // 中间文件列表

    // 生成初始排序块
    void runGeneration();

    // 多路归并排序
    void multiwayMerge();

public:
    ExternalMergeSorter(size_t bufferSize, const std::string& inputFileName, const std::string& outputFileName);
    void sort();
};
