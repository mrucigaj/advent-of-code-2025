#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Tile {
    std::int64_t x;
    std::int64_t y;
};

std::vector<Tile> readInput(std::string const &filename)
{
    std::vector<Tile> redTiles;
    std::int64_t a, b;
    char comma;

    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error reading file: " + filename);
    }

    while (file >> a >> comma >> b) {
        redTiles.push_back({a, b});        
    }

    return redTiles;
}

std::uint64_t findMaxAreaInside(std::vector<Tile> const &redTiles) {
    std::uint64_t maxArea = 0;

    for (std::uint32_t i = 0; i < redTiles.size(); ++i) {
        auto const &r1 = redTiles[i];

        for (std::uint32_t j = 0; j < i; ++j) {
            auto const &r2 = redTiles[j];

            std::uint64_t currentArea =
                (std::abs(r1.x - r2.x) + 1) *
                (std::abs(r1.y - r2.y) + 1);

            maxArea = std::max(maxArea, currentArea);
        }
    }

    return maxArea;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
        return 1;
    }

    std::vector<Tile> redTiles;
    try {
        redTiles = readInput(argv[1]);
    } catch (std::runtime_error const &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::cout << "Solution to part 1: " << findMaxAreaInside(redTiles) << std::endl;
    
    return 0;
}