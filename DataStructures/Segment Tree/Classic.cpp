class SegmentTree {
#define L 2*node+1
#define R 2*node+2
#define mid (l+r)/2
private:
    int SZ;
    vector<int>tree;

    void build(int node, int l, int r,vector<int>&arr) {
        if(l==r) {
            tree[node]=arr[l];
            return;
        }
        build(L,l,mid,arr);
        build(R,mid+1,r,arr);
        tree[node]=tree[L]+tree[R];
    }
    void update(int node,int l,int r,int idx, int value) {
        if(l==r) {
            tree[node]=value;
            return;
        }
        if(idx<=mid) {
            update(L,l,mid,idx,value);
        }
        else
            update(R,mid+1,r,idx,value);
        tree[node]=tree[L]+tree[R];
    }
    int query(int node,int l,int r,int lq,int rq) {
        if(l>rq || r<lq) {
            return 0;
        }
        if (lq <= l && r <= rq) {
            return tree[node];
        }
        return query(L,l,mid,lq,rq)+query(R,mid+1,r,lq,rq);
    }
      int find_kth(int node, int l, int r, int k) {
        if(l==r) return l;
        if(tree[L] >= k) return find_kth(L, l, mid, k);
        else return find_kth(R, mid+1, r, k - tree[L]);
    }
public:
    SegmentTree(vector<int>&arr) {
        SZ=arr.size();
        tree.assign(4*SZ,0);
        build(0,0,SZ-1,arr);
    }
    void update(int idx,int value) {
        update(0,0,SZ-1,idx,value);
    }
    int query(int l,int r) {
        return query(0,0,SZ-1,l,r);
    }
    
     int find_kth(int k) {
        if(k <= 0 || k > tree[0]) return -1; 
        return find_kth(0, 0, SZ - 1, k);
    }
#undef L
#undef R
#undef mid
};