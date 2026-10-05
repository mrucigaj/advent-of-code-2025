#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

#include "Highs.h"


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
        std::uint32_t num = 0;
        bool entry = false;
        for (std::uint32_t i = line.find('{') + 1; i < line.size(); ++i) {
            char c = line[i];
            if (std::isdigit(c)) {
                num *= 10;
                num += c - '0';
                entry = true;
                continue;
            } 
            if (entry) {
                joltages.push_back(num);
                num = 0;
                entry = false;
            }
        }

        problems.push_back({goalState, buttons, joltages});
    }

    return problems;
}

std::uint32_t findOptimalSolution(Problem const &p)
{
    HighsModel model;

    model.lp_.num_col_ = p.buttons.size();
    model.lp_.num_row_ = p.joltages.size();
    model.lp_.sense_ = ObjSense::kMinimize;
    model.lp_.offset_ = 0;
    model.lp_.col_cost_ = std::vector<double>(p.buttons.size(), 1.0);
    model.lp_.col_lower_ = std::vector<double>(p.buttons.size(), 0.0);
    model.lp_.col_upper_ = std::vector<double>(p.buttons.size(), 1.0e30);;
    model.lp_.row_lower_.assign(p.joltages.begin(), p.joltages.end());
    model.lp_.row_upper_.assign(p.joltages.begin(), p.joltages.end());
    
    model.lp_.a_matrix_.format_ = MatrixFormat::kColwise;

    std::vector<std::size_t> colStarts;
    std::vector<std::size_t> rowIndices;
    std::vector<double> values;

    std::uint32_t idx = 0u;
    for (std::uint32_t i = 0; i < p.buttons.size(); ++i) {
        colStarts.push_back(idx);
        for (auto b : p.buttons[i]) {
            rowIndices.push_back(b);
            values.push_back(1.0);
            ++idx;
        }
    }
    colStarts.push_back(idx);

    model.lp_.a_matrix_.start_.assign(colStarts.begin(), colStarts.end());
    model.lp_.a_matrix_.index_.assign(rowIndices.begin(), rowIndices.end());
    model.lp_.a_matrix_.value_ = values;

    // Create a Highs instance
    Highs highs;
    HighsStatus return_status;

    model.lp_.integrality_.resize(model.lp_.num_col_);
    for (int col=0; col < model.lp_.num_col_; col++)
        model.lp_.integrality_[col] = HighsVarType::kInteger;
    
    // Pass the model to HiGHS
    return_status = highs.passModel(model);
    assert(return_status==HighsStatus::kOk);
    
    // Get a const reference to the LP data in HiGHS
    const HighsLp& lp = highs.getLp();
    
    // Solve the model
    return_status = highs.run();
    assert(return_status==HighsStatus::kOk);
    
    // Get the model status
    const HighsModelStatus& model_status = highs.getModelStatus();
    assert(model_status==HighsModelStatus::kOptimal);

    const HighsInfo& info = highs.getInfo();

    return info.objective_function_value;
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

    std::cout << "Answer to part 2: " << sumOptimalSolutions << std::endl;
    
    return 0;
}