#include "../include/TwoBitBranchPredictor.h"

#include <iomanip>
#include <iostream>

TwoBitBranchPredictor::TwoBitBranchPredictor(int btbSize)
    : btb(new BranchTargetBuffer(btbSize)),
      stateTableSize(btbSize),
      btbHits(0), btbMisses(0),
      staticPredictionHits(0), staticPredictionMisses(0),
      dynamicPredictionHits(0), dynamicPredictionMisses(0), btbHitButMispredicted(0) {

    // Initialize state table with default WEAKLY_TAKEN state
    stateTable = new PredictionState[stateTableSize];
    for (int i = 0; i < stateTableSize; i++) {
        stateTable[i] = WEAKLY_TAKEN;
    }

}

TwoBitBranchPredictor::~TwoBitBranchPredictor() {

}

int TwoBitBranchPredictor::getStateIndex(int sourceAddr) {
   return (sourceAddr % stateTableSize);
}

bool TwoBitBranchPredictor::staticPredict(char type, char direction) {
   return(direction == 'B' && type == 'B');
}


bool TwoBitBranchPredictor::dynamicPredict(int sourceAddr) {
    int index = getStateIndex(sourceAddr);
    return (stateTable[index] >= WEAKLY_TAKEN);
}

int TwoBitBranchPredictor::predictTargetAddress(int sourceAddr) {
    return btb->getTargetAddress(sourceAddr);
}
/*
void TwoBitBranchPredictor::update(int sourceAddr, int targetAddr, bool taken) {
    btb -> insert(sourceAddr, targetAddr);

    int index = getStateIndex(sourceAddr);
    if (stateTable[index] < STRONGLY_TAKEN) {
        stateTable[index] = static_cast<PredictionState>(stateTable[index] + 1);
    } else {
    // Move toward STRONGLY_NOT_TAKEN
    if (stateTable[index] > STRONGLY_NOT_TAKEN) {
        stateTable[index] = static_cast<PredictionState>(stateTable[index] - 1);
        }
    }
}
*/
void TwoBitBranchPredictor::update(int sourceAddr, int targetAddr, bool taken) {
    // Update BTB
    btb->insert(sourceAddr, targetAddr);

    // Update prediction state based on actual outcome
    int index = getStateIndex(sourceAddr);

    if (taken) {
        // Branch was taken, move toward STRONGLY_TAKEN
        if (stateTable[index] < STRONGLY_TAKEN) {
            stateTable[index] = static_cast<PredictionState>(stateTable[index] + 1);
        }
    } else {
        // Branch was not taken, move toward STRONGLY_NOT_TAKEN
        if (stateTable[index] > STRONGLY_NOT_TAKEN) {
            stateTable[index] = static_cast<PredictionState>(stateTable[index] - 1);
        }
    }
}
void TwoBitBranchPredictor::simulateTrace(const std::string& traceFilename) {
    TraceReader reader(traceFilename);
    std::vector<Instruction> instructions = reader.readTrace();

   /* std::cout << "Simulating: " << instructions.size() << " instructions..." << std::endl;*/

    for (const auto& instr : instructions) {
        // BTB prediction
        int predictedTarget = predictTargetAddress(instr.sourceAddr);

        bool inBTB = (predictedTarget != -1);

        if (inBTB) {
            btbHits++;
        } else {
            btbMisses++;
        }

        // Static prediction
        bool staticTaken = staticPredict(instr.type, instr.direction);
        if (staticTaken == instr.taken) {
            staticPredictionHits++;
        } else {
            staticPredictionMisses++;
        }

        // Dynamic prediction
        bool dynamicTaken = dynamicPredict(instr.sourceAddr);
        if (dynamicTaken == instr.taken) {
            dynamicPredictionHits++;
        } else {
            dynamicPredictionMisses++;
            if (inBTB){
                btbHitButMispredicted++;
            }
        }

        // Update state and BTB
        update(instr.sourceAddr, instr.targetAddr, instr.taken);
    }
}



void TwoBitBranchPredictor::printStats() const {
    int totalInstructions = staticPredictionHits + staticPredictionMisses;

    double staticAccuracy = totalInstructions > 0 ?
        (double)staticPredictionHits / totalInstructions * 100.0 : 0.0;

    double dynamicAccuracy = totalInstructions > 0 ?
        (double)dynamicPredictionHits / totalInstructions * 100.0 : 0.0;

    double btbAccuracy = (btbHits + btbMisses) > 0 ?
        (double)btbHits / (btbHits + btbMisses) * 100.0 : 0.0;

    double improvement = dynamicAccuracy - staticAccuracy;

    std::cout << "Branch Predictor Statistics:" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Total instructions processed: " << totalInstructions << std::endl;
    std::cout << std::endl;

    std::cout << "Static Branch Prediction Statistics:" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Direction prediction hits: " << staticPredictionHits << std::endl;
    std::cout << "Direction prediction misses: " << staticPredictionMisses << std::endl;
    std::cout << "Direction prediction accuracy: " << std::fixed << std::setprecision(2)
              << staticAccuracy << "%" << std::endl;
    std::cout << std::endl;

    std::cout << "Two-Level Branch Prediction Statistics:" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Direction prediction hits: " << dynamicPredictionHits << std::endl;
    std::cout << "Direction prediction misses: " << dynamicPredictionMisses << std::endl;
    std::cout << "Direction prediction accuracy: " << std::fixed
              << dynamicAccuracy << "%" << std::endl;
    std::cout << "Improvement over static prediction: " << std::fixed
              << improvement << "%" << std::endl;
    std::cout << std::endl;

    std::cout << "BTB Statistics:" << std::endl;
    std::cout << "--------------" << std::endl;
    std::cout << "BTB hits: " << btbHits << std::endl;
    std::cout << "BTB misses: " << btbMisses << std::endl;
    std::cout << "BTB hits but direction mispredicted: " << btbHitButMispredicted << std::endl;
    std::cout << "BTB hit rate: " << std::fixed << std::setprecision(2)
              << btbAccuracy << "%" << std::endl;
}


