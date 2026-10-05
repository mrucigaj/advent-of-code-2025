#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Tile {
    std::int64_t x;
    std::int64_t y;
    
    // Directions: whether a corner opens into that direction.
    bool upLeft;
    bool upRight;
    bool downLeft;
    bool downRight;
};

enum class EdgeDirection {
    Up,
    Down,
    Left,
    Right
};

enum class PositionOnInterval {
    Left,
    Inside,
    Right
};

void setTileInsideDirection(Tile &previous, Tile &current, Tile &next)
{
    using enum EdgeDirection;

    EdgeDirection first;
    EdgeDirection second;

    first = previous.x == current.x ?
        (previous.y < current.y ? Up : Down) :
        (previous.x < current.x ? Right : Left);

    second = current.x == next.x ? 
        (current.y < next.y ? Up : Down) :
        (current.x < next.x ? Right : Left);

    if (first == Up) {
        if (second == Left) {
            current.downLeft = true;
            return;
        }  
        if (second == Right) {
            current.downLeft = true;
            current.upLeft = true;
            current.upRight = true;
            return;
        }
    }
    if (first == Down) {
        if (second == Left) {
            current.downLeft = true;
            current.downRight = true;
            current.upRight = true;
            return;
        }  
        if (second == Right) {
            current.upRight = true;
            return;
        }
    }
    if (first == Left) {
        if (second == Up) {
            current.downLeft = true;
            current.downRight = true;
            current.upLeft = true;
            return;
        }  
        if (second == Down) {
            current.downRight = true;
            return;
        }
    }
    if (first == Right) {
        if (second == Up) {
            current.upLeft = true;
            return;
        }  
        if (second == Down) {
            current.downRight = true;
            current.upLeft = true;
            current.upRight = true;
            return;
        }
    }

    assert(false);
}

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

void setupInput(std::vector<Tile> &redTiles)
{
    std::size_t numTiles = redTiles.size();

    for (std::uint32_t i = 1; i < numTiles - 1; ++i) {
        setTileInsideDirection(redTiles[i - 1], redTiles[i], redTiles[i + 1]);
    }

    setTileInsideDirection(redTiles[numTiles - 1], redTiles[0], redTiles[1]);
    setTileInsideDirection(redTiles[numTiles - 2], redTiles[numTiles - 1], redTiles[0]);
}

PositionOnInterval getPositionOnInterval(std::int64_t intervalEndpoint1,
    std::int64_t intervalEndpoint2, std::uint64_t point)
{
    std::int64_t minEndpoint = std::min(intervalEndpoint1, intervalEndpoint2);
    std::int64_t maxEndpoint = std::max(intervalEndpoint1, intervalEndpoint2);

    if (point <= minEndpoint) {
        return PositionOnInterval::Left;
    }

    if (point >= maxEndpoint) {
        return PositionOnInterval::Right;
    }

    return PositionOnInterval::Inside;
}

bool edgeCrossesRectangle(Tile const &corner1, Tile const &corner2,
    Tile const &edgeEndpoint1, Tile const &edgeEndpoint2)
{
    using enum PositionOnInterval;

    if (edgeEndpoint1.x == edgeEndpoint2.x) {
        if (getPositionOnInterval(corner1.x, corner2.x, edgeEndpoint1.x) != Inside) {
            return false;
        }

        PositionOnInterval pos1 = getPositionOnInterval(corner1.y, corner2.y, edgeEndpoint1.y);
        PositionOnInterval pos2 = getPositionOnInterval(corner1.y, corner2.y, edgeEndpoint2.y);
        
        return pos1 == Inside || pos1 != pos2;
    }

    if (edgeEndpoint1.y == edgeEndpoint2.y) {
        if (getPositionOnInterval(corner1.y, corner2.y, edgeEndpoint1.y) != Inside) {
            return false;
        }

        PositionOnInterval pos1 = getPositionOnInterval(corner1.x, corner2.x, edgeEndpoint1.x);
        PositionOnInterval pos2 = getPositionOnInterval(corner1.x, corner2.x, edgeEndpoint2.x);
        
        return pos1 == Inside || pos1 != pos2;
    }

    assert(false);
}

std::uint64_t findMaxAreaInside(std::vector<Tile> const &redTiles) {
    std::uint64_t maxArea = 0;

    for (std::uint32_t i = 0; i < redTiles.size(); ++i) {
        auto const &r1 = redTiles[i];
        
        for (std::uint32_t j = 0; j < i; ++j) {
            auto const &r2 = redTiles[j];

            if (!((r1.downLeft && r2.upRight) || (r1.downRight && r2.upLeft) ||
                (r1.upLeft && r2.downRight) || (r1.upRight && r2.downLeft))) {                
                continue;
            }

            std::uint64_t currentArea =
                (std::abs(r1.x - r2.x) + 1) *
                (std::abs(r1.y - r2.y) + 1);

            if (currentArea > maxArea) {             
                bool areaInside =
                    !edgeCrossesRectangle(r1, r2, redTiles[0], redTiles[redTiles.size() - 1]);

                for (std::uint32_t k = 0; k < redTiles.size() - 1; ++k) {
                    if (edgeCrossesRectangle(r1, r2, redTiles[k], redTiles[k + 1])) {
                        areaInside = false;
                        break;
                    }
                }                               

                if (areaInside) {
                    maxArea = currentArea;
                }
            }
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

    setupInput(redTiles);

    std::cout << "Solution to part 2: " << findMaxAreaInside(redTiles) << std::endl;
    
    return 0;
}