# Branch Predictor Project - Complete Technical Documentation

## Project Summary

This project is a comprehensive implementation and analysis of **branch prediction algorithms** used in modern computer processors. The project combines C++ implementation of prediction logic with Python-based data analysis and visualization tools to provide deep insights into processor performance optimization.

## What This Code Does

### Core Functionality

The project simulates two fundamental branch prediction strategies that processors use to improve performance:

1. **Static Branch Prediction**
   - Uses simple heuristics: backward branches (loops) are predicted taken, forward branches (conditionals) are predicted not taken
   - Provides baseline performance metrics for comparison

2. **Two-Bit Dynamic Branch Prediction**
   - Implements a 4-state finite state machine (Strongly Taken, Weakly Taken, Weakly Not Taken, Strongly Not Taken)
   - Adapts to program behavior by learning from branch history
   - Provides superior accuracy for programs with predictable patterns

### Branch Target Buffer (BTB) Simulation

Both predictors utilize a **Branch Target Buffer** - a critical processor component that:

- Caches branch target addresses to avoid recalculation
- Uses Least Recently Used (LRU) replacement policy
- Configurable size allows studying performance trade-offs
- Implemented with doubly-linked list for efficient operations

### Performance Analysis

The simulators process instruction traces and generate comprehensive statistics:

- BTB hit/miss rates
- Prediction accuracy percentages
- Misprediction overhead analysis
- Performance scaling with different cache sizes

---

## Detailed File-by-File Explanations

### Header Files (.h)

#### 1. `TraceReader.h` - Instruction Format Definition

**Purpose**: Defines the data structure for parsing instruction traces

```cpp
struct Instruction {
    char type;        // 'B' for branch, 'R' for return
    int sourceAddr;   // Where the instruction is located
    int targetAddr;   // Where it jumps to (if taken)
    char direction;   // 'F' forward, 'B' backward
    bool taken;       // True if branch was actually taken
};
```

**Key Components**:

- `TraceReader(filename)`: Constructor takes trace file path
- `readTrace()`: Returns vector of all instructions from file
- `parseLine()`: Converts one line of text into Instruction struct

**What it does**: Each line in your trace file gets converted into this struct for simulation processing.

#### 2. `BranchTargetBuffer.h` - LRU Cache Implementation

**Purpose**: Implements hardware-accurate BTB with LRU replacement

```cpp
struct BTBEntry {
    int sourceAddr_tag;   // Branch location (cache key)
    int targetAddr_data;  // Jump destination (cached value)
    BTBEntry* prev, *next; // Doubly-linked list pointers
};
```

**Cache Structure**:

```
[HEAD] <-> [Entry1] <-> [Entry2] <-> [Entry3] <-> [TAIL]
   ↑                                              ↑
  MRU                                            LRU
```

**Key Methods**:

- `getTargetAddress(sourceAddr)`: Look up target in cache, return -1 if miss
- `insert(sourceAddr, targetAddr)`: Add new entry, evict LRU if full
- `moveToFront()`: Make an entry Most Recently Used (MRU)

#### 3. `BranchPredictor.h` - Static Prediction Interface

**Purpose**: Defines static prediction strategy interface

**Prediction Strategy**: "Predict taken if the branch is in the BTB"

**Key Methods**:

- `predictTaken(sourceAddr)`: Returns true if sourceAddr is found in BTB
- `predictTargetAddress()`: Get cached target from BTB
- `update()`: Add taken branches to BTB
- `simulateTrace()`: Process entire instruction trace

**Statistics Tracked**:

- `btbHits/btbMisses`: Cache performance
- `predictionHits/predictionMisses`: Prediction accuracy
- `btbHitButMispredicted`: BTB found entry but wrong prediction

#### 4. `TwoBitBranchPredictor.h` - Dynamic Prediction Interface

**Purpose**: Defines advanced prediction with state machine

**Prediction Strategy**: Uses 4-state finite state machine per branch

```
States: STRONGLY_NOT_TAKEN → WEAKLY_NOT_TAKEN → WEAKLY_TAKEN → STRONGLY_TAKEN
        (predict not taken)                     (predict taken)
```

**Key Components**:

- `stateTable[]`: Array of prediction states indexed by branch address
- Two prediction methods: static (BTB-based) + dynamic (state machine)
- Compares both strategies' performance

### Implementation Files (.cpp)

#### 5. `TraceReader.cpp` - File Parsing Logic

**Purpose**: Converts text trace files into structured data

**Example trace line**:

```
B 0040124c from 004011a0 to 00401260 F not taken
```

**Parsing Process**:

1. Extract instruction type (`B`)
2. Find source address after "from " (`004011a0`)
3. Find target address after "to " (`00401260`)
4. Extract direction (`F`)
5. Determine if taken by searching for "taken" vs "not taken"

**Key Function**: `parseLine()` uses string parsing and regex to extract structured data from trace text.

#### 6. `BranchTargetBuffer.cpp` - LRU Cache Logic

**Purpose**: Implements hardware cache behavior with perfect LRU replacement

**Cache Operations**:

**Cache Hit**:

```cpp
// Found entry, move to front (make MRU)
moveToFront(found_entry);
return found_entry->targetAddr_data;
```

**Cache Miss + Insert**:

```cpp
// Take LRU entry (tail->prev), update it, move to front
BTBEntry* lru_node = tail->prev;
lru_node->sourceAddr_tag = newSourceAddr;
lru_node->targetAddr_data = newTargetAddr;
moveToFront(lru_node);
```

**Doubly-Linked List Management**:

- `addToFront()`: Insert new node after head
- `removeNode()`: Unlink node from list
- `moveToFront()`: Remove + add (for LRU updates)

#### 7. `BranchPredictor.cpp` - Static Prediction Algorithm

**Purpose**: Implements baseline static prediction strategy

**Core Algorithm**:

```cpp
bool predictTaken(int sourceAddr) {
    // Predict taken if branch is in BTB
    return (btb->getTargetAddress(sourceAddr) != -1);
}
```

**Simulation Loop**: For each instruction:

1. Check if branch is in BTB (`getTargetAddress()`)
2. Make prediction based on BTB presence
3. Compare prediction with actual outcome
4. Update statistics
5. If branch was taken, add to BTB (`update()`)

**Key Insight**: This strategy caches "taken" branches and predicts future behavior based on past behavior.

#### 8. `TwoBitBranchPredictor.cpp` - Dynamic Prediction Algorithm

**Purpose**: Implements advanced adaptive prediction with comparison

**Dual Prediction System**:

1. **Static**: Same as BranchPredictor (BTB-based)
2. **Dynamic**: 2-bit saturating counter per branch

**State Machine Logic**:

```cpp
bool dynamicPredict(int sourceAddr) {
    int index = getStateIndex(sourceAddr);  // Hash address to index
    return (stateTable[index] >= WEAKLY_TAKEN);  // Predict taken for states 2,3
}
```

**State Updates**:

```cpp
if (branch_actually_taken) {
    state = min(state + 1, STRONGLY_TAKEN);     // Move toward "taken"
} else {
    state = max(state - 1, STRONGLY_NOT_TAKEN); // Move toward "not taken"
}
```

**Data Structure**: Uses simple array with modulo hashing: `index = sourceAddr % stateTableSize`

#### 9. `main.cpp` - Static Predictor Entry Point

**Purpose**: Command-line interface for static predictor

**Usage**: `./branch_sim <trace_file> <btb_size>`

**Process**:

1. Parse command line arguments
2. Create BranchPredictor with specified BTB size
3. Run simulation on trace file
4. Print statistics

#### 10. `TwoBitPredictorMain.cpp` - Dynamic Predictor Entry Point

**Purpose**: Command-line interface for dynamic predictor

**Usage**: `./branch_sim_TwoBit <trace_file> <btb_size>`

**Process**:

1. Parse command line arguments
2. Create TwoBitBranchPredictor with specified BTB size
3. Run simulation (both static and dynamic predictions)
4. Print comparative statistics

### Python Analysis Files

#### 11. `plot_btb_overhead.py` - Static Predictor Analysis

**Purpose**: Automate testing across different BTB sizes and measure overhead

**Process**:

1. **Batch Simulation**: Run C++ simulator with BTB sizes [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 5096, 8000, 10000]
2. **Parse Output**: Extract statistics using regex
3. **Calculate Overhead**: `overhead = (instructions_fetched / instructions_executed)`
4. **Generate Plots**: BTB size vs hit rate, BTB size vs overhead

**Key Functions**:

- `run_simulation(btb_size)`: Execute C++ simulator via subprocess
- `parse_results(output)`: Extract metrics using regex patterns
- Matplotlib plotting for performance visualization

#### 12. `plot_btb_overheadTwoBit.py` - Dynamic Predictor Analysis

**Purpose**: Same automation but for two-bit predictor

**Additional Analysis**:

- Compares static vs dynamic accuracy
- Shows improvement from adaptive prediction
- Generates side-by-side performance plots

**Key Metrics**: Static accuracy, dynamic accuracy, improvement percentage, BTB hit rate

#### 13. `twobit_visualise_results.py` - Accuracy Visualization

**Purpose**: Focus specifically on prediction accuracy trends

**Key Outputs**:

- Static vs Dynamic accuracy comparison
- Improvement percentage over different BTB sizes
- Visualization of learning effectiveness

**Process**: Similar automation but focused on accuracy metrics rather than overhead analysis

---

## The Critical Role of Python

Python serves as the **data science and visualization engine** for this project, transforming raw simulation data into actionable insights:

### 1. Automation and Batch Processing

```python
# Example: Running multiple simulations automatically
BTB_SIZES = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 5096, 8000, 10000]
for size in BTB_SIZES:
    result = subprocess.run([SIMULATOR_PATH, TRACE_FILE, str(size)])
```

Python automates the execution of hundreds of simulations across different configurations, enabling:

- **Systematic Analysis**: Test all BTB sizes without manual intervention
- **Reproducible Research**: Consistent methodology across experiments
- **Time Efficiency**: Complete analysis in minutes instead of hours

### 2. Data Processing and Extraction

```python
# Parse simulation output for analysis
total_instructions = int(re.search(r"Total instructions processed: (\d+)", output).group(1))
btb_hits = int(re.search(r"BTB hits: (\d+)", output).group(1))
accuracy = (btb_hits / total_instructions) * 100
```

Python's regex and string processing capabilities extract performance metrics from C++ simulator output, enabling:

- **Structured Data Collection**: Convert text output to numerical datasets
- **Error Handling**: Robust parsing even with varying output formats
- **Statistical Analysis**: Calculate derived metrics like efficiency ratios

### 3. Professional Visualization

```python
plt.plot(btb_sizes, hit_rates, 'o-', color='blue', label='BTB Hit Rate')
plt.xlabel('BTB Size (entries)')
plt.ylabel('Hit Rate (%)')
plt.title('BTB Performance Analysis')
```

Python's matplotlib creates publication-quality graphs that reveal:

- **Performance Trends**: How BTB size affects hit rates
- **Optimization Points**: Identify optimal cache sizes
- **Comparative Analysis**: Static vs. dynamic predictor performance
- **Visual Insights**: Patterns invisible in raw numbers

### 4. Research and Development Support

The Python scripts enable:

- **Hypothesis Testing**: Quickly validate theoretical predictions
- **Algorithm Comparison**: Side-by-side performance evaluation
- **Parameter Optimization**: Find best configurations for specific workloads
- **Documentation**: Generate graphs for reports and presentations

## Technical Innovation

### Hybrid Architecture Benefits

This C++/Python hybrid approach provides:

- **Performance**: C++ handles computationally intensive simulation
- **Flexibility**: Python enables rapid analysis and visualization
- **Modularity**: Independent simulation and analysis components
- **Extensibility**: Easy to add new predictors or analysis methods

### Real-World Relevance

The project models actual processor components:

- **Industry Standard**: BTB and prediction algorithms used in Intel, AMD, ARM processors
- **Performance Impact**: Branch mispredictions cost 10-20 CPU cycles
- **Design Trade-offs**: Cache size vs. power consumption vs. accuracy

## Educational and Research Value

This project serves multiple purposes:

### Computer Architecture Education

- Demonstrates fundamental processor optimization techniques
- Provides hands-on experience with performance analysis
- Illustrates hardware/software co-design principles

### Research Applications

- Baseline for developing new prediction algorithms
- Framework for analyzing workload-specific optimizations
- Tool for studying processor design trade-offs

### Industry Relevance

- Techniques applicable to embedded systems design
- Performance analysis methodologies used in semiconductor industry
- Understanding critical for systems programming and optimization

## Conclusion

This project successfully bridges **theoretical computer architecture** with **practical data science**, using Python's analytical power to unlock insights from C++ simulation data. The combination demonstrates how modern computational research requires both efficient implementation and sophisticated analysis tools to drive meaningful discoveries in processor design and optimization.

The Python component is not just a visualization add-on—it's an integral part of the research methodology that transforms raw performance data into engineering insights that could influence real processor design decisions.
