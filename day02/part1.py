import argparse


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("file_path")
    return parser.parse_args()


def is_repeated(num: str) -> bool:
    n = len(num)
    n_half = n // 2

    if n % 2 == 1:
        return False
    
    for i in range(n_half):
        if num[i] != num[n_half + i]:
            return False
    
    return True


def check_and_sum_range(first: int, last: int):
    sum_repeated = 0

    for i in range(first, last + 1):
        if is_repeated(str(i)):
            sum_repeated += i

    return sum_repeated


def sum_invalid_ids(file_path):
    id_sum = 0

    with open(file_path) as f:        
        line = f.readline()

        for current_range in line.split(','):
            first, last = current_range.split('-')            
            id_sum += check_and_sum_range(int(first), int(last))

    return id_sum


def main():
    args = parse_args()
    id_sum = sum_invalid_ids(args.file_path)
    print(f"Answer to part 1: {id_sum}")


if __name__ == "__main__":
    main()