// main.cpp
#include "../include/BranchPredictor.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Check for correct number of command line arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <trace_file> <btb_size>" << std::endl;
        return 1;
    }

    // Parse command line arguments
    std::string traceFile = argv[1];
    int btbSize = std::stoi(argv[2]);

    // Print simulation parameters
    std::cout << "Static Branch Predictor Simulation" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Trace file: " << traceFile << std::endl;
    std::cout << "BTB size: " << btbSize << std::endl;
    std::cout << "Prediction policy: Backward branches (direction='b') predicted taken," << std::endl;
    std::cout << "                   All other branches predicted not taken" << std::endl;
    std::cout << std::endl;

    // Create branch predictor with specified BTB size
    BranchPredictor predictor(btbSize);

    // Run the simulation
    predictor.simulateTrace(traceFile);

    // Print the results
    predictor.printStats();

    return 0;
}