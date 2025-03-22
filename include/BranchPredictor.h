// BranchPredictor.h
#ifndef BRANCH_PREDICTOR_H
#define BRANCH_PREDICTOR_H

#include "BranchTargetBuffer.h"
#include "TraceReader.h"
#include <vector>

class BranchPredictor {
private:
    BranchTargetBuffer* btb;
    int btbHits;
    int btbMisses;
    int predictionHits;
    int predictionMisses;

public:
    BranchPredictor(int btbSize);
    ~BranchPredictor();

    // Predict if a branch will be taken using static prediction:
    // - Backward branches (target < source) are predicted taken
    // - Forward branches (target > source) are predicted not taken
    bool predictTaken(char type, char direction);

    // Get predicted target address from BTB
    int predictTargetAddress(int sourceAddr);

    // Update BTB based on actual outcome
    void update(int sourceAddr, int targetAddr);

    // Run simulation on a trace file
    void simulateTrace(const std::string& traceFilename);

    // Print statistics
    void printStats() const;
};

#endif // BRANCH_PREDICTOR_H