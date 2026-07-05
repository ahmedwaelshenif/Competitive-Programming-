struct TreeNode {
    int sum = 0, lazy = 0, isLazy = 0;
    TreeNode *left = nullptr, *right = nullptr;

    TreeNode() {}
    TreeNode(int x): sum(x) {}
    TreeNode(TreeNode *&a, TreeNode *&b): left(a), right(b) {}

    void update(int x, ll lx, ll rx) {
        sum += 1ll * x * ((rx - lx) % mod) % mod;
        if(sum >= mod) sum -= mod;
        lazy += x, isLazy = 1;
        if(lazy >= mod) lazy -= mod;
    }
};

using Node = TreeNode*;

struct DynamicSagara {
    Node root = new TreeNode();
    const ll n;

    DynamicSagara(ll n): n(n) {}

    void propagate(Node &cur, ll lx, ll rx) {
        ll m = (lx + rx) / 2;
        if (cur->left == nullptr) cur->left = new TreeNode(); // l -> m
        if (cur->right == nullptr) cur->right = new TreeNode(); // m -> r
        if (!cur->isLazy) return;

        cur->left->update(cur->lazy, lx, m);
        cur->right->update(cur->lazy, m, rx);

        cur->lazy = cur->isLazy = 0;
    }

    void update(ll &l, ll &r, int &val, Node &node, ll lx, ll rx) {
        if (lx >= r || rx <= l) return;
        if (lx >= l && rx <= r) return node->update(val, lx, rx);
        propagate(node, lx, rx);

        ll m = (lx + rx) / 2;
        update(l, r, val, node->left, lx, m);
        update(l, r, val, node->right, m, rx);

        node->sum = (node->left->sum + node->right->sum);
        if(node->sum >= mod) node->sum -= mod;
    }

    ll query(ll l, ll r, Node &node, ll lx, ll rx) {
        if (lx >= r || rx <= l) return 0;
        if (lx >= l && rx <= r) return node->sum;
        propagate(node, lx, rx);

        ll m = (lx + rx) / 2;
        ll ans = query(l, r, node->left, lx, m);
        ans += query(l, r, node->right, m, rx);
        if(ans >= mod) ans -= mod;
        return ans;
    }

    void update(ll l, ll r, int val) { update(l, r, val, root, 0, n); }

    ll query(ll l, ll r) { return query(l, r, root, 0, n); }
};