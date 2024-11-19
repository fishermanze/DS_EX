#include "ExternalMergeSorter.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>

ExternalMergeSorter::ExternalMergeSorter(size_t bufferSize, const std::string& inputFileName, const std::string& outputFileName)
    : bufferSize(bufferSize), inputFileName(inputFileName), outputFileName(outputFileName) {}

void ExternalMergeSorter::sort() {
    runGeneration(); // ���ɳ�ʼ�����
    multiwayMerge(); // ִ�ж�·�鲢����
}

// ��ʼ���������
void ExternalMergeSorter::runGeneration() {
    Buffer inputBuffer(bufferSize, inputFileName, true);

    size_t blockCounter = 0;
    size_t start = 0;
    size_t end = 0;

    std::vector<int> run;

    while (inputBuffer.hasMoreData()) {
        inputBuffer.loadBlock();
        start = end;
        while (inputBuffer.hasMoreDataInBlock()) {
            run.push_back(inputBuffer.getNextItem());
            ++end;
        }

        std::sort(run.begin(), run.end());

        std::string tempFileName = "temp_" + std::to_string(blockCounter++) + ".txt";
        tempFiles.push_back(tempFileName);

        Buffer outputBuffer(bufferSize, tempFileName, false);
        outputBuffer.writeMetadata(run.size(), start, end); // д��Ԫ����
        for (const auto& item : run) {
            outputBuffer.setNextItem(item);
        }
        outputBuffer.writeBlock();
        run.clear();
    }
}

void ExternalMergeSorter::multiwayMerge() {
    std::vector<std::ifstream> tempFileStreams;

    for (const auto& tempFile : tempFiles) {
        tempFileStreams.emplace_back(tempFile, std::ios::in);
        if (!tempFileStreams.back().is_open()) {
            throw std::runtime_error("Failed to open temporary file: " + tempFile);
        }
        size_t numItems, start, end;
        tempFileStreams.back() >> numItems >> start >> end; // ��ȡԪ����
        tempFileStreams.back().ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ����Ԫ������
    }

    auto cmp = [](const std::pair<int, size_t>& a, const std::pair<int, size_t>& b) {
        return a.first > b.first;
        };
    std::priority_queue<std::pair<int, size_t>, std::vector<std::pair<int, size_t>>, decltype(cmp)> minHeap(cmp);

    for (size_t i = 0; i < tempFileStreams.size(); ++i) {
        int value;
        if (tempFileStreams[i] >> value) {
            minHeap.emplace(value, i);
        }
    }

    std::ofstream outputFile(outputFileName, std::ios::out);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Failed to open output file: " + outputFileName);
    }

    while (!minHeap.empty()) {
        auto [value, fileIndex] = minHeap.top();
        minHeap.pop();
        outputFile << value << " ";

        int nextValue;
        if (tempFileStreams[fileIndex] >> nextValue) {
            minHeap.emplace(nextValue, fileIndex);
        }
    }
    outputFile << "\n";

    for (auto& stream : tempFileStreams) {
        stream.close();
    }
}
