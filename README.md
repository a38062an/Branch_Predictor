# Branch_Predictor

## Things to take into account

- Do we cache the branch?
- When is a branch taken (our policy)
- Do we cache only unconditional branches?
- Do  we only cache forward branches?
- Do we cache both forward and backward branches?
- What BTB entries do we replace?

## Decisions to make

- The branch is cached if its taken.
- The branch will be predicted taken if its in the cache.
- We will use a Least recently used policy for our cache replacement algorithm
- We will program the branch predictor in C++. 

## Specific implementation logic
- For the BTB we will use a doubly linked list

# How to run the program

- To run the branch prediction for a given size this is how you do it: (where 10 is the size)
```bash
g++ -o branch_sim main.cpp BranchPredictor.cpp BranchTargetBuffer.cpp TraceReader.cpp
./branch_sim ../misc/block_profile 10
```
- To run and plot the results of the branch prediction for varying sizes this is how you do it:
```bash
./plot_btb_overhead.py
```
- To run the two bit branch prediction for a given size:
```bash
g++ -o branch_sim_TwoBit TwoBitPredictorMain.cpp TwoBitBranchPredictor.cpp BranchTargetBuffer.cpp TraceReader.cpp
./branch_sim_TwoBit ../misc/block_profile 10
```