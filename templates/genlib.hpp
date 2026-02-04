#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <set>
#include <numeric>

using namespace std;


static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

inline long long randInt(long long l, long long r) {
    return uniform_int_distribution<long long>(l, r)(rng);
}

// Generate a random permutation of length n
inline vector<int> genPermutation(int n) {
    vector<int> p(n);
    iota(p.begin(), p.end(), 1);
    shuffle(p.begin(), p.end(), rng);
    return p;
}

// Generate a random array of length n with elements in [min_val, max_val]
inline vector<int> genArray(int n, int minVal, int maxVal) {
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = randInt(minVal, maxVal);
    }
    return a;
}

// Generate a random tree with n nodes using Prüfer sequence
// This ensures a Uniformly Random Labeled Tree (Cayley's Formula)
inline void genTree(int n) {
    if (n == 1) {
        return;
    }
    
    if (n == 2) {
        cout << "1 2\n";
        return;
    }

    vector<int> prufer(n - 2);
    vector<int> degree(n, 1);
    for (int i = 0; i < n - 2; ++i) {
        prufer[i] = randInt(0, n - 1);
        degree[prufer[i]] += 1;
    }

    set<int> leaves;
    for (int i = 0; i < n; ++i) {
        if (degree[i] == 1) {
            leaves.insert(i);
        }
    }

    vector<pair<int, int>> edges;
    for (int i = 0; i < n - 2; ++i) {
        int u = *leaves.begin(); 
        leaves.erase(leaves.begin());
        
        int v = prufer[i];
        edges.emplace_back(u + 1, v + 1);
        
        degree[v] -= 1;
        if (degree[v] == 1) {
            leaves.insert(v);
        }
    }
    
    int u = *leaves.begin(); leaves.erase(leaves.begin());
    int v = *leaves.begin();
    edges.emplace_back(u + 1, v + 1);

    vector<int> p = genPermutation(n);
    shuffle(edges.begin(), edges.end(), rng);
    
    for (auto& edge : edges) {
        if (randInt(0, 1)) {
            swap(edge.first, edge.second); 
        }
        cout << p[edge.first - 1] << " " << p[edge.second - 1] << "\n";
    }
}

// Generate a random connected graph with n nodes and m edges
inline void genConnectedGraph(int n, int m) {
    if (m < n - 1) {
        cerr << "Error: m must be >= n - 1 for connected graph" << endl;
        return;
    }
    
    set<pair<int, int>> edges;
    
    for (int i = 2; i <= n; ++i) {
        int u = randInt(1, i - 1);
        int v = i;
        if (u > v) {
            swap(u, v);
        }
        edges.emplace(u, v);
    }
    
    long long maxEdges = 1LL * n * (n - 1) / 2;
    if (m > maxEdges) {
        m = maxEdges; 
    }
    
    while (edges.size() < m) {
        int u = randInt(1, n);
        int v = randInt(1, n);
        if (u == v) {
            continue;
        }
        if (u > v) {
            swap(u, v);
        }
        edges.emplace(u, v);
    }
    
    vector<pair<int, int>> edgeList(edges.begin(), edges.end());
    
    vector<int> p = genPermutation(n);
    shuffle(edgeList.begin(), edgeList.end(), rng);
    
    cout << n << " " << m << "\n";
    for (auto& edge : edgeList) {
        cout << p[edge.first - 1] << " " << p[edge.second - 1] << "\n";
    }
}

// Generate a random graph with n nodes and m edges (not necessarily connected)
inline void genGraph(int n, int m) {
    set<pair<int, int>> edges;
    
    long long maxEdges = 1LL * n * (n - 1) / 2;
    if (m > maxEdges) {
        m = maxEdges; 
    }
    
    while (edges.size() < m) {
        int u = randInt(1, n);
        int v = randInt(1, n);
        if (u == v) {
            continue;
        }
        if (u > v) {
            swap(u, v);
        }
        edges.emplace(u, v);
    }
    
    vector<pair<int, int>> edgeList(edges.begin(), edges.end());
    
    vector<int> p = genPermutation(n);
    shuffle(edgeList.begin(), edgeList.end(), rng);
    
    cout << n << " " << m << "\n";
    for (auto& edge : edgeList) {
        cout << p[edge.first - 1] << " " << p[edge.second - 1] << "\n";
    }
}
