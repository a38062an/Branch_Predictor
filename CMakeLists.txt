cmake_minimum_required(VERSION 3.20)
project(Branch_Predictor LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
add_executable(Branch_Predictor
        src/BranchPredictor.cpp
        src/BranchTargetBuffer.cpp
        src/main.cpp
        src/TraceReader.cpp
        src/TwoBitBranchPredictor.cpp
        src/TwoBitPredictorMain.cpp
)
