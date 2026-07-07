/***************************************************|
|  Persistent Segment Tree (Black Box Template)     |
|  Supports: Update, Sum, Min, Max, GCD, XOR        |
|  0-indexed for positions, 0-indexed for versions. |
|===================================================/
 * Space Complexity: O(N + Q * log N)
 * Time Complexity (Build): O(N)
 * Time Complexity (Query/Update): O(log N)
 ***************************************************/

#include <bits/stdc++.h>
using namespace std;

class PersistentSegTree {
private:
    static const long long INF = 2e18; // Use a sufficiently large value for infinity

    struct Node {
        long long sum, mn, mx, g, xr;
        int lc, rc; // Left child and Right child indices

        // Identity node (used for out of bounds / null merging)
        Node() {
            sum = 0;
            mn = INF;
            mx = -INF;
            g = 0;
            xr = 0;
            lc = rc = 0;
        }

        // Leaf node with a specific value
        Node(long long v) {
            sum = v;
            mn = v;
            mx = v;
            g = abs(v); // GCD should be positive
            xr = v;
            lc = rc = 0;
        }
    };

    vector<Node> tree;
    vector<int> roots; // Stores the root node index for each version
    int n;

    // Helper to merge two nodes
    Node merge(const Node& l, const Node& r) {
        Node res;
        res.sum = l.sum + r.sum;
        res.mn = min(l.mn, r.mn);
        res.mx = max(l.mx, r.mx);
        res.g = std::gcd(l.g, r.g);
        res.xr = l.xr ^ r.xr;
        return res;
    }

    int build(int l, int r, const vector<long long>& a) {
        int u = tree.size();
        tree.emplace_back();
        if (l == r) {
            tree[u] = Node(a[l]);
            return u;
        }
        int mid = l + (r - l) / 2;
        tree[u].lc = build(l, mid, a);
        tree[u].rc = build(mid + 1, r, a);
        
        Node merged = merge(tree[tree[u].lc], tree[tree[u].rc]);
        merged.lc = tree[u].lc;
        merged.rc = tree[u].rc;
        tree[u] = merged;
        
        return u;
    }

    int update(int prev_u, int l, int r, int pos, long long val) {
        int u = tree.size();
        tree.push_back(tree[prev_u]); // Copy previous node's state
        
        if (l == r) {
            tree[u] = Node(val); // Apply the new value at the leaf
            return u;
        }
        
        int mid = l + (r - l) / 2;
        if (pos <= mid) {
            tree[u].lc = update(tree[prev_u].lc, l, mid, pos, val);
        } else {
            tree[u].rc = update(tree[prev_u].rc, mid + 1, r, pos, val);
        }
        
        Node merged = merge(tree[tree[u].lc], tree[tree[u].rc]);
        merged.lc = tree[u].lc;
        merged.rc = tree[u].rc;
        tree[u] = merged;
        
        return u;
    }

    Node query(int u, int l, int r, int ql, int qr) {
        if (ql > qr) return Node(); // Return identity if invalid bounds
        if (ql <= l && r <= qr) return tree[u]; // Fully inside
        
        int mid = l + (r - l) / 2;
        if (qr <= mid) return query(tree[u].lc, l, mid, ql, qr);       // Entirely left
        if (ql > mid) return query(tree[u].rc, mid + 1, r, ql, qr);    // Entirely right
        
        // Overlaps both
        return merge(
            query(tree[u].lc, l, mid, ql, qr), 
            query(tree[u].rc, mid + 1, r, ql, qr)
        );
    }

public:
    // Initialize with a specific array. Version 0 is created.
    PersistentSegTree(const vector<long long>& initial_arr) {
        n = initial_arr.size();
        tree.push_back(Node()); // Dummy 0 index (helps catch bugs)
        roots.push_back(build(0, n - 1, initial_arr));
    }

    // Initialize with zeros. Version 0 is created.
    PersistentSegTree(int size) {
        n = size;
        tree.push_back(Node()); // Dummy 0 index
        roots.push_back(build(0, n - 1, vector<long long>(size, 0)));
    }

    // Point update on a specific version. 
    // Returns the NEW version number.
    int update(int version, int pos, long long val) {
        int new_root = update(roots[version], 0, n - 1, pos, val);
        roots.push_back(new_root);
        return roots.size() - 1;
    }

    // Number of versions currently available (including version 0)
    int versions_count() {
        return roots.size();
    }

    // Queries: (version, L, R) -> 0-indexed bounds [L, R] inclusive
    long long get_sum(int version, int l, int r) { return query(roots[version], 0, n - 1, l, r).sum; }
    long long get_min(int version, int l, int r) { return query(roots[version], 0, n - 1, l, r).mn; }
    long long get_max(int version, int l, int r) { return query(roots[version], 0, n - 1, l, r).mx; }
    long long get_gcd(int version, int l, int r) { return query(roots[version], 0, n - 1, l, r).g; }
    long long get_xor(int version, int l, int r) { return query(roots[version], 0, n - 1, l, r).xr; }
};

int main() {
    // 1. Initial array
    vector<long long> a = {2, 4, 6, 8, 10};
    PersistentSegTree pst(a); // This is Version 0

    // 2. Perform an update -> Creates Version 1
    // Update index 2 from '6' to '12' based on Version 0
    int v1 = pst.update(0, 2, 12); // a is now {2, 4, 12, 8, 10}

    // 3. Perform another update -> Creates Version 2
    // Update index 4 from '10' to '16' based on Version 1
    int v2 = pst.update(v1, 4, 16); // a is now {2, 4, 12, 8, 16}

    // --- Query Examples ---
    
    // Check Original Array (Version 0): {2, 4, 6, 8, 10}
    cout << "Version 0 - Sum[0..4]: " << pst.get_sum(0, 0, 4) << "\n"; // 30
    cout << "Version 0 - Max[0..4]: " << pst.get_max(0, 0, 4) << "\n"; // 10
    cout << "Version 0 - GCD[0..2]: " << pst.get_gcd(0, 0, 2) << "\n"; // 2 (gcd of 2, 4, 6)

    cout << "---------------------------------\n";

    // Check Version 1 Array: {2, 4, 12, 8, 10}
    cout << "Version 1 - Sum[0..4]: " << pst.get_sum(v1, 0, 4) << "\n"; // 36
    cout << "Version 1 - Max[0..4]: " << pst.get_max(v1, 0, 4) << "\n"; // 12
    cout << "Version 1 - GCD[0..2]: " << pst.get_gcd(v1, 0, 2) << "\n"; // 2 (gcd of 2, 4, 12)

    cout << "---------------------------------\n";

    // Check Version 2 Array: {2, 4, 12, 8, 16}
    cout << "Version 2 - Sum[0..4]: " << pst.get_sum(v2, 0, 4) << "\n"; // 42
    cout << "Version 2 - XOR[2..4]: " << pst.get_xor(v2, 2, 4) << "\n"; // 12 ^ 8 ^ 16 = 20

    return 0;
}