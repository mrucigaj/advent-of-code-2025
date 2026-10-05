#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Node {
    std::string id;
    std::vector<std::string> outputIds;
    std::uint32_t countInTotal = 0u;

    std::uint32_t countIn = 0u;
    std::uint64_t pathCounter = 0u;
};

std::unordered_map<std::string, Node> readInput(std::string const &filename)
{
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error reading file: " + filename);
    }

    std::unordered_map<std::string, Node> idToNodeMap;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        
        std::string inNodeName;
        iss >> inNodeName;
        inNodeName = inNodeName.substr(0, 3);
        
        if (!idToNodeMap.contains(inNodeName)) {
            idToNodeMap.insert({inNodeName, {inNodeName, {}}});
        }

        std::string outNodeName;
        while (iss >> outNodeName) {
            if (!idToNodeMap.contains(outNodeName)) {
                idToNodeMap.insert({outNodeName, {outNodeName, {}}});
            }
            idToNodeMap[inNodeName].outputIds.push_back(outNodeName);
        }
    }
    
    return idToNodeMap;
}

std::unordered_set<std::string> reduceToReachableFrom(
    std::unordered_map<std::string, Node> &nodeMap, std::string const &from)
{
    std::unordered_set<std::string> reachable;
    std::unordered_set<std::string> checked;
    std::queue<std::string> toCheck;
    toCheck.push(from);

    while (!toCheck.empty()) {
        std::string &current = toCheck.front();
        
        reachable.insert(current);
        for (auto a : nodeMap[current].outputIds) {
            if (!checked.contains(a)) {
                toCheck.push(a);
                checked.insert(a);
            }
        }
        toCheck.pop();
    }

    return reachable;
}

void recalculateIn(std::unordered_map<std::string, Node> &nodeMap)
{
    for (auto &node : nodeMap) {
        node.second.countInTotal = 0;
    }
    for (auto &node : nodeMap) {
        for (auto const& nextNode : node.second.outputIds) {
            ++nodeMap[nextNode].countInTotal;
        }
    }
}

std::uint64_t propagateCount(std::unordered_map<std::string, Node> &fullNodeMap,
    std::string &startNode, std::string &endNode)
{

    std::unordered_set<std::string> reachableIds =
        reduceToReachableFrom(fullNodeMap, startNode);

    std::unordered_map<std::string, Node> nodeMap;
    for (auto const &id : reachableIds) {
        nodeMap.insert({id, fullNodeMap[id]});
    }

    recalculateIn(nodeMap);

    std::queue<std::string> readyNodes;
    readyNodes.push(startNode);

    nodeMap[startNode].pathCounter = 1u;

    while (!readyNodes.empty()) {
        std::string nodeId = readyNodes.front();
        readyNodes.pop();

        for (auto &nextNode : nodeMap[nodeId].outputIds) {
            nodeMap[nextNode].pathCounter += nodeMap[nodeId].pathCounter;
            ++nodeMap[nextNode].countIn;
            if (nodeMap[nextNode].countIn == nodeMap[nextNode].countInTotal) {
                readyNodes.push(nextNode);
            }
        }
    }

    return nodeMap[endNode].pathCounter;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
        return 1;
    }

    std::unordered_map<std::string, Node> nodeMap;
    try {
        nodeMap = readInput(argv[1]);
    } catch (std::runtime_error const &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::string nodeYou = "you";
    std::string nodeOut = "out";
    std::string nodeSvr = "svr";
    std::string nodeDac = "dac";
    std::string nodeFft = "fft";

    std::cout << "Answer to part 1: " <<
        propagateCount(nodeMap, nodeYou, nodeOut) << std::endl;

    std::uint64_t svrToFft = propagateCount(nodeMap, nodeSvr, nodeFft);
    std::uint64_t fftToDac = propagateCount(nodeMap, nodeFft, nodeDac);
    std::uint64_t dacToOut = propagateCount(nodeMap, nodeDac, nodeOut);

    std::cout << "Answer to part 2: " << svrToFft * fftToDac * dacToOut << std::endl;
    
    return 0;
}