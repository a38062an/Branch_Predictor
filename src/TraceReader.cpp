/*
* We use this util file to read in the paticular trace
* The lines of branch instructions come in two different forms return statements and any other branch type
* Handled both cases when there is a return instruction and anything else
* Return and instruction array filled with details of each line (struct entry)l
*
*/

#include "../include/TraceReader.h"
#include <fstream>
#include <iostream>

TraceReader::TraceReader(const std::string &filename) : filename(filename) {}

Instruction TraceReader::parseLine(const std::string &line) {
    Instruction instr;
    instr.type = line[0];

    if (instr.type == 'B') {
        // Format: B from 00008008 to 00008040 F * taken
        instr.sourceAddr = std::stoul(line.substr(7, 8), nullptr, 16);
        instr.targetAddr = std::stoul(line.substr(19, 8), nullptr, 16);
        instr.direction = line[28];
        instr.taken = (line.substr(32) == "* taken");
    } else if (instr.type == 'R') {
        // Format: R from 00008070               ? not taken
        instr.sourceAddr = std::stoul(line.substr(7, 8), nullptr, 16);
        instr.targetAddr = 0; // No target address for returns
        instr.direction = '?';
        instr.taken = (line.substr(line.find('?') + 2) == "taken");
    }

    return instr;
}


std::vector<Instruction> TraceReader::readTrace() {
    std::vector<Instruction> instructions;
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()){ // Populate the vector with instructions
        while (std::getline(file, line)) {
            instructions.push_back(parseLine(line));
        }
        file.close(); // Close the file
    } else {
        std::cerr << "Error: Unable to open trace file."  << std::endl;

    }

    return instructions;
}








