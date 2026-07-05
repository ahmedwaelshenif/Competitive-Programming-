#include<bits/stdc++.h>
using namespace std;
class MergeSortTree {
private:
    int n;
    vector<vector<int>> tree;
    vector<vector<long long>> prefix; // prefix[node][i] = sum of first i elements

    void build(int node, int l, int r, const vector<int>& a) {
        if (l == r) {
            tree[node] = {a[l]};
            prefix[node] = {0, (long long)a[l]}; 
            return;
        }
        int mid = (l + r) / 2;
        build(2*node+1, l, mid, a);
        build(2*node+2, mid+1, r, a);
        merge(tree[2*node+1].begin(), tree[2*node+1].end(),
              tree[2*node+2].begin(), tree[2*node+2].end(),
              back_inserter(tree[node]));

        
        prefix[node].resize(tree[node].size() + 1, 0);
        for (int i = 0; i < (int)tree[node].size(); i++)
            prefix[node][i+1] = prefix[node][i] + tree[node][i];
    }

    // Count of elements ≤ k in range [ql, qr]
    int count_le(int node, int l, int r, int ql, int qr, int k) {
        if (r < ql || l > qr) return 0;
        if (ql <= l && r <= qr) {
            return upper_bound(tree[node].begin(), tree[node].end(), k) - tree[node].begin();
        }
        int mid = (l + r) / 2;
        return count_le(2*node+1, l, mid, ql, qr, k) + count_le(2*node+2, mid+1, r, ql, qr, k);
    }

    // Count of elements ≥ k in range
    int count_ge(int node, int l, int r, int ql, int qr, int k) {
        if (r < ql || l > qr) return 0;
        if (ql <= l && r <= qr) {
            return tree[node].end() - lower_bound(tree[node].begin(), tree[node].end(), k);
        }
        int mid = (l + r) / 2;
        return count_ge(2*node+1, l, mid, ql, qr, k) + count_ge(2*node+2, mid+1, r, ql, qr, k);
    }

    // Count of exact occurrences of x
    int count_eq(int node, int l, int r, int ql, int qr, int x) {
        if (r < ql || l > qr) return 0;
        if (ql <= l && r <= qr) {
            return upper_bound(tree[node].begin(), tree[node].end(), x) -
                   lower_bound(tree[node].begin(), tree[node].end(), x);
        }
        int mid = (l + r) / 2;
        return count_eq(2*node+1, l, mid, ql, qr, x) + count_eq(2*node+2, mid+1, r, ql, qr, x);
    }

    // Helper for k-th smallest (binary search on value)
    int kth_smallest(int l, int r, int k, int lo, int hi) {
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            int cnt = count_le(0, 0, n-1, l, r, mid);
            if (cnt >= k) hi = mid;
            else lo = mid + 1;
        }
        return lo;
    }
    int maxLessThan(int node, int l, int r, int ql, int qr, int k) {
        if (r < ql || l > qr) return LLONG_MIN;
        if (ql <= l && r <= qr) {
            auto it = lower_bound(tree[node].begin(), tree[node].end(), k);
            if (it == tree[node].begin()) return LLONG_MIN; // no element < k
            return *(--it);
        }
        int mid = (l + r) / 2;
        return max(
            maxLessThan(2 * node + 1, l, mid, ql, qr, k),
            maxLessThan(2 * node + 2, mid + 1, r, ql, qr, k)
        );
    }

    int minGreaterThan(int node, int l, int r, int ql, int qr, int k) {
        if (r < ql || l > qr) return LLONG_MAX;
        if (ql <= l && r <= qr) {
            auto it = upper_bound(tree[node].begin(), tree[node].end(), k);
            if (it == tree[node].end()) return LLONG_MAX; // no element > k
            return *it;
        }
        int mid = (l + r) / 2;
        return min(
            minGreaterThan(2 * node + 1, l, mid, ql, qr, k),
            minGreaterThan(2 * node + 2, mid + 1, r, ql, qr, k)
        );
    }
    // Sum of elements <= k in range [ql..qr]
    int sum_le(int node, int l, int r, int ql, int qr, int k) {
        if (r < ql || l > qr) return 0;
        if (ql <= l && r <= qr) {
            int pos = upper_bound(tree[node].begin(), tree[node].end(), k) 
                      - tree[node].begin();
            return prefix[node][pos];
        }
        int mid = (l + r) / 2;
        return sum_le(2*node+1, l, mid, ql, qr, k) 
             + sum_le(2*node+2, mid+1, r, ql, qr, k);
    }

// Sum of elements >= k in range [ql..qr]
    int sum_ge(int node, int l, int r, int ql, int qr, int k) {
        if (r < ql || l > qr) return 0;
        if (ql <= l && r <= qr) {
            int pos = lower_bound(tree[node].begin(), tree[node].end(), k) 
                      - tree[node].begin();
            return prefix[node][tree[node].size()] - prefix[node][pos];
        }
        int mid = (l + r) / 2;
        return sum_ge(2*node+1, l, mid, ql, qr, k) 
             + sum_ge(2*node+2, mid+1, r, ql, qr, k);
    }


    public:
       MergeSortTree(const vector<int>& a) {
        n = a.size();
        tree.resize(4 * n);
        prefix.resize(4 * n); 
        build(0, 0, n - 1, a);
    }

    // Interface methods
    int countLessEqual(int l, int r, int x) {
        return count_le(0, 0, n - 1, l, r, x);
    }

    int countGreaterEqual(int l, int r, int x) {
        return count_ge(0, 0, n - 1, l, r, x);
    }

    int countLessThan(int l, int r, int x) {
        return count_le(0, 0, n - 1, l, r, x - 1);
    }

    int countGreaterThan(int l, int r, int x) {
        return count_ge(0, 0, n - 1, l, r, x + 1);
    }

    int countEqual(int l, int r, int x) {
        return count_eq(0, 0, n - 1, l, r, x);
    }

    int kthSmallest(int l, int r, int k) {
        // Adjust search range based on actual min/max if needed
        int lo = INT_MIN, hi = INT_MAX;
        return kth_smallest(l, r, k, lo, hi);
    }
    int kthLargest(int l, int r, int k) {
        int total = r - l + 1;
        return kthSmallest(l, r, total - k + 1);
    }
    int getMaxLessThan(int l, int r, int k) {
        int res = maxLessThan(0, 0, n - 1, l, r, k);
        return res == LLONG_MIN ? -1 : res; // or any sentinel value
    }

    int getMinGreaterThan(int l, int r, int k) {
        int res = minGreaterThan(0, 0, n - 1, l, r, k);
        return res == LLONG_MAX ? -1 : res; // or any sentinel value
    }
    // Count elements where lo <= x <= hi in subarray [l..r]
    int countInRange(int l, int r, int lo, int hi) {
    return count_le(0, 0, n-1, l, r, hi) 
         - count_le(0, 0, n-1, l, r, lo - 1);
    }
    int sumLessEqual(int l, int r, int k) {
        return sum_le(0, 0, n-1, l, r, k);
    }

    int sumGreaterEqual(int l, int r, int k) {
        return sum_ge(0, 0, n-1, l, r, k);
    }

    // Bonus: sum of elements where lo <= x <= hi in [l..r]
    int sumInRange(int l, int r, int lo, int hi) {
        return sum_le(0, 0, n-1, l, r, hi) 
             - sum_le(0, 0, n-1, l, r, lo - 1);
    }
    int  sumLessThan(int l, int r, int k) {
        return sum_le(0, 0, n-1, l, r, k - 1);
    }

    int  sumGreaterThan(int l, int r, int k) {
        return sum_ge(0, 0, n-1, l, r, k + 1);
    }
};