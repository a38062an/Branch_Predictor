# Branch Predictor Project - Quick Reference Guide

## 🚀 Project Overview

**Purpose**: Simulate and analyze branch prediction algorithms used in modern processors
**Languages**: C++ (simulation) + Python (analysis)
**Key Focus**: Compare static vs dynamic prediction strategies

---

## 📁 File Structure & Responsibilities

### Core C++ Files

| File | Purpose | Key Function |
|------|---------|--------------|
| `TraceReader.h/.cpp` | Parse instruction traces | Convert text → Instruction objects |
| `BranchTargetBuffer.h/.cpp` | LRU cache implementation | Store branch targets, LRU replacement |
| `BranchPredictor.h/.cpp` | Static prediction | Predict taken if in BTB |
| `TwoBitBranchPredictor.h/.cpp` | Dynamic prediction | 2-bit state machine + static comparison |
| `main.cpp` | Static predictor CLI | `./branch_sim trace btb_size` |
| `TwoBitPredictorMain.cpp` | Dynamic predictor CLI | `./branch_sim_TwoBit trace btb_size` |

### Python Analysis Files

| File | Purpose | Output |
|------|---------|---------|
| `plot_btb_overhead.py` | Static predictor analysis | BTB size vs performance plots |
| `plot_btb_overheadTwoBit.py` | Dynamic predictor analysis | Static vs dynamic comparison |
| `twobit_visualise_results.py` | Accuracy visualization | Accuracy trends over BTB sizes |

---

## 🔧 Build & Run Commands

### Compilation

```bash
# Static predictor
g++ -o branch_sim main.cpp BranchPredictor.cpp BranchTargetBuffer.cpp TraceReader.cpp -Iinclude

# Dynamic predictor  
g++ -o branch_sim_TwoBit TwoBitPredictorMain.cpp TwoBitBranchPredictor.cpp BranchTargetBuffer.cpp TraceReader.cpp -Iinclude
```

### Execution

```bash
# Single simulation
./branch_sim misc/block_profile 64
./branch_sim_TwoBit misc/block_profile 128

# Automated analysis
cd src
python3 plot_btb_overhead.py
python3 plot_btb_overheadTwoBit.py
```

---

## 🧠 Core Algorithms

### Static Prediction

```cpp
bool predictTaken(int sourceAddr) {
    return (btb->getTargetAddress(sourceAddr) != -1);  // In BTB = predict taken
}
```

### Dynamic Prediction (2-bit State Machine)

```
States: 0=Strongly NT, 1=Weakly NT, 2=Weakly T, 3=Strongly T
Prediction: state >= 2 → predict TAKEN
Update: taken ? state++ : state-- (with saturation)
```

### LRU Cache (BTB)

```
Structure: HEAD ↔ [MRU] ↔ [entries] ↔ [LRU] ↔ TAIL
Hit: moveToFront(entry)
Miss: overwrite LRU, moveToFront(victim)
```

---

## 📊 Key Data Structures

### Instruction Format

```cpp
struct Instruction {
    char type;        // 'B'=branch, 'R'=return
    int sourceAddr;   // Branch location
    int targetAddr;   // Jump destination  
    char direction;   // 'F'=forward, 'B'=backward
    bool taken;       // Actually taken?
};
```

### BTB Entry

```cpp
struct BTBEntry {
    int sourceAddr_tag;   // Cache key
    int targetAddr_data;  // Cache value
    BTBEntry *prev, *next; // LRU pointers
};
```

### State Table

```cpp
PredictionState* stateTable;  // Array of 2-bit counters
int index = sourceAddr % stateTableSize;  // Address hashing
```

---

## 🎯 Performance Metrics

### BTB Performance

- **Hit Rate**: `btb_hits / (btb_hits + btb_misses)`
- **Miss Penalty**: Pipeline flush cost
- **Overhead**: `instructions_fetched / instructions_executed`

### Prediction Accuracy

- **Static Accuracy**: Correct predictions / total predictions
- **Dynamic Accuracy**: 2-bit predictor accuracy
- **Improvement**: Dynamic - Static accuracy

---

## 🐍 Python Analysis Workflow

### Data Flow

```
C++ Simulator → Text Output → Python Regex → Numerical Data → Matplotlib Plots
```

### Key Python Functions

- `subprocess.run()`: Execute C++ simulators
- `re.search()`: Extract metrics from output
- `matplotlib.pyplot`: Generate performance graphs
- Batch processing: Automate 100+ simulations

---

## 💡 Design Insights

### Why Static Prediction Works

- **Spatial Locality**: Nearby branches have similar behavior
- **Temporal Locality**: Recently taken branches likely to be taken again
- **Simple Hardware**: Just check BTB presence

### Why 2-bit Counters Work

- **Hysteresis**: Resist single bad predictions
- **Pattern Recognition**: Capture common branch patterns
- **Hardware Efficient**: 2 bits per branch, simple logic

### Why LRU Cache Works

- **Recency**: Recently used branches execute again soon
- **Program Patterns**: Loops and frequent functions stay cached
- **Hardware Realistic**: Mimics actual processor BTBs

---

## 🔬 Research Applications

### Computer Architecture Education

- Understand processor optimization techniques
- Learn cache replacement policies
- Experience performance analysis methodology

### Performance Research

- Compare prediction algorithms
- Study BTB size trade-offs
- Analyze workload-specific optimizations

### Industry Relevance

- Real processors use similar techniques
- Performance gains: 15-20% from good prediction
- Design trade-offs: accuracy vs hardware cost

---

## 📚 Study Focus Areas

### Core Concepts

1. **Branch Prediction**: Why needed, how it works
2. **Cache Design**: LRU, replacement policies, addressing
3. **State Machines**: 2-bit counter behavior, transitions
4. **Performance Analysis**: Metrics, trade-offs, optimization

### Implementation Details

1. **Doubly-Linked Lists**: LRU cache operations
2. **Address Hashing**: Modulo mapping to table indices  
3. **File Parsing**: Text processing, regex extraction
4. **Data Visualization**: Python automation, matplotlib

### Analysis Skills

1. **Batch Processing**: Automated testing workflows
2. **Statistical Analysis**: Trend identification, comparison
3. **Performance Modeling**: Overhead calculation, optimization
4. **Scientific Visualization**: Professional plot generation

This project demonstrates real computer architecture concepts with research-quality analysis tools - perfect for understanding how modern processors achieve high performance!
