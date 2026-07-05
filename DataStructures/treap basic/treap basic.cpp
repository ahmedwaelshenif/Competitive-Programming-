#include <bits/stdc++.h>
using namespace std;

struct LightImplicitTreap {
    using ll = long long;

    struct Node {
        ll val;                // value at this position
        int sz;                // subtree size
        uint32_t pr;           // random priority
        Node *l, *r;

        Node(ll v, uint32_t p) : val(v), sz(1), pr(p), l(nullptr), r(nullptr) {}
    };

    Node* root = nullptr;
    mt19937 rng{(uint32_t)chrono::steady_clock::now().time_since_epoch().count()};

    LightImplicitTreap() = default;

    // Build from vector by repeated insert at end
    LightImplicitTreap(const vector<ll>& a) {
        for (ll x : a) push_back(x);
    }

    // --------- basic helpers ---------
    int size() const { return get_sz(root); }

    static int get_sz(Node* t) { return t ? t->sz : 0; }

    void pull(Node* t) {
        if (!t) return;
        t->sz = 1 + get_sz(t->l) + get_sz(t->r);
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
            a->r = merge(a->r, b);
            pull(a);
            return a;
        } else {
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
        }
        root = merge(a, merge(b, c));
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
        inorder(t->l, res);
        res.push_back(t->val);
        inorder(t->r, res);
    }
};