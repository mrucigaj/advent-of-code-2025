#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>


using Present = std::array<bool, 9>;

struct Problem {
    std::uint32_t width;
    std::uint32_t length;
    std::vector<std::uint32_t> desiredShapeCounts;
};

struct ProblemSet {
    std::vector<Present> presents;
    std::vector<Problem> problems;
};

struct SolvabilityReducion {
    std::uint32_t solvable;
    std::uint32_t unsolvable;
    std::uint32_t ambiguous;
};


ProblemSet readInput(std::string const &filename)
{
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error reading file: " + filename);
    }
    
    std::string line;
    std::vector<Present> presents;
    std::vector<Problem> problems;

    while (std::getline(file, line)) {
        if (line.find('x') == std::string::npos) {
            Present currentPresent;
            char c;

            for (std::uint32_t i = 0; i < 3; ++i) {
                std::getline(file, line);
                std::istringstream iss(line);
                for (std::uint32_t j = 0; j < 3; ++j) {
                    iss >> c;
                    currentPresent[3 * i + j] = c == '#';
                }
            }

            presents.push_back(currentPresent);

            std::getline(file, line); // Empty line.
        } else {
            Problem problem;

            std::istringstream iss(line);
            char x, colon;
            std::uint32_t s;

            iss >> problem.width;
            iss >> x;
            iss >> problem.length;
            iss >> colon;

            for (std::size_t i = 0; i < presents.size(); ++i) {
                iss >> s;
                problem.desiredShapeCounts.push_back(s);
            }

            problems.push_back(problem);

        }
    }

    return {presents, problems};
}

std::uint32_t countFilled(Present const &present) {
    return std::accumulate(present.begin(), present.end(), 0u);
}

SolvabilityReducion simpleCheck(ProblemSet const &problemSet)
{
    std::uint32_t countSolvable = 0u;
    std::uint32_t countUnsolvable = 0u;

    for (auto problem : problemSet.problems) {
        std::uint32_t countMinCells = 0;        
        for (std::uint32_t i = 0; i < problem.desiredShapeCounts.size(); ++i) {
            countMinCells +=
                countFilled(problemSet.presents[i]) * problem.desiredShapeCounts[i];
        }
        if (countMinCells > problem.length * problem.width) {
            ++countUnsolvable;
        }

        std::uint32_t countShapes = std::accumulate(
            problem.desiredShapeCounts.begin(), problem.desiredShapeCounts.end(), 0u);
        std::uint32_t available3x3s = (problem.width / 3) * (problem.length / 3);
        if (countShapes <= available3x3s) {
            ++countSolvable;
        }
    }

    std::uint32_t countAmbigous = problemSet.problems.size() - countSolvable - countUnsolvable;

    return {countSolvable, countUnsolvable, countAmbigous};
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
        return 1;
    }

    ProblemSet problemSet;
    try {
        problemSet = readInput(argv[1]);
    } catch (std::runtime_error const &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    auto simpleSolutionSpread = simpleCheck(problemSet);

    if (simpleSolutionSpread.ambiguous == 0) {
        std::cout << "Solution to part 1: " << simpleSolutionSpread.solvable << std::endl;
    } else {
        std::cout << "Solution in interval: [" << simpleSolutionSpread.solvable << ", " <<
        simpleSolutionSpread.solvable + simpleSolutionSpread.ambiguous << "]" << std::endl;
    }

    return 0;
}