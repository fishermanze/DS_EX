#pragma once
#include <string>
#include <vector>
#include <queue>
#include "buffer.h"

class ExternalMergeSorter {
private:
    size_t bufferSize;               // ��������С
    std::string inputFileName;       // �����ļ�
    std::string outputFileName;      // ����ļ�
    std::vector<std::string> tempFiles; // �м��ļ��б�

    // ���ɳ�ʼ�����
    void runGeneration();

    // ��·�鲢����
    void multiwayMerge();

public:
    ExternalMergeSorter(size_t bufferSize, const std::string& inputFileName, const std::string& outputFileName);
    void sort();
};
