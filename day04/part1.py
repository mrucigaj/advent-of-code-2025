import argparse


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("file_path")
    return parser.parse_args()


def check_grid(grid):
    counter = 0

    # inside
    for i in range(1, len(grid) - 1):
        for j in range(1, len(grid[i]) - 1):
            if grid[i][j] == '@':
                neighbours = [
                    grid[i][j - 1],
                    grid[i][j + 1],
                    grid[i - 1][j],
                    grid[i + 1][j],
                    grid[i - 1][j - 1],
                    grid[i - 1][j + 1],
                    grid[i + 1][j - 1],
                    grid[i + 1][j + 1]
                ]
                if neighbours.count('@') < 4:
                    counter += 1

    # left and right columns
    for i in range(1, len(grid) - 1):
        if grid[i][0] == '@':
            neighbours = [
                grid[i][1],
                grid[i - 1][0],
                grid[i + 1][0],
                grid[i - 1][1],
                grid[i + 1][1]
            ]
            if neighbours.count('@') < 4:
                counter += 1
        if grid[i][-1] == '@':
            neighbours = [
                grid[i][-2],
                grid[i - 1][-1],
                grid[i + 1][-1],
                grid[i - 1][-2],
                grid[i + 1][-2]
            ]
            if neighbours.count('@') < 4:
                counter += 1
   
    # top and bottom rows
    for j in range(1, len(grid) - 1):
        if grid[0][j] == '@':
            neighbours = [
                grid[0][j - 1],
                grid[0][j + 1],
                grid[1][j - 1],
                grid[1][j],
                grid[1][j + 1]
            ]
            if neighbours.count('@') < 4:
                counter += 1
        if grid[-1][j] == '@':
            neighbours = [
                grid[-1][j - 1],
                grid[-1][j + 1],
                grid[-2][j - 1],
                grid[-2][j],
                grid[-2][j + 1]
            ]
            if neighbours.count('@') < 4:
                counter += 1

    corners = [
        grid[0][0],
        grid[0][-1],
        grid[-1][0],
        grid[-1][-1]
    ]
    counter += corners.count('@')

    return counter


def main():
    args = parse_args()

    grid = []

    with open(args.file_path) as f:        
        for line in f:
            row = list(line.rstrip("\n"))
            grid.append(row)

    print(f"Answer to part 1: {check_grid(grid)}")


if __name__ == "__main__":
    main()
