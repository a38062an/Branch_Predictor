#include "../include/BranchPredictor.h"
#include <iostream>


/*
* This Is the branch prediction logic which compares the branch direction and type of branch.
* If the predicted branch matches the actual branch we increment our branch predition hits else we increment the miss.
* We check if the branch is in the cache, if not we have a cache miss else we have a cache hit
* We then print these statistics
*
*/


BranchPredictor::BranchPredictor(int btbSize)
  : btb(new BranchTargetBuffer(btbSize)), btbHits(0), btbMisses(0),
  predictionHits(0), predictionMisses(0), btbHitButMispredicted(0){}


BranchPredictor::~BranchPredictor() {
    delete btb;
}

/*
bool BranchPredictor::predictTaken(char type, char direction) {
    return (direction == 'B' and type == 'B');
}
*/

bool BranchPredictor::predictTaken(int sourceAddr) {
    return (btb->getTargetAddress(sourceAddr) != -1);
}

int BranchPredictor::predictTargetAddress(int sourceAddr){
    int target = btb -> getTargetAddress(sourceAddr);
    return target;
}

void BranchPredictor::update(Instruction instr) {
    if (instr.taken){ // If its taken add it to the cache
        btb -> insert(instr.sourceAddr, instr.targetAddr);
    }
}

void BranchPredictor::simulateTrace(const std::string &traceFilename) {
    TraceReader reader(traceFilename);
    std::vector<Instruction> instructions = reader.readTrace();

    std::cout << "Simluating: " << instructions.size() << " instructions..." << std::endl;

    std::cout << "what the hell" << std::endl;


    /*
    for (int i = 0; i < 10; i++) {
        Instruction instr = instructions[i];
        std::cout << "Debug line " << i << ": Type=" << instr.type
                  << ", Direction=" << instr.direction
                  << ", Addr=" << std::hex << instr.sourceAddr
                  << " to " << instr.targetAddr << std::dec
                  << ", Taken=" << instr.taken
                  << std::endl;
    }
    */
    for (const auto& instr : instructions) {

        int predictedTarget = predictTargetAddress(instr.sourceAddr);

        bool inBTB = (predictedTarget != -1);

        if (inBTB){
            btbHits++;
        } else {
            btbMisses++;
        }

        bool taken = predictTaken(instr.sourceAddr);

        // Debugging
        /*
        std::cout << "Debug line "  << ": Type=" << instr.type
                 << ", Direction=" << instr.direction
                 << ", Addr=" << std::hex << instr.sourceAddr
                 << " to " << instr.targetAddr << std::dec
                 << ", Taken=" << instr.taken
                 << std::endl;
        */

        if (taken == instr.taken){
            predictionHits++;
        } else {
            predictionMisses++;

            // If we mispredict and our value that is additional overhead
            if (inBTB){
                btbHitButMispredicted++;
            }

        }

        update(instr);

     }
}

void BranchPredictor::printStats() const {
    int totalInstructions = predictionHits + predictionMisses;
    double predictionAccuracy = totalInstructions > 0 ?
        (double)predictionHits / totalInstructions * 100.0 : 0.0;

    double btbAccuracy = (btbHits + btbMisses) > 0 ?
        (double)btbHits / (btbHits + btbMisses) * 100.0 : 0.0;

    std::cout << "Static Branch Predictor Statistics:" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Total instructions processed: " << totalInstructions << std::endl;
    std::cout << "Direction prediction hits: " << predictionHits << std::endl;
    std::cout << "Direction prediction misses: " << predictionMisses << std::endl;
    std::cout << "Direction prediction accuracy: " << predictionAccuracy << "%" << std::endl;
    std::cout << "BTB hits but direction mispredicted: " << btbHitButMispredicted << std::endl;
    std::cout << std::endl;
    std::cout << "BTB hits: " << btbHits << std::endl;
    std::cout << "BTB misses: " << btbMisses << std::endl;
    std::cout << "BTB hit rate: " << btbAccuracy << "%" << std::endl;
}








