#include "../include/TwoBitBranchPredictor.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <trace_file> <btb_size>" << std::endl;
        return 1;
    }

    std::string traceFile = argv[1];
    int btbSize = std::stoi(argv[2]);

    std::cout << "Two-Level Branch Prediction Simulation" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Trace file: " << traceFile << std::endl;
    std::cout << "BTB size: " << btbSize << " entries" << std::endl;
    std::cout << std::endl;

    // Create and run the two-level branch predictor
    TwoBitBranchPredictor predictor(btbSize);
    predictor.simulateTrace(traceFile);
    predictor.printStats();

    return 0;
}