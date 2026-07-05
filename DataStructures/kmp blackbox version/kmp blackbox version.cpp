vector<int> computeprefix(string pat) {
    int m=pat.size();
    vector<int> longestprefix(m);
    for(int i=1,k=0;i<m;i++) {
        while(k>0 && pat[k]!=pat[i]) {
            k=longestprefix[k-1];
        }
        if(pat[k]==pat[i]) {
            longestprefix[i]=++k;
        }
        else {
            longestprefix[i]=k;
        }
    }
    return longestprefix;
}
int longest_suffix_palindrome(string pat) {
    string temp=pat;
    reverse(temp.begin(),temp.end());
    temp.push_back('@');
    temp+=pat;
    pat=temp;
    int m=pat.size();
    vector<int> longestprefix(m);
    for(int i=1,k=0;i<m;i++) {
        while(k>0 && pat[k]!=pat[i]) {
            k=longestprefix[k-1];
        }
        if(pat[k]==pat[i]) {
            longestprefix[i]=++k;
        }
        else {
            longestprefix[i]=k;
        }
    }
    return longestprefix[m-1];
}
int longest_prefix_palindrome(string pat) {
    string temp=pat;
    reverse(temp.begin(),temp.end());
    pat.push_back('@');
    pat+=temp;
    int m=pat.size();
    vector<int> longestprefix(m);
    for(int i=1,k=0;i<m;i++) {
        while(k>0 && pat[k]!=pat[i]) {
            k=longestprefix[k-1];
        }
        if(pat[k]==pat[i]) {
            longestprefix[i]=++k;
        }
        else {
            longestprefix[i]=k;
        }
    }
    return longestprefix[m-1];
}
int minimum_number_of_letters_to_make_string_palindrome(string pat) {
    int x=longest_prefix_palindrome(pat);
    int y=longest_suffix_palindrome(pat);
    if (x<y) {
        swap(x,y);
    }
    return pat.size()-x;
}
int minimum_number_of_Repetition (string &s) {

    vector<int> longest_prefix=computeprefix(s);
    if (s.size()%(s.size()-longest_prefix[s.size()-1])==0) {
        return (s.size()-longest_prefix[s.size()-1]);
    }
    else {
        return 1;
    }
}
vector<int> prefixesFrequency(string pat) {
    int m=pat.size();
    vector<int> pre=computeprefix(pat);
    vector<int> freq(m,1);
    for(int i=0;i<m;i++) {
        int k=pre[i];
        while(k>0) {
            freq[k-1]++;
            k=pre[k-1];
        }
    }
    return freq;
}
vector<int> kmp(string s,string pat) {
    int n=s.length();
    int m=pat.length();
    vector<int> longesprefix=computeprefix(pat);
    vector<int> ans;
    for(int i=0,k=0;i<n;i++) {
        while(k>0 && pat[k]!=s[i]) {
            k=longesprefix[k-1];
        }
        if (pat[k]==s[i]) {
            k++;
        }
        if (k==m) {
            ans.push_back(i-m+1);
            k=longesprefix[k-1];
        }
    }
    return ans;
}