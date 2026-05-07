#include <omp.h>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Node {
public:
    int value;
    Node *left, *right;
    Node(int v) {
        value = v;
        left = right = NULL;
    }
};

Node* generateTree(int arr[], int n) {
    vector<Node*> nodes;
    for (int i = 0; i < n; i++)
        nodes.push_back(new Node(arr[i]));

    for (int i = 0; i < n; i++) {
        if (2*i+1 < n) nodes[i]->left  = nodes[2*i+1];
        if (2*i+2 < n) nodes[i]->right = nodes[2*i+2];
    }
    return nodes[0];
}

void printTree(Node* root) {
    vector<Node*> level{root};
    int lvl = 0;
    while (!level.empty()) {
        cout << "Level " << lvl++ << ": ";
        vector<Node*> next;
        for (Node* node : level) {
            cout << node->value << " ";
            if (node->left) next.push_back(node->left);
            if (node->right) next.push_back(node->right);
        }
        cout << endl;
        level = next;
    }
}

void parallelBFS(Node* root) {
    vector<Node*> level{root};
    while (!level.empty()) {
        vector<Node*> next;

        #pragma omp parallel for ordered
        for (int i = 0; i < level.size(); i++) {
            Node* node = level[i];

            #pragma omp ordered
            cout << node->value << " ";

            if (node->left) {
                #pragma omp critical
                next.push_back(node->left);
            }
            if (node->right) {
                #pragma omp critical
                next.push_back(node->right);
            }
        }
        level = next;
    }
}

void dfs(Node* root) {
    if (!root) return;

    #pragma omp critical
    cout << root->value << " ";

    #pragma omp task
    dfs(root->left);

    #pragma omp taskwait

    #pragma omp task
    dfs(root->right);

    #pragma omp taskwait
}

void parallelDFS(Node* root) {
    #pragma omp parallel
    {
        #pragma omp single
        dfs(root);
    }
}

int main() {
    int arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int n = sizeof(arr)/sizeof(arr[0]);
    Node* root = generateTree(arr, n);

    cout << "Tree Structure:\n";
    printTree(root);

    cout << "\nParallel BFS: ";
    auto start_bfs = high_resolution_clock::now();
    parallelBFS(root);
    auto end_bfs = high_resolution_clock::now();
    cout << "\nBFS Time: "
         << duration_cast<microseconds>(end_bfs - start_bfs).count()
         << " microseconds\n";

    cout << "\nParallel DFS: ";
    auto start_dfs = high_resolution_clock::now();
    parallelDFS(root);
    auto end_dfs = high_resolution_clock::now();
    cout << "\nDFS Time: "
         << duration_cast<microseconds>(end_dfs - start_dfs).count()
         << " microseconds\n";
}