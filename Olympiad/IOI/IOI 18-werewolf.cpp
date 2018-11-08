/*
    We build a prefix and suffix tree with dsu, by incrementing them by 1 vertex at each step. Now queries become:
    Do two subtrees have a common vertex (the first subtree is from the prefix tree and the second subtree is from the suffix tree).
    This can be done with DFS order + dsu on tree by keeping a set for each vertex. The complexity will be O(N log^2 N + Q log N).
*/
 
#include <bits/stdc++.h>
#include "werewolf.h"
//#include "grader.cpp"
 
#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
 
using namespace std;
template<class T, class T1> int chkmax(T &x, const T1 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T1> int chkmin(T &x, const T1 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 18);
 
int tp[MAXN], sz[MAXN], par[MAXN];
 
void init(int n)
{
    for(int i = 0; i <= n; i++)
        par[i] = i, sz[i] = 1, tp[i] = i;
}
 
int root(int x) { return x == par[x] ? x : (par[x] = root(par[x])); }
 
bool connected(int u, int v)
{
    return root(u) == root(v);
}
 
void unite(int u, int v)
{
    u = root(u), v = root(v);
    if(u == v) return;
    if(sz[u] > sz[v]) swap(u, v);
    par[u] = v;
    sz[v] += sz[u];
}
 
struct Tree
{
    vector<int> adj[MAXN];
    int st[MAXN], en[MAXN], dfs_time;
 
    void add_edge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
 
    void pre_dfs(int u, int pr)
    {
        st[u] = ++dfs_time;
        for(int v: adj[u])
            if(v != pr)
                pre_dfs(v, u);
        en[u] = dfs_time;
    }
 
    void order(int root)
    {
        dfs_time = 0;
        pre_dfs(root, root);
    }
} t1, t2;
 
vector<int> adj[MAXN];
vector<int> pref[MAXN], suff[MAXN];
vector<int> ans;
int vert1[MAXN], vert2[MAXN];
 
set<int> st[MAXN];
vector<int> que[MAXN];
 
void solve(int u, int pr = -1)
{
    st[u].insert(t1.st[u]);
    for(int v: t2.adj[u])
        if(v != pr)
        {
            solve(v, u);
            if(st[u].size() < st[v].size()) swap(st[u], st[v]);
 
            for(int x: st[v])
                st[u].insert(x);
 
            st[v].clear();
        }
 
    for(int i: que[u])
    {
        int L = t1.st[vert1[i]], R = t1.en[vert1[i]];
 
        auto it = st[u].lower_bound(L);
        if(it != st[u].end() && *it <= R)
            ans[i] = 1;
    }
}
 
std::vector<int> check_validity(int n, std::vector<int> X, std::vector<int> Y, std::vector<int> S, std::vector<int> E, std::vector<int> L, std::vector<int> R)
{
    int q = S.size(), m = X.size();
    ans.assign(q, 0);
 
    for(int i = 0; i < m; i++)
    {
        int u = X[i], v = Y[i];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
 
    for(int i = 0; i < q; i++)
    {
        suff[L[i]].push_back(i);
        pref[R[i]].push_back(i);
    }
 
    init(n);
    for(int i = 0; i < n; i++)
    {
        for(int v: adj[i])
            if(i > v)
                if(!connected(i, v))
                {
                    t1.add_edge(tp[root(i)], tp[root(v)]);
                    unite(i, v);
                    tp[root(i)] = i;
                }
 
        for(int it: pref[i])
            vert1[it] = tp[root(E[it])];
    }
 
    int r1 = tp[root(0)];
    t1.order(r1);
 
    init(n);
    for(int i = n - 1; i >= 0; i--)
    {
        for(int v: adj[i])
            if(i < v)
                if(!connected(i, v))
                {
                    t2.add_edge(tp[root(i)], tp[root(v)]);
                    unite(i, v);
                    tp[root(i)] = i;
                }
 
        for(int it: suff[i])
            vert2[it] = tp[root(S[it])];
    }
 
    int r2 = tp[root(0)];
    t2.order(r2);
 
    for(int i = 0; i < q; i++)
        que[vert2[i]].push_back(i);
 
    solve(r2);
 
    return ans;
}
