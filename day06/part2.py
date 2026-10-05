import argparse
import numpy as np


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("file_path")
    return parser.parse_args()


def calculate_section(grid):
    operator = grid[-1, 0]
    nums = []

    for col in grid[:-1, :].T:
        nums.append(int(''.join(col)))

    if operator == '+':
        return int(np.sum(nums))
    
    if operator == '*':
        total = 1
        for num in nums:
            total *= num
        return total


def calculate_result(grid):
    n = len(grid[0])
    section_start = 0

    total = 0

    for i in range(n):
        if np.all(grid[:, i] == ' '):
            section_end = i
            current_grid = grid[:, section_start:section_end]
            total += calculate_section(current_grid)
            section_start = section_end + 1

    current_grid = grid[:, section_start:]
    total += calculate_section(current_grid)

    return total


def main():
    args = parse_args()
    
    grid = []

    with open(args.file_path) as f:        
        for line in f:
            row = list(line)[:-1]
            grid.append(row)

    grid = np.array(grid)
    
    print(f"Answer to part 2: {calculate_result(grid)}")

    
if __name__ == "__main__":
    main()