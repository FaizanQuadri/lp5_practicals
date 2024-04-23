#include <iostream>
#include <vector>
#include <queue>
#include <omp.h> // OpenMP library for parallel processing
using namespace std;

const int MAX_NODES = 100;
vector<int> graph[MAX_NODES]; // Adjacency list representation of the graph

// Function for parallel Breadth First Search (BFS)
void parallelBFS(int start) {
    bool visited[MAX_NODES] = {false}; // Array to mark visited nodes
    queue<int> q; // Queue for BFS traversal
    q.push(start);
    visited[start] = true;
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        // Parallel loop over neighbors of the current node
        #pragma omp parallel for
        for (int i = 0; i < graph[current].size(); ++i) {
            int neighbor = graph[current][i];
            // Critical section to ensure thread safety when updating visited array and pushing into queue
            #pragma omp critical
            {
                if (!visited[neighbor]) {
                    q.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }
    }
    
    // Output the visited nodes after BFS traversal
    cout << "BFS Visited Nodes: ";
    for (int i = 0; i < MAX_NODES; ++i) {
        if (visited[i]) {
            cout << i << " ";
        }
    }
    cout << endl;
}

// Function for parallel Depth First Search (DFS)
void parallelDFS(int start, bool visited[]) {
    visited[start] = true;
    
    // Parallel loop over neighbors of the current node
    #pragma omp parallel for
    for (int i = 0; i < graph[start].size(); ++i) {
        int neighbor = graph[start][i];
        if (!visited[neighbor]) {
            parallelDFS(neighbor, visited);
        }
    }
}

int main() {
    // Define the graph
    graph[0] = {1, 2};
    graph[1] = {0, 3, 4};
    graph[2] = {0, 5, 6};
    graph[3] = {1};
    graph[4] = {1};
    graph[5] = {2};
    graph[6] = {2};
    
    int start_node = 0;
    
    // Perform parallel BFS
    parallelBFS(start_node);
    
    bool visited[MAX_NODES] = {false}; // Array to mark visited nodes for DFS
    
    // Perform parallel DFS
    parallelDFS(start_node, visited);
    
    // Output the visited nodes after DFS traversal
    cout << "DFS Visited Nodes: ";
    for (int i = 0; i < MAX_NODES; ++i) {
        if (visited[i]) {
            cout << i << " ";
        }
    }
    cout << endl;
    
    return 0;
}
