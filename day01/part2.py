import argparse


DIAL_SIZE = 100
START_POSITION = 50


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("file_path")
    return parser.parse_args()


def main(filename: str):
    dial_position = START_POSITION
    zero_counter = 0

    with open(filename) as f:
        
        for line in f:
            direction = line[0]
            move = int(line[1:])

            num_zero_passes = 0

            prev_dial_position = dial_position

            if direction == 'R':
                dial_position += move

            if direction == 'L':
                dial_position -= move

            rotation_prev = prev_dial_position // 100
            rotation = dial_position // 100

            num_zero_passes += abs(rotation_prev - rotation)

            dial_position %= 100
            if direction == 'L' and dial_position == 0:
                num_zero_passes += 1

            if direction == 'L' and prev_dial_position == 0:
                num_zero_passes -= 1

            zero_counter += num_zero_passes

    print(f"Answer to part 2: {zero_counter}")


if __name__ == "__main__":
    args = parse_args()
    main(args.file_path)
