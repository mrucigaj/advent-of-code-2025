#include <algorithm>
#include <cmath>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

struct Vertex {
    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t z;
};

struct Edge {   
    std::size_t vertex1;
    std::size_t vertex2;
    double length;
};


std::vector<Vertex> readInput(std::string const &filename)
{
    std::vector<Vertex> coordinates;
    std::uint32_t a, b, c;
    char comma;

    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error reading file.");
    }

    while (file >> a >> comma >> b >> comma >> c) {
        Vertex vertex = {a, b, c};
        coordinates.push_back(vertex);        
    }

    return coordinates;
}

std::vector<Edge> calcAndSortEdges(std::vector<Vertex> const &coordinates)
{
    std::vector<Edge> edges;
    for (std::size_t i = 0; i < coordinates.size(); ++i) {
        for (std::size_t j = 0; j < i; ++j) {
            std::int32_t diffX = coordinates[i].x - coordinates[j].x;
            std::int32_t diffY = coordinates[i].y - coordinates[j].y;
            std::int32_t diffZ = coordinates[i].z - coordinates[j].z;

            double cost = std::sqrt(
                std::pow(diffX, 2.0) + 
                std::pow(diffY, 2.0) + 
                std::pow(diffZ, 2.0)
            );
            
            edges.push_back({i, j, cost});
        }
    }

    std::ranges::sort(edges, {}, &Edge::length);

    return edges;
}

void connectClosestPair(
    std::vector<Edge> const &edges,
    std::vector<std::size_t> &components,
    std::uint32_t idxOffest)
{
    auto const &vertexIdx1 = edges[idxOffest].vertex1;
    auto const &vertexIdx2 = edges[idxOffest].vertex2;

    std::size_t minComponent = std::min(
        components[vertexIdx1],
        components[vertexIdx2]
    );
    std::size_t maxComponent = std::max(
        components[vertexIdx1],
        components[vertexIdx2]
    );

    for (auto &c : components) {
        if (c == maxComponent) {
            c = minComponent;
        }
    }

    return;
}

std::uint32_t multiplyComponentSizes(std::vector<std::size_t> &components)
{
    std::unordered_map<std::size_t, std::uint32_t> sizes;

    for (std::size_t component : components) {
        ++sizes[component];
    }

    std::vector<std::pair<int, int>> counts(sizes.begin(), sizes.end());

    std::ranges::sort(counts, [](auto const a, auto const b) {
        return a.second > b.second;
    });

    int sum = 0;
    for (int i = 0; i < counts.size(); ++i) {
        sum += counts[i].second;
    }

    // Part 1 requires mulitplying the sizes of three largest circuits.
    return counts[0].second * counts[1].second * counts[2].second;    
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
        return 1;
    }

    std::vector<Vertex> coordinates;
    try {
        coordinates = readInput(argv[1]);
    } catch (std::runtime_error const &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    auto sortedEdges = calcAndSortEdges(coordinates);

    std::vector<std::size_t> components(coordinates.size());
    std::iota(components.begin(), components.end(), 0);  // Fill `components` with 0, 1, 2, ...

    for (std::uint32_t i = 0; i < 1000; ++i) {
        connectClosestPair(sortedEdges, components, i);
    }

    std::cout << "Answer to part 1: " << multiplyComponentSizes(components) << std::endl;

    return 0;
}