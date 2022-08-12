# Crypto Liquidity Pool Simple Arbitrage

## Project Requirements

- [X] Liquidity Pools in src/Pool.hxx and Pool.cxx
- [X] Simple Arbitrage Logic defined in src/ArbitrageStrategy.cxx
- [X] Used GTest to add test cases for various scenarios
- [X] Used Google Benchmark to benchmark async calculation and synchronous calculation

## Usage

### Requirements

- CMake v3.23
- Clang v13.0
- C++20 std

### Building the project

#### Using the build script

```bash
chmod u+x build.sh && ./build.sh
```

#### Using CLI

```bash
# remove existing build, make build directory and cd inside
rm -rf build && mkdir build && cd build

# init cmake from CMakeLists in project root
cmake -DCMAKEBUILD_TYPE=Relase ..

# build
cmake --build .
```

## Running the project

### Run Main

Main program takes in user input to set the X and Y values of the two pools and calculates the amount of eth required and profit to make the arbitrage.

```bash
❯ ./build/bin/Main
Enter the ETH quantity for Pool A (X_1):
89 
Enter the DAI quantity for Pool A (Y_1):
536375
Enter the ETH quantity for Pool B (X_2):
45
Enter the DAI quantity for Pool B (Y_2):
346361
Calculating async call
You will need: 7.57786 ETH; and you will profit: 0.000272033
```

### Running tests

```bash
./build/bin/TestMain
```

### Running Benchmarks

```bash
./build/bin/ArbitrageStrategyBenchmark
```

# Project Breakdown

## Problem Statement

### Given
- Two Uniswap Pools, A and B
- Constant Product Formula: 
  - `X_1 * Y_1 = K_1`
  - `X_2 * Y_2 = K_2`

### Constraints
- Swap fee 0.3%
  - added to reserves

### Requirements
- Define data structs of pool status
  - add / remove / swap 
  - maintains status data in-memory
- Define arbitrage calculation logic
  - trigger calc in NON-BLOCKING mode
  - find how many ETH we need for an arbitrage calculation
  - find out profit from trade
  
### Testing

- Simulate user's swap transactions (e.g. 100 DAI to ETH in Pool A)
  - Testing using GTest
- Benchmark non-blocking calculation 
  - Benchmarking using Google Benchmark to run benchmark both synchronously and asynchronously

## System Interfaces

### Pool
status()
  - shows current token quantities and k
add()
  - adds token to pool
remove()
  - removes token from pool
swap()
  - swaps one token for another in a pool
  - fee of 0.3% applied

### Strategy
calculate_async()
  - return ETH required & potential profit

## Data Models

### Token
type: TokenType -> ETH / DAI / LP