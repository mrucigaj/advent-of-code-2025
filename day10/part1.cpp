#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


struct Problem {
    std::vector<bool> goalState;
    std::vector<std::vector<std::uint32_t>> buttons;
    std::vector<std::uint32_t> joltages;
};


std::vector<Problem> readInput(std::string const &filename)
{
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error reading file: " + filename);
    }
    
    std::string line;
    std::vector<Problem> problems;

    while (std::getline(file, line)) {
        std::vector<bool> goalState;
        std::vector<std::vector<std::uint32_t>> buttons;
        std::vector<std::uint32_t> joltages;

        // Lights.
        for (std::uint32_t i = 1; i < line.find(']'); ++i) {
            goalState.push_back(line[i] == '#');
        }

        // Buttons.
        std::vector<std::uint32_t> button;
        for (std::uint32_t i = line.find(']') + 1; i < line.find('{'); ++i) {
            char c = line[i];

            if (c == ')') {
                buttons.push_back(button);
                button.clear();
                continue;
            }

            if (std::isdigit(c)) {
                button.push_back(c - '0');
            }
        }

        // Joltages.
        for (std::uint32_t i = line.find('{') + 1; i < line.size(); ++i) {
            char c = line[i];
            if (std::isdigit(c)) {
                joltages.push_back(c - '0');
            }
        }

        problems.push_back({goalState, buttons, joltages});
    }

    return problems;
}

void getCombinations(std::uint32_t maxIdx, std::uint32_t combSize, std::uint32_t start,
    std::vector<std::uint32_t> &current, std::vector<std::vector<std::uint32_t>> &result)
{
    if (current.size() >= combSize) {
        result.push_back(current);
        return;
    }

    for (std::uint32_t i = start; i < maxIdx; ++i) {
        current.push_back(i);
        getCombinations(maxIdx, combSize, i + 1, current, result);
        current.pop_back();
    }
}

void applyButton(std::vector<bool> &state, std::vector<std::uint32_t> const &button)
{
    for (auto b : button) {
        state[b] = !state[b];
    }
}

std::uint32_t findOptimalSolution(Problem const &problem)
{
    std::uint32_t problemSize = problem.goalState.size();
    
    for (std::uint32_t numClicks = 0; numClicks <= problem.buttons.size(); ++numClicks) {
        std::vector<std::vector<std::uint32_t>> combinations;
        std::vector<std::uint32_t> current;
        getCombinations(problem.buttons.size(), numClicks, 0, current, combinations);

        for (auto &c : combinations) {
            std::vector<bool> state(problemSize, false);

            for (auto idx : c) {
                applyButton(state, problem.buttons[idx]);
            }

            if (state == problem.goalState) {
                return numClicks;
            }
        }
    }

    assert(false);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
        return 1;
    }

    std::vector<Problem> problems;
    try {
        problems = readInput(argv[1]);
    } catch (std::runtime_error const &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::uint32_t sumOptimalSolutions = 0;
    for (auto const &p : problems) {
        sumOptimalSolutions += findOptimalSolution(p);
    }

    std::cout << "Answer to part 1: " << sumOptimalSolutions << std::endl;
    
    return 0;
}