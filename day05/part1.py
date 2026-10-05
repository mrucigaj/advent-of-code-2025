import argparse


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("file_path")
    return parser.parse_args()


def is_in_ranges(ranges, number):
    for r in ranges:
        if number >= r[0] and number <= r[1]:
            return True
    return False


def main():
    args = parse_args()

    ranges = []

    with open(args.file_path) as f:        
        line = f.readline()

        while line != "\n":
            left, right = line.split("-")
            ranges.append((int(left), int(right)))
            line = f.readline()

        counter = 0
        line = f.readline()        

        for line in f:
            if is_in_ranges(ranges, int(line)):
                counter += 1

    print(f"Answer to part 1: {counter}")

    
if __name__ == "__main__":
    main()