import argparse


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("file_path")
    return parser.parse_args()


def main():
    args = parse_args()

    with open(args.file_path) as f:        
        line = f.readline()
        width = len(line) - 1

        num_splits = 0
        previous_row = [0 if l == '.' else 1 for l in line.rstrip("\n")]

        for line in f:
            current_row = [0 if l == '.' else l for l in line.rstrip("\n")]

            for i in range(width):
                if previous_row[i] != '^':
                    if previous_row[i] > 0:
                        if current_row[i] == '^':
                            num_splits += 1
                            current_row[i - 1] += previous_row[i]
                            current_row[i + 1] += previous_row[i]
                        else:
                            current_row[i] += previous_row[i]

            previous_row = current_row

    print(f"Answer to part 1: {num_splits}")
    print(f"Answer to part 2: {sum([int(l) for l in current_row])}")

    
if __name__ == "__main__":
    main()
