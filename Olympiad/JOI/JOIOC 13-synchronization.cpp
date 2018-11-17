/*
	Let's imagine that we never remove edges. This means that we can solve the problem with a simple dsu (we will keep the sizes of the components). To solve this version we will use link-cut tree.
	For each component we will keep the "answer" for its root. Obviously at the current time the answers will be the same the vertices in one component. So when we merge we will only add the values.
	Unfortunately this is wrong as the components we merge might have common vertices. But we can easily fix it by exploiting the tree structure of the graph. For each edge we will keep the size of
	the component when we last cut it. Let this number for an edge E be C(E). Then when we again add edge E to the graph, the new answer will be Answer(E.u) + Answer(E.v) - C(E). We mustn't forget
	to change C(E) when we again remove E.

	As I don't like implementing link-cut trees with splay tree, the theoretical complexity will be O(N log^2 N). However, in practice implementing them with treaps has the same speed and sometimes 
	is even faster. That's because the rotations of splay trees are slow.
*/

#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);

random_device rd;
mt19937 mt(rd());

struct node
{
	int id, sz, prior;
	node *l, *r, *par, *pp;

	node() { id = sz = prior = 0; l = r = par = pp = nullptr; }
	node(int _i)
	{
		id = _i;
		prior = mt();
		sz = 1;
		l = r = par = pp = nullptr;
	}
};

using pnode = node*;

inline int size(pnode t) { return t ? t->sz : 0; }

void pull(pnode &t)
{
	if(!t) return;
	t->sz = 1 + size(t->l) + size(t->r);
	
	t->par = nullptr;
	
	if(t->l) t->l->par = t;
	if(t->r) t->r->par = t;

	if(t->l && t->l->pp) t->pp = t->l->pp, t->l->pp = nullptr;
	if(t->r && t->r->pp) t->pp = t->r->pp, t->r->pp = nullptr;
}

void merge(pnode &t, pnode l, pnode r)
{
	if(!l) { t = r; pull(t); return; }
	if(!r) { t = l; pull(t); return; }

	if(l->prior > r->prior)
		merge(l->r, l->r, r), t = l;
	else
		merge(r->l, l, r->l), t = r;

	pull(t);
}

void split_sz(pnode t, pnode &l, pnode &r, int k, int add = 0)
{
	if(!t) { l = nullptr; r = nullptr; return; }

	int idx = add + size(t->l);
	if(idx <= k)
		split_sz(t->r, t->r, r, k, idx + 1), l = t;
	else
		split_sz(t->l, l, t->l, k, add), r = t;

	pull(t);
}

int get_pos(pnode t)
{
	if(!t) return 0;

	int ret = size(t->l);
	while(t->par)
	{
		if(t->par->r == t)
			ret += 1 + size(t->par->l);
		t = t->par;
	}

	return ret;
}

pnode treap_root(pnode t) { while(t->par) t = t->par; return t; }

pnode remove_after(pnode v)
{
	pnode root = treap_root(v), L, R, prv_pp = root->pp;
	root->pp = nullptr;
	
	split_sz(root, L, R, get_pos(v));
	if(R) R->pp = v;
	L->pp = prv_pp;
	return L;
}

void access(pnode v)
{
	v = remove_after(v);
	while(v->pp)
	{
		auto pr = v->pp;
		v->pp = nullptr;
		pr = remove_after(pr);
		merge(v, pr, v);
	}
}

int n, m, q;
pair<int, int> ed[MAXN];

void read()
{	
	cin >> n >> m >> q;
	for(int i = 1; i <= n - 1; i++)
		cin >> ed[i].first >> ed[i].second;
}

int last[MAXN];
int state_e[MAXN];
int ans[MAXN];
pnode ver[MAXN];

int dep[MAXN];
vector<int> adj[MAXN];

void pre_dfs(int u, int pr = -1, int d = 0)
{
	dep[u] = d;
	for(int v: adj[u])
		if(v != pr)
			pre_dfs(v, u, d + 1);
}

int root(int u)
{
	access(ver[u]);
	auto p = treap_root(ver[u]);
	while(p->l) p = p->l;
	return p->id;
}

void cut(int u, int v)
{
	if(dep[u] < dep[v]) swap(u, v);
	access(ver[u]);
	access(ver[v]);
	treap_root(ver[u])->pp = nullptr;
}

void link(int u, int v)
{
	if(dep[u] < dep[v]) swap(u, v);
	access(ver[u]);
	treap_root(ver[u])->pp = ver[v];
}

void solve()
{
	for(int i = 1; i < n; i++)
	{
		adj[ed[i].first].push_back(ed[i].second);
		adj[ed[i].second].push_back(ed[i].first);
	}

	pre_dfs(1);
	for(int i = 1; i <= n; i++)
		ans[i] = 1, ver[i] = new node(i);

	while(m--)
	{
		int i;
		cin >> i;

		state_e[i] ^= 1;

		if(state_e[i] == 0)
		{
			last[i] = ans[root(ed[i].first)];
			cut(ed[i].first, ed[i].second);

			ans[root(ed[i].first)] = last[i];
			ans[root(ed[i].second)] = last[i];
		}
		else
		{
			int nw = ans[root(ed[i].first)] + ans[root(ed[i].second)] - last[i];
			link(ed[i].first, ed[i].second);
			
			ans[root(ed[i].first)] = nw;
		}
	}

	while(q--)
	{
		int u;
		cin >> u;
		cout << ans[root(u)] << endl;
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

