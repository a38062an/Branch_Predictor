#ifndef TRACEREADER_H
#define TRACEREADER_H

#include <vector>
#include <string>

struct Instruction {
    char type; // 'B' for branch, 'R' for other
    int sourceAddr;
    int targetAddr;
    bool direction; // 'F' for forward 'B' for backward
    bool taken; // True if taken, false if not taken, irrelevant for 'R'
};

class TraceReader {
public:
    TraceReader(const std::string& filename);
    std::vector<Instruction> readTrace();

private:
    std::string filename;
    Instruction parseLine(const std::string& line);
};

#endif // TRACEREADER_H