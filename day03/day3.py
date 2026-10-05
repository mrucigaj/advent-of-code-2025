import argparse


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("file_path")
    return parser.parse_args()


def find_highest_joltage(line: str, n_batteries: int) -> int:
    joltage = 0
    index = -1

    for i in range(n_batteries - 1, 0, -1):
        line_to_search = line[index + 1:-i]
        index += line_to_search.index(max(line_to_search)) + 1

        joltage += int(line[index]) * 10 ** i

    line_to_search = line[index + 1:]
    index += line_to_search.index(max(line_to_search)) + 1
    
    joltage += int(line[index])

    return joltage


def calc_total_joltage(file_path: str, n_batteries: int) -> int:
    total = 0

    with open(file_path) as f:        
        for line in f:
            total += find_highest_joltage(line.rstrip("\n"), n_batteries)

    return total


def main():
    args = parse_args()

    joltage_1 = calc_total_joltage(args.file_path, 2)
    joltage_2 = calc_total_joltage(args.file_path, 12)

    print(f"Answer for part 1: {joltage_1}")
    print(f"Answer for part 2: {joltage_2}")


if __name__ == "__main__":
    main()