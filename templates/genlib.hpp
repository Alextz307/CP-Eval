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

inline long long rand_int(long long l, long long r) {
    return uniform_int_distribution<long long>(l, r)(rng);
}

// Generate a random permutation of length n
inline vector<int> gen_permutation(int n) {
    vector<int> p(n);
    iota(p.begin(), p.end(), 1);
    shuffle(p.begin(), p.end(), rng);
    return p;
}

// Generate a random array of length n with elements in [min_val, max_val]
inline vector<int> gen_array(int n, int min_val, int max_val) {
    vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = rand_int(min_val, max_val);
    return a;
}

// Generate a random tree with n nodes using Prüfer sequence
// This ensures a Uniformly Random Labeled Tree (Cayley's Formula)
inline void gen_tree(int n) {
    if (n == 1) return;
    if (n == 2) {
        cout << "1 2\n";
        return;
    }

    // Prüfer sequence: n-2 numbers from [0, n-1]
    vector<int> prufer(n - 2);
    vector<int> degree(n, 1);
    for (int i = 0; i < n - 2; ++i) {
        prufer[i] = rand_int(0, n - 1);
        degree[prufer[i]]++;
    }

    set<int> leaves;
    for (int i = 0; i < n; ++i) {
        if (degree[i] == 1) leaves.insert(i);
    }

    vector<pair<int, int>> edges;
    for (int i = 0; i < n - 2; ++i) {
        int u = *leaves.begin(); // Smallest leaf
        leaves.erase(leaves.begin());
        
        int v = prufer[i];
        edges.push_back({u + 1, v + 1}); // 1-based indexing
        
        degree[v]--;
        if (degree[v] == 1) leaves.insert(v);
    }
    
    // The last two nodes
    int u = *leaves.begin(); leaves.erase(leaves.begin());
    int v = *leaves.begin();
    edges.push_back({u + 1, v + 1});

    vector<int> p = gen_permutation(n);
    shuffle(edges.begin(), edges.end(), rng);
    
    for (auto& edge : edges) {
        if (rand_int(0, 1)) swap(edge.first, edge.second); 
        cout << p[edge.first - 1] << " " << p[edge.second - 1] << "\n";
    }
}

// Generate a random connected graph with n nodes and m edges
inline void gen_connected_graph(int n, int m) {
    if (m < n - 1) {
        cerr << "Error: m must be >= n - 1 for connected graph" << endl;
        return;
    }
    
    set<pair<int, int>> edges;
    
    // 1. Spanning tree to ensure connectivity
    for (int i = 2; i <= n; ++i) {
        int u = rand_int(1, i - 1);
        int v = i;
        if (u > v) swap(u, v);
        edges.insert({u, v});
    }
    
    // 2. Add remaining edges
    long long max_edges = 1LL * n * (n - 1) / 2;
    if (m > max_edges) m = max_edges; 
    
    while (edges.size() < m) {
        int u = rand_int(1, n);
        int v = rand_int(1, n);
        if (u == v) continue;
        if (u > v) swap(u, v);
        edges.insert({u, v});
    }
    
    vector<pair<int, int>> edge_list(edges.begin(), edges.end());
    
    vector<int> p = gen_permutation(n);
    shuffle(edge_list.begin(), edge_list.end(), rng);
    
    cout << n << " " << m << "\n";
    for (auto& edge : edge_list) {
        cout << p[edge.first - 1] << " " << p[edge.second - 1] << "\n";
    }
}
