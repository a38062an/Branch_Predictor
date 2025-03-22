#ifndef TWOBITBRANCHPREDICTOR_H
#define TWOBITBRANCHPREDICTOR_H

#include "BranchTargetBuffer.h"
#include "TraceReader.h"
#include <vector>

enum PredictionState {
STRONGLY_NOT_TAKEN = 0,
WEAKLY_NOT_TAKEN = 1,
WEAKLY_TAKEN = 2,
STRONGLY_TAKEN = 3
};



class TwoBitBranchPredictor {
private:
    BranchTargetBuffer* btb;

    PredictionState* stateTable;

    int stateTableSize;

    // Statistics
    int btbHits;
    int btbMisses;
    int staticPredictionHits;
    int staticPredictionMisses;
    int dynamicPredictionHits;
    int dynamicPredictionMisses;

    bool staticPredict(char type, char direction);
    bool dynamicPredict(int sourceAddr);
    int getStateIndex(int sourceAddr);

public:
    TwoBitBranchPredictor(int btbSize);
    ~TwoBitBranchPredictor();

    // Main functionality
    int predictTargetAddress(int sourceAddr);
    void update(int sourceAddr, int targetAddr, bool taken);
    void simulateTrace(const std::string& traceFilename);
    void printStats() const;
};

#endif // TWO_LEVEL_BRANCH_PREDICTOR_H



