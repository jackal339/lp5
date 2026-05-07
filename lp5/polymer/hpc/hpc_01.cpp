#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// ---------------- SAFE PARALLEL BFS ----------------
void parallelBFS(vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "Parallel BFS Traversal: ";

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        cout << node << " ";

        // Process neighbors (controlled parallelism)
        #pragma omp parallel for ordered
        for (int i = 0; i < graph[node].size(); i++) {
            int neighbor = graph[node][i];

            if (!visited[neighbor]) {
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
}

// ---------------- CONTROLLED PARALLEL DFS ----------------
void dfsUtil(vector<vector<int>>& graph, int node, vector<bool>& visited) {
    cout << node << " ";

    for (int neighbor : graph[node]) {  

        bool shouldVisit = false;

        #pragma omp critical
        {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                shouldVisit = true;
            }
        }

        if (shouldVisit) {
            #pragma omp task
            dfsUtil(graph, neighbor, visited);
        }
    }

    #pragma omp taskwait
}

void parallelDFS(vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);

    visited[start] = true;

    cout << "\nParallel DFS Traversal: ";

    #pragma omp parallel
    {
        #pragma omp single
        {
            dfsUtil(graph, start, visited);
        }
    }
}

// ---------------- MAIN ----------------
int main() {
    vector<vector<int>> graph = {
        {1, 2},
        {0, 3, 4},
        {0, 5},
        {1},
        {1},
        {2}
    };

    parallelBFS(graph, 0);
    parallelDFS(graph, 0);

    cout << endl;
    return 0;
}



// #include <iostream>
// #include <vector>
// #include <queue>
// #include <omp.h>

// using namespace std;

// // ---------------- PARALLEL BFS ----------------
// void parallelBFS(vector<vector<int>>& graph, int start) {
//     vector<bool> visited(graph.size(), false);
//     queue<int> q;

//     visited[start] = true;
//     q.push(start);

//     cout << "Parallel BFS Traversal: ";

//     while (!q.empty()) {
//         int node = q.front();
//         q.pop();
//         cout << node << " ";

//         #pragma omp parallel for
//         for (int i = 0; i < graph[node].size(); i++) {
//             int neighbor = graph[node][i];

//             if (!visited[neighbor]) {
//                 #pragma omp critical
//                 {
//                     if (!visited[neighbor]) {
//                         visited[neighbor] = true;
//                         q.push(neighbor);
//                     }
//                 }
//             }
//         }
//     }
// }

// // ---------------- PARALLEL DFS (TASK BASED) ----------------
// void dfsTask(vector<vector<int>>& graph, int node, vector<bool>& visited) {
//     bool alreadyVisited = false;

//     // Critical section to safely update visited
//     #pragma omp critical
//     {
//         if (visited[node]) {
//             alreadyVisited = true;
//         } else {
//             visited[node] = true;
//             cout << node << " ";
//         }
//     }

//     if (alreadyVisited) return;

//     // Create tasks for neighbors
//     for (int neighbor : graph[node]) {
//         #pragma omp task
//         dfsTask(graph, neighbor, visited);
//     }
// }

// void parallelDFS(vector<vector<int>>& graph, int start) {
//     vector<bool> visited(graph.size(), false);

//     cout << "\nParallel DFS Traversal: ";

//     #pragma omp parallel
//     {
//         #pragma omp single
//         {
//             dfsTask(graph, start, visited);
//         }
//     }
// }

// // ---------------- MAIN ----------------
// int main() {
//     // Undirected graph (Adjacency List)
//     vector<vector<int>> graph = {
//         {1, 2},      // 0
//         {0, 3, 4},   // 1
//         {0, 5},      // 2
//         {1},         // 3
//         {1},         // 4
//         {2}          // 5
//     };

//     int startNode = 0;

//     parallelBFS(graph, startNode);
//     parallelDFS(graph, startNode);

//     cout << endl;
//     return 0;
// }