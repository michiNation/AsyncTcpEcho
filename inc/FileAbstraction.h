#pragma once

#include <fstream>
#include <vector>
class FileAbstraction {

private:

std::ifstream inStream;
int64_t inStreamBytes = 0;

std::ofstream ofStream;
int64_t ofStreamBytesWritten = 0;

bool isInstreamFile{false};
public:
    FileAbstraction(bool IwantToReadaFile);

    void LodeFile(std::string path);

    void CloseFile();

    void WriteBytes(std::vector<uint8_t>& bytes);
    
    void WriteBytes(const char * bytes, int size);

    std::vector<uint8_t> ReadBytes(uint length);

    int64_t GetFileSize();

};