struct trie {
    struct node {
        node * arr[26];
        int prefix{},end{};
        node() {
            memset(arr,0,sizeof(arr));
        }
 
    };
    node * root=new node();
    void insert(const string &s) {
       node * curr=root;
        for (auto ch : s) {
            auto it=ch-'a';
            if (curr->arr[it]==0) {
                curr->arr[it]=new node();
            }
            curr=curr->arr[it];
            curr->prefix++;
        }
        curr->end++;
    }
    int count(const string &s,int var) {
        node * curr=root;
        for (auto ch : s) {
            auto it=ch-'a';
            if (curr->arr[it]==0) {
                return 0;
            }
            curr=curr->arr[it];
        }
        return curr->end;
        }
    void insertSuffixes(const string& s) { // insert all suffixes of a string in o(n^2))
        int n = s.size();
        for (int i = 0; i < n; ++i) {
            node* curr = root;
            for (int j = i; j < n; ++j) {
                int idx = s[j] - 'a';
                if (!curr->arr[idx]) {
                    curr->arr[idx] = new node();
                    curr->arr[idx]->end++;
                }
                curr = curr->arr[idx];
            }
        }
    }
};