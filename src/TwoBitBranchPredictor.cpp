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
    delete[] stateTable;
    delete btb;
}

int TwoBitBranchPredictor::getStateIndex(int sourceAddr) {
    return (sourceAddr % stateTableSize);
}

// Static prediction now matches your updated model
bool TwoBitBranchPredictor::staticPredict(int sourceAddr) {
    // Match your updated static model - predict taken if in BTB
    return (btb->getTargetAddress(sourceAddr) != -1);
}

bool TwoBitBranchPredictor::dynamicPredict(int sourceAddr) {
    int index = getStateIndex(sourceAddr);
    return (stateTable[index] >= WEAKLY_TAKEN);
}

int TwoBitBranchPredictor::predictTargetAddress(int sourceAddr) {
    return btb->getTargetAddress(sourceAddr);
}

void TwoBitBranchPredictor::update(Instruction instr) {
    // Always update BTB for taken branches (matches your static model)
    if (instr.taken) {
        btb->insert(instr.sourceAddr, instr.targetAddr);
    }

    // Update the 2-bit counter based on whether the branch was taken
    int index = getStateIndex(instr.sourceAddr);
    if (instr.taken) {
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

    std::cout << "Simulating: " << instructions.size() << " instructions..." << std::endl;

    for (const auto& instr : instructions) {
        // BTB prediction
        int predictedTarget = predictTargetAddress(instr.sourceAddr);
        bool inBTB = (predictedTarget != -1);

        if (inBTB) {
            btbHits++;
        } else {
            btbMisses++;
        }

        // Static prediction (using your updated model)
        bool staticTaken = staticPredict(instr.sourceAddr);
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
            if (inBTB) {
                btbHitButMispredicted++;
            }
        }

        // Update state and BTB
        update(instr);
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

    // Modified output format for Python parsing
    std::cout << "Static Accuracy: " << std::fixed << std::setprecision(2) << staticAccuracy << "%" << std::endl;
    std::cout << "Dynamic Accuracy: " << std::fixed << std::setprecision(2) << dynamicAccuracy << "%" << std::endl;
    std::cout << "Improvement: " << std::fixed << std::setprecision(2) << improvement << "%" << std::endl;
    std::cout << "BTB Hit Rate: " << std::fixed << std::setprecision(2) << btbAccuracy << "%" << std::endl;

    std::cout << std::endl;
    std::cout << "BTB hits: " << btbHits << std::endl;
    std::cout << "BTB misses: " << btbMisses << std::endl;
    std::cout << "BTB hits but direction mispredicted: " << btbHitButMispredicted << std::endl;

}


