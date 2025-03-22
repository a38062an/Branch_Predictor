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

    // Set instruction type (B, R, or M)
    instr.type = line[0];

    // Find the source address (common for all types)
    size_t fromPos = line.find("from ");
    if (fromPos != std::string::npos) {
        instr.sourceAddr = std::stoul(line.substr(fromPos + 5, 8), nullptr, 16);
    }

    // Handle different instruction types
    if (instr.type == 'B') {
        // Find target address for branch instructions
        size_t toPos = line.find("to ");
        if (toPos != std::string::npos) {
            instr.targetAddr = std::stoul(line.substr(toPos + 3, 8), nullptr, 16);
        }

        // Get direction (B or F)
        size_t dirPos = toPos + 12; // Position after the target address
        if (dirPos < line.length()) {
            instr.direction = line[dirPos];
        }
    } else {
        // For R (return) and M (?) instructions, no target address
        instr.targetAddr = 0;
        instr.direction = '?';
    }

    // Check if the branch was taken
    // Look for "* taken" or "? taken" vs "not taken"
    if (line.find("not taken") != std::string::npos) {
        instr.taken = false;
    } else if (line.find("taken") != std::string::npos) {
        instr.taken = true;
    } else {
        // Default if neither found (shouldn't happen with your data)
        instr.taken = false;
    }

    return instr;
}

/*
Instruction TraceReader::parseLine(const std::string &line) {
    Instruction instr;
    instr.type = line[0];

    // Find the source address (common for both B and R types)
    size_t fromPos = line.find("from ");
    if (fromPos != std::string::npos) {
        instr.sourceAddr = std::stoul(line.substr(fromPos + 5, 8), nullptr, 16);
    }

    if (instr.type == 'B') {
        // Find target address for branch instructions
        size_t toPos = line.find("to ");
        if (toPos != std::string::npos) {
            instr.targetAddr = std::stoul(line.substr(toPos + 3, 8), nullptr, 16);
        }

        // Get direction (usually at position 28, but using a more robust search)
        size_t dirPos = line.find(" to ") + 12;
        if (dirPos < line.length()) {
            instr.direction = line[dirPos];
        }
    } else if (instr.type == 'R') {
        instr.targetAddr = 0; // No target address for returns
        instr.direction = '?';

    }

    // Check if the branch was taken
    instr.taken = (line.find("* taken") != std::string::npos);

    return instr;
}
*/
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








