import argparse
import numpy as np


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("file_path")
    return parser.parse_args()


def calculate_column(column):
    nums = (column[:-1]).astype(np.int64)
    operator = column[-1]

    if operator == '+':
        return np.sum(nums)
    
    if operator == '*':
        return np.prod(nums)
    

def main():
    args = parse_args()

    grid = []

    with open(args.file_path) as f:        
        for line in f:
            row = line.split()
            grid.append(row)

    grid = np.array(grid)
    
    total = 0
    
    for i in range(len(grid[0])):
        total += calculate_column(grid[:, i])

    print(f"Answer to part 1: {total}")

    
if __name__ == "__main__":
    main()