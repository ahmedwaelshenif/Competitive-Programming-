struct PalindromeTree {
    struct Node {
        int len;                          // Length of palindrome
        int suff_link;                    // Suffix link (failure link)
        map<char, int> next;              // Transitions to larger palindromes
        int count;                        // Occurrence count of this palindrome
        int num;                          // Number of palindromes ending here
        int first_pos;                    // Last position where this palindrome appears

        Node(int l) : len(l), suff_link(0), count(0), num(0), first_pos(-1) {}
    };

    string s;                             // Input string (grows online)
    vector<Node> tree;                    // Palindrome tree nodes
    int suff;                             // Index of the largest suffix-palindrome node

    // Constructor — O(1)
    PalindromeTree() {
        // Root -1 (imaginary node)
        tree.emplace_back(-1);
        tree[0].suff_link = 0;

        // Root 0 (empty string)
        tree.emplace_back(0);
        tree[1].suff_link = 0;

        suff = 1; // Start with root (empty string)
    }

    // Add a character at the end — O(1) amortized per character
    void add_char(char c) {
        s += c;
        int pos = s.size() - 1;

        int curr = suff;
        while (true) {
            int curlen = tree[curr].len;
            if (pos - curlen - 1 >= 0 && s[pos - curlen - 1] == c)
                break;
            curr = tree[curr].suff_link;
        }

        if (tree[curr].next.count(c)) {
            suff = tree[curr].next[c];
            tree[suff].count++;
            return;
        }

        int new_node = tree.size();
        tree.emplace_back(tree[curr].len + 2);
        tree[new_node].first_pos = pos;

        tree[curr].next[c] = new_node;

        if (tree[new_node].len == 1) {
            tree[new_node].suff_link = 1;
        } else {
            int link = tree[curr].suff_link;
            while (true) {
                int linklen = tree[link].len;
                if (pos - linklen - 1 >= 0 && s[pos - linklen - 1] == c)
                    break;
                link = tree[link].suff_link;
            }
            tree[new_node].suff_link = tree[link].next[c];
        }

        tree[new_node].count = 1;
        tree[new_node].num = 1 + tree[tree[new_node].suff_link].num;
        suff = new_node;
    }

    // Build tree from string — O(n)
    void build(const string &str) {
        for (char c : str) add_char(c);
    }

    // Count total occurrences of each palindrome — O(n)
    void count_occurrences() {
        for (int i = tree.size() - 1; i >= 2; i--) {
            int link = tree[i].suff_link;
            tree[link].count += tree[i].count;
        }
    }

    // Count distinct palindromes — O(1)
    int count_distinct_palindromes() const {
        return tree.size() - 2; // excluding the two roots
    }

    // Get the longest palindromic substring — O(1)
    string longest_palindrome() const {
        int max_len = 0;
        int idx = -1;
        for (int i = 2; i < tree.size(); ++i) {
            if (tree[i].len > max_len) {
                max_len = tree[i].len;
                idx = i;
            }
        }
        if (idx == -1) return "";
        int end = tree[idx].first_pos;
        return s.substr(end - max_len + 1, max_len);
    }

    // Debug: print all palindromes and their data — O(n²) (for output only)
    void print_palindromes() const {
        for (int i = 2; i < tree.size(); ++i) {
            int len = tree[i].len;
            int end = tree[i].first_pos;
            string pal = s.substr(end - len + 1, len);

            cout << "Palindrome: " << pal
                 << ", Length: " << len
                 << ", Count: " << tree[i].count
                 << ", Ends at index: " << end
                 << ", Suffix link to len " << tree[tree[i].suff_link].len
                 << '\n';
        }
    }
    int total_palindromic_substrings() const {
        //count_occurrences(); put this part in main before u call the function
        int total = 0;
        for (int i = 2; i < tree.size(); ++i) {
            total += tree[i].count;
        }
        return total;
    }
};