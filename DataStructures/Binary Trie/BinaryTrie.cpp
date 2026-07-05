struct BinaryTrie {
    struct Node {
        int freq;
        Node *child[2];
        Node() {
            freq = 0;
            child[0] = child[1] = nullptr;
        }
    };
    Node *root;
    BinaryTrie() {
        root = new Node();
    }
    void insert(bitset<32> x) {
        Node *cur = root;
        for(int i=31;i>=0;i--) {
            if(cur->child[x[i]]) {
                cur = cur->child[x[i]];
            }
            else {
                cur->child[x[i]] = new Node();
                cur=cur->child[x[i]];
            }
            cur->freq++;
        }
    }

    void remove(bitset<32> x) {
        Node *cur = root;
        for(int i=31;i>=0;i--) {
            cur->child[x[i]]->freq--;
            if( cur->child[x[i]]->freq==0) {
                Node *tmp = cur->child[x[i]];
                cur->child[x[i]] = nullptr;
                delete tmp;
                break;
            }
            cur=cur->child[x[i]];
        }
    }
    void find_kth(Node *node,int i,bitset<32>&ret,int k) {
        if(i<0) return;
        if(node->child[0] && node->child[0]->freq>=k) {
            find_kth(node->child[0],i-1,ret,k);
        }
        else if(node->child[1]) {
            int new_k=k;
            if(node->child[0]) new_k-=node->child[0]->freq;
            find_kth(node->child[1],i-1,ret,new_k);
            ret[i]=1;
        }
    }
    int getmex(int XOR) { // pass XOR=0 if you want the real mex
        Node *cur = root;
        bitset<21> ans;
        for(int i=20;i>=0;i--) {
            int need=(1LL<<i);
            bool bit=((XOR>>i)&1);
            if (cur->child[bit]&&cur->child[bit]->freq==need) {// the freq mustr be for distnict lements
                cur=cur->child[!bit];
                ans[i]=true;
            }
            else {
                cur=cur->child[bit];
            }
            if(cur==nullptr) {
                break;
            }
        }
        return ans.to_ullong();
    }
};
