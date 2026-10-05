# Advent of Code 2025

My solutions for all 12 days of [Advent of Code 2025](https://adventofcode.com/2025).

Run the commands below from the corresponding day's folder, using your own puzzle input.

## Days 1–7: Python

The solutions for two parts of each day are either separate (`part1.py`, `part2.py`) or together in one file (eg. `day3.py`). Run them using `python` command:

```sh
python part1.py input/input.txt
python part2.py input/input.txt
```

```sh
python day3.py input/input.txt
```

Day 6 requires NumPy (`python -m pip install numpy`).

## Days 8–12: C++

Use a compiler that supports C++20. Compile and run the programs using:

```sh
mkdir build
g++ -std=c++20 part1.cpp -o build/part1
./build/part1 input/input.txt
```

Replace `part1` with `part2` or the appropriate file name where the solution for both parts is in a single file (eg. `day11.cpp`).

### Day 10 part 2: CMake

Part 2 uses the HiGHS integer optimization solver. CMake downloads and builds HiGHS, then links it to the solution. Git and internet access are required for the first build. From the day 10 folder:

```sh
cmake -S . -B build
cmake --build build --config Release
./build/day10_part2 input/input.txt
```
