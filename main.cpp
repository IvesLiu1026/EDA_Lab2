#include <bits/stdc++.h>

using namespace std;

struct Node {
    int partition;  // Current partition of the node
    vector<int> nets;  // Nets the node is connected to
    bool locked;  // Indicates if the node has been locked (moved)
};

struct Net {
    unordered_map<int, int> nodes;  // Nodes connected by this net
};

vector<Node> nodes;
vector<Net> nets;
unordered_map<int, int> partitionSize;  // Keep track of the size of each partition
int numNodes, numNets;

void readInput(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file." << endl;
        exit(1);
    }
    file >> numNets >> numNodes;
    nodes.resize(numNodes);
    nets.resize(numNets);

    string line;
    getline(file, line);  // Skip the rest of the first line
    for (int i = 0; i < numNets; i++) {
        getline(file, line);
        istringstream iss(line);
        int nodeId;
        while (iss >> nodeId) {
            --nodeId;  // Convert to zero-based index
            nets[i].nodes[nodeId]++;
            nodes[nodeId].nets.push_back(i);
        }
    }
    file.close();
}

void initialPartition() {
    for (int i = 0; i < numNodes; i++) {
        nodes[i].partition = i < numNodes / 2 ? 0 : 1;
        nodes[i].locked = false;
        partitionSize[nodes[i].partition]++;
    }
}

int calculateGain(int nodeId) {
    int gain = 0;
    Node& node = nodes[nodeId];
    int currentPartition = node.partition;
    for (int netId : node.nets) {
        bool isInternal = true, isExternal = true;
        for (auto& kv : nets[netId].nodes) {
            if (nodes[kv.first].partition == currentPartition) {
                isExternal = false;
            } else {
                isInternal = false;
            }
        }
        if (isInternal) gain--;
        if (isExternal) gain++;
    }
    return gain;
}

void optimizePartition() {
    bool improved = true;
    while (improved) {
        improved = false;
        for (int i = 0; i < numNodes; i++) {
            if (nodes[i].locked) continue;
            int gain = calculateGain(i);
            if (gain > 0) {
                nodes[i].partition = 1 - nodes[i].partition;  // Swap partition
                nodes[i].locked = true;
                partitionSize[0]--;
                partitionSize[1]++;
                improved = true;
            }
        }
    }
}

void outputResult(const string& outputFilename) {
    ofstream output(outputFilename);
    for (const Node& node : nodes) {
        output << node.partition << endl;
    }
    output.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input file>" << endl;
        return 1;
    }
    string inputFilename = argv[1];
    string outputFilename = "output.txt";

    readInput(inputFilename);
    initialPartition();
    optimizePartition();
    outputResult(outputFilename);

    return 0;
}