#include <bits/stdc++.h>
using namespace std;

struct ImplicitTreap {
    using ll = long long;

    struct Node {
        ll val;                // value at this position
        ll sum;                // sum of subtree
        ll sum_even;           // sum of even-indexed elements in subtree
        ll sum_odd;            // sum of odd-indexed elements in subtree
        ll xr;                 // xor of subtree
        ll mn, mx;             // min/max of subtree
        int sz;                // subtree size
        uint32_t pr;           // random priority
        bool rev;              // lazy reverse flag
        
        // New lazy tags for range updates
        bool has_set;          // lazy set flag
        ll lazy_set;           // lazy set value
        ll lazy_add;           // lazy add value

        Node *l, *r;

        Node(ll v, uint32_t p) : val(v), sum(v), sum_even(v), sum_odd(0), xr(v), mn(v), mx(v),
                                 sz(1), pr(p), rev(false), has_set(false), lazy_set(0), lazy_add(0),
                                 l(nullptr), r(nullptr) {}
    };

    Node* root = nullptr;
    mt19937 rng{(uint32_t)chrono::steady_clock::now().time_since_epoch().count()};

    static constexpr ll INF = (1LL << 62);

    ImplicitTreap() = default;

    // Build from vector in O(n log n) by repeated insert at end.
    ImplicitTreap(const vector<ll>& a) {
        for (ll x : a) push_back(x);
    }

    // --------- basic helpers ---------
    int size() const { return get_sz(root); }

    static int get_sz(Node* t) { return t ? t->sz : 0; }
    static ll get_sum(Node* t) { return t ? t->sum : 0; }
    static ll get_sum_even(Node* t) { return t ? t->sum_even : 0; }
    static ll get_sum_odd(Node* t)  { return t ? t->sum_odd : 0; }
    static ll get_xr(Node* t)  { return t ? t->xr  : 0; }
    static ll get_mn(Node* t)  { return t ? t->mn  : INF; }
    static ll get_mx(Node* t)  { return t ? t->mx  : -INF; }

    void pull(Node* t) {
        if (!t) return;
        t->sz  = 1 + get_sz(t->l) + get_sz(t->r);
        t->sum = t->val + get_sum(t->l) + get_sum(t->r);
        t->xr  = t->val ^ get_xr(t->l) ^ get_xr(t->r);
        t->mn  = min({t->val, get_mn(t->l), get_mn(t->r)});
        t->mx  = max({t->val, get_mx(t->l), get_mx(t->r)});

        int lsz = get_sz(t->l);
        if (lsz % 2 == 0) {
            // Left size is even -> Root is even -> Right child shifts by lsz + 1 (Odd)
            // Parities in the right child FLIP
            t->sum_even = get_sum_even(t->l) + t->val + get_sum_odd(t->r);
            t->sum_odd  = get_sum_odd(t->l)  + 0      + get_sum_even(t->r);
        } else {
            // Left size is odd -> Root is odd -> Right child shifts by lsz + 1 (Even)
            // Parities in the right child PRESERVE
            t->sum_even = get_sum_even(t->l) + 0      + get_sum_even(t->r);
            t->sum_odd  = get_sum_odd(t->l)  + t->val + get_sum_odd(t->r);
        }
    }

    void apply_rev(Node* t) {
        if (!t) return;
        swap(t->l, t->r);
        // If a subtree of even size is reversed, all its even-indexed elements
        // shift to odd indices, and all odd-indexed elements shift to even.
        if (t->sz % 2 == 0) {
            swap(t->sum_even, t->sum_odd);
        }
        t->rev ^= 1;
    }

    void apply_set(Node* t, ll v) {
        if (!t) return;
        t->val = v;
        t->sum = v * t->sz;
        
        ll evens = (t->sz + 1) / 2;
        ll odds = t->sz / 2;
        t->sum_even = v * evens;
        t->sum_odd = v * odds;
        
        t->mn = v;
        t->mx = v;
        t->xr = (t->sz % 2 == 1) ? v : 0;
        
        t->lazy_set = v;
        t->has_set = true;
        t->lazy_add = 0; // Set overrides any pending adds
    }

    void apply_add(Node* t, ll v) {
        if (!t) return;
        t->val += v;
        t->sum += v * t->sz;
        
        ll evens = (t->sz + 1) / 2;
        ll odds = t->sz / 2;
        t->sum_even += v * evens;
        t->sum_odd += v * odds;
        
        t->mn += v;
        t->mx += v;
        // Note: Mathematical limitation - XOR cannot be maintained cleanly with range additions
        
        t->lazy_add += v;
    }

    void push(Node* t) {
        if (!t) return;
        if (t->rev) {
            apply_rev(t->l);
            apply_rev(t->r);
            t->rev = false;
        }
        if (t->has_set) {
            apply_set(t->l, t->lazy_set);
            apply_set(t->r, t->lazy_set);
            t->has_set = false;
        }
        if (t->lazy_add != 0) {
            apply_add(t->l, t->lazy_add);
            apply_add(t->r, t->lazy_add);
            t->lazy_add = 0;
        }
    }

    Node* new_node(ll v) {
        return new Node(v, rng());
    }

    // Split by first k elements:
    // a = first k nodes, b = remaining nodes
    void split(Node* t, int k, Node*& a, Node*& b) {
        if (!t) {
            a = b = nullptr;
            return;
        }
        push(t);
        if (get_sz(t->l) >= k) {
            split(t->l, k, a, t->l);
            b = t;
            pull(b);
        } else {
            split(t->r, k - get_sz(t->l) - 1, t->r, b);
            a = t;
            pull(a);
        }
    }

    // Merge two treaps where all elements in a come before all elements in b
    Node* merge(Node* a, Node* b) {
        if (!a || !b) return a ? a : b;
        if (a->pr > b->pr) {
            push(a);
            a->r = merge(a->r, b);
            pull(a);
            return a;
        } else {
            push(b);
            b->l = merge(a, b->l);
            pull(b);
            return b;
        }
    }

    // --------- interface ---------

    void insert_at(int pos, ll x) {
        Node *a, *b;
        split(root, pos, a, b);
        root = merge(merge(a, new_node(x)), b);
    }

    void push_back(ll x) {
        insert_at(size(), x);
    }

    void erase_at(int pos) {
        Node *a, *b, *c;
        split(root, pos, a, b);        
        split(b, 1, b, c);            
        delete b;
        root = merge(a, c);
    }

    ll get_at(int pos) {
        Node* t = root;
        while (t) {
            push(t);
            int left_sz = get_sz(t->l);
            if (pos < left_sz) {
                t = t->l;
            } else if (pos == left_sz) {
                return t->val;
            } else {
                pos -= left_sz + 1;
                t = t->r;
            }
        }
        throw runtime_error("get_at: position out of range");
    }

    void set_at(int pos, ll x) {
        Node *a, *b, *c;
        split(root, pos, a, b);
        split(b, 1, b, c);
        if (b) {
            b->val = x;
            pull(b);
        }
        root = merge(a, merge(b, c));
    }

    // Range Addition Update
    void add_range(int l, int r, ll v) {
        Node *a, *b, *c;
        split(root, l, a, b);
        split(b, r - l + 1, b, c);
        apply_add(b, v);
        root = merge(a, merge(b, c));
    }

    // Range Assignment Update
    void set_range(int l, int r, ll v) {
        Node *a, *b, *c;
        split(root, l, a, b);
        split(b, r - l + 1, b, c);
        apply_set(b, v);
        root = merge(a, merge(b, c));
    }

    struct Info {
        ll sum, sum_even, sum_odd, xr, mn, mx;
    };

    Info range_query(int l, int r) {
        Node *a, *b, *c;
        split(root, l, a, b);
        split(b, r - l + 1, b, c);

        Info res;
        res.sum      = get_sum(b);
        res.sum_even = get_sum_even(b);
        res.sum_odd  = get_sum_odd(b);
        res.xr       = get_xr(b);
        res.mn       = get_mn(b);
        res.mx       = get_mx(b);

        root = merge(a, merge(b, c));
        return res;
    }

    ll range_sum(int l, int r) { return range_query(l, r).sum; }
    ll range_sum_even(int l, int r) { return range_query(l, r).sum_even; }
    ll range_sum_odd(int l, int r) { return range_query(l, r).sum_odd; }
    ll range_xor(int l, int r) { return range_query(l, r).xr; }
    ll range_min(int l, int r) { return range_query(l, r).mn; }
    ll range_max(int l, int r) { return range_query(l, r).mx; }

    void reverse_range(int l, int r) {
        Node *a, *b, *c;
        split(root, l, a, b);
        split(b, r - l + 1, b, c);
        apply_rev(b);
        root = merge(a, merge(b, c));
    }

    void rotate_left(int l, int r, int k) {
        int len = r - l + 1;
        if (len <= 1) return;
        k %= len;
        if (k < 0) k += len;
        if (k == 0) return;

        Node *a, *b, *c;
        split(root, l, a, b);
        split(b, len, b, c);

        Node *x, *y;
        split(b, k, x, y);
        b = merge(y, x);

        root = merge(a, merge(b, c));
    }

    void rotate_right(int l, int r, int k) {
        int len = r - l + 1;
        if (len <= 1) return;
        k %= len;
        if (k < 0) k += len;
        rotate_left(l, r, len - k);
    }

    // ---------------------------------------------------------------
    // ADDED: cut-and-paste a subarray [l, r] to start at index `pos`
    // (pos is measured in the array AFTER the [l, r] block is removed)
    // ---------------------------------------------------------------
    void move_range(int l, int r, int pos) {
        Node *a, *b, *c;
        split(root, l, a, b);
        split(b, r - l + 1, b, c);   // b = the block being moved
        root = merge(a, c);          // sequence with the block removed

        Node *x, *y;
        split(root, pos, x, y);
        root = merge(merge(x, b), y);
    }

    // ---------------------------------------------------------------
    // ADDED: binary search on the `sum` aggregate.
    // Returns the smallest index i such that sum(a[0..i]) >= x, or
    // size() if no prefix reaches x. Requires values to be
    // non-negative (so prefix sums are monotonic) -- otherwise the
    // descent below isn't valid. O(log n).
    // ---------------------------------------------------------------
    int find_prefix_ge(ll x) {
        Node* t = root;
        ll acc = 0;      // sum of everything fully to the left of t's subtree
        int offset = 0;  // count of elements fully to the left of t's subtree
        while (t) {
            push(t);
            ll left_sum = get_sum(t->l);
            if (acc + left_sum >= x) {
                t = t->l;
            } else if (acc + left_sum + t->val >= x) {
                return offset + get_sz(t->l);
            } else {
                acc += left_sum + t->val;
                offset += get_sz(t->l) + 1;
                t = t->r;
            }
        }
        return size(); // no prefix reaches x
    }

    // ---------------------------------------------------------------
    // ADDED: binary search on the `mx` aggregate.
    // Returns the leftmost index whose value >= x, or -1 if none
    // exist. Works regardless of sign, no monotonicity assumption
    // needed (just uses subtree max to prune). O(log n).
    // ---------------------------------------------------------------
    int find_first_value_ge(ll x) {
        if (get_mx(root) < x) return -1;
        Node* t = root;
        int offset = 0;
        while (t) {
            push(t);
            if (get_mx(t->l) >= x) {
                t = t->l;
            } else if (t->val >= x) {
                return offset + get_sz(t->l);
            } else {
                offset += get_sz(t->l) + 1;
                t = t->r;
            }
        }
        return -1; // unreachable given the initial mx check
    }

    vector<ll> to_vector() {
        vector<ll> res;
        res.reserve(size());
        inorder(root, res);
        return res;
    }

private:
    void inorder(Node* t, vector<ll>& res) {
        if (!t) return;
        push(t);
        inorder(t->l, res);
        res.push_back(t->val);
        inorder(t->r, res);
    }
};