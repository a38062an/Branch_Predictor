# Branch Predictor Simulation

C++ implementation of branch prediction algorithms with Branch Target Buffer (BTB) simulation and Python visualization tools.

## Overview

This project implements two branch prediction strategies used in modern processors:

1. **Static Branch Predictor**: Uses directional heuristics (backward branches predicted taken, forward branches predicted not taken)
2. **Two-Bit Dynamic Branch Predictor**: Adaptive prediction using a 4-state finite state machine

Both predictors use a configurable Branch Target Buffer (BTB) with LRU replacement policy to cache branch target addresses.

## Components

- **BranchPredictor**: Static prediction implementation
- **TwoBitBranchPredictor**: Dynamic prediction with 2-bit counters
- **BranchTargetBuffer**: LRU cache using doubly-linked list
- **TraceReader**: Instruction trace parser

## Python Analysis Tools

Python handles data analysis and visualization:

Python handles data analysis and visualization:

- **plot_btb_overhead.py**: Automates simulations across BTB sizes, generates performance plots
- **plot_btb_overheadTwoBit.py**: Analyzes two-bit predictor performance  
- **twobit_visualise_results.py**: Visualizes accuracy trends

Uses subprocess to run C++ simulators, matplotlib for graphs, and regex for parsing output.

## Building and Running

### Build Options

Using CMake:

```bash
mkdir build && cd build
cmake ..
make
```

Manual compilation:

```bash
# Static predictor
g++ -o branch_sim src/main.cpp src/BranchPredictor.cpp src/BranchTargetBuffer.cpp src/TraceReader.cpp -Iinclude

# Two-bit predictor  
g++ -o branch_sim_TwoBit src/TwoBitPredictorMain.cpp src/TwoBitBranchPredictor.cpp src/BranchTargetBuffer.cpp src/TraceReader.cpp -Iinclude
```

### Running Simulations

Single runs:

```bash
./branch_sim misc/block_profile 64
./branch_sim_TwoBit misc/block_profile 128
```

Automated analysis:

```bash
cd src
python3 plot_btb_overhead.py
python3 plot_btb_overheadTwoBit.py
```

## Output Analysis

Static predictor shows BTB hit rates and prediction accuracy. Two-bit predictor compares static vs dynamic performance and shows state machine effectiveness.

Static predictor output:

- BTB Hit Rate: Percentage of branches found in cache
- Prediction Accuracy: Overall correctness of taken/not-taken predictions
- Overhead Analysis: Performance cost vs. BTB size trade-offs

Two-bit predictor output:

- Static vs Dynamic Accuracy: Comparison of prediction strategies
- State Machine Performance: Effectiveness of adaptive learning
- BTB Utilization: Cache efficiency across different workloads

## Implementation Details

Branches are cached when taken using LRU replacement. Static prediction uses directional heuristics. Dynamic prediction uses a 4-state FSM that adapts to program behavior.

Branch caching policy:

- Branches are cached when taken (active branch optimization)
- Both forward and backward branches are cached
- LRU replacement ensures recently used branches stay accessible

Prediction strategies:

1. Static: Backward branches → taken, Forward branches → not taken
2. Dynamic: Four-state FSM (Strongly/Weakly Taken/Not-Taken) with learning

BTB implementation:

- Doubly-linked list for efficient LRU operations
- Configurable size for performance studies
- Address-based indexing with collision handling

## Project Structure

```diagram
Branch_Predictor/
├── include/                 # Header files
├── src/                     # C++ source and Python scripts
├── misc/                    # Test data and results
├── CMakeLists.txt          # Build configuration
└── README.md               # Documentation
```
