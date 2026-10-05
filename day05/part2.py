import argparse


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("file_path")
    return parser.parse_args()


def simplify_ranges(ranges):
    simplified = []

    for r in ranges:
        lower = r[0]
        upper = r[1]

        lower_in = -1
        upper_in = -1

        # Remove intervals that are completely contained in the one we are adding.
        to_remove = []
        for i, s in enumerate(simplified):
            if lower <= s[0] and s[1] <= upper:
                to_remove.append(i)
        
        for i in reversed(to_remove):
            del simplified[i]

        # Find in which interval are new borders contained.
        for i, s in enumerate(simplified):
            if lower >= s[0] and lower <= s[1]:
                lower_in = i
            if upper >= s[0] and upper <= s[1]:
                upper_in = i
            if lower_in != -1 and upper_in != -1:
                break

        # If both contained in the same existing interval.
        if lower_in != -1 and lower_in == upper_in:
            continue

        # Combined options.
        if lower_in == -1:
            if upper_in == -1:
                simplified.append(r)

            else:
                simplified[upper_in][0] = lower

        else:
            if upper_in == -1:
                simplified[lower_in][1] = upper

            else:
                simplified[lower_in] = [simplified[lower_in][0], simplified[upper_in][1]]
                del simplified[upper_in]

        simplified.sort()

    return simplified


def count_simple_ranges_size(ranges):
    counter = 0
    for r in ranges:
        counter += r[1] - r[0] + 1

    return counter


def main():
    args = parse_args()

    ranges = []

    with open(args.file_path) as f:        
        line = f.readline()

        while line != "\n":
            left, right = line.split("-")
            ranges.append([int(left), int(right)])
            line = f.readline()

    ranges = simplify_ranges(ranges)
    counter = count_simple_ranges_size(ranges)

    print(f"Answer to part 2: {counter}")


if __name__ == "__main__":
    main()