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
    int btbHitButMispredicted;
    int staticPredictionHits;
    int staticPredictionMisses;
    int dynamicPredictionHits;
    int dynamicPredictionMisses;

    // Helper methods
    int getStateIndex(int sourceAddr);
    bool staticPredict(int sourceAddr);
    bool dynamicPredict(int sourceAddr);
    int predictTargetAddress(int sourceAddr);
    void update(Instruction instr);
public:

    TwoBitBranchPredictor(int btbSize);
    ~TwoBitBranchPredictor();

    void simulateTrace(const std::string& traceFilename);
    void printStats() const;


};

#endif