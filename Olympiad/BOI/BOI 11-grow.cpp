/*
	All queries can be handled with a simple implicit treap. The complexity is O((N + M) log N).
*/

#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define pb push_back

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);

random_device rd;
mt19937 mt(rd());

struct node
{
	int sz, prior, value, lazy;
	node *l, *r;
	node() { lazy = 0; value = 0; sz = 0; prior = 0; l = nullptr; r = nullptr; }
	node(int v) { lazy = 0; value = v; sz = 1; prior = mt(); l = nullptr; r = nullptr; }
};

typedef node* pnode;

inline int size(pnode v) { return v ? v->sz : 0; }

void push(pnode &t)
{
	if(!t) return;
	if(t->lazy)
	{
		t->value += t->lazy;
		if(t->l) t->l->lazy += t->lazy;
		if(t->r) t->r->lazy += t->lazy;
		t->lazy = 0;
	}
}

void pull(pnode &v) 
{ 
	if(!v) return;
	push(v->l);
	push(v->r);
	v->sz = size(v->l) + size(v->r) + 1; 
}

void merge(pnode &t, pnode l, pnode r)
{
	push(l), push(r);
	if(!l) { t = r; return; }
	if(!r) { t = l; return; }

	if(l->prior > r->prior)
		merge(l->r, l->r, r), t = l;
	else
		merge(r->l, l, r->l), t = r;

	pull(t);
}

void split(pnode t, pnode &l, pnode &r, int k)
{
	push(t);
	if(!t) { l = nullptr; r = nullptr; return; }

	if(t->value <= k)
		split(t->r, t->r, r, k), l = t;
	else
		split(t->l, l, t->l, k), r = t;

	pull(t);
}

void merge_op(pnode &t, pnode l, pnode r)
{
	push(l), push(r);
	if(!l) { t = r; return;  }
	if(!r) { t = l; return;  }

	if(l->prior < r->prior)
		swap(l, r);

	pnode L, R;
	split(r, L, R, l->value - mt() % 2);
	merge_op(l->r, l->r, R);
	merge_op(l->l, L, l->l);

	t = l;
	pull(t);
}

void split_sz(pnode t, pnode &l, pnode &r, int k, int add = 0)
{
	push(t);
	if(!t) { l = nullptr; r = nullptr; return; }

	int idx = add + size(t->l);
	if(idx <= k)
		split_sz(t->r, t->r, r, k, idx + 1), l = t;
	else
		split_sz(t->l, l, t->l, k, add), r = t;

	pull(t);
}

pnode root;

void insert(int x)
{
	pnode l, r, v = new node(x);
	split(root, l, r, x);
	merge(l, l, v);
	merge(root, l, r);
}

void add(int h, int c)
{
	pnode L, R, mid;
	split(root, L, R, h - 1);
	split_sz(R, mid, R, c - 1);

	if(mid) mid->lazy += 1;

	merge(root, L, mid);
	merge_op(root, root, R);
}

int query(int x, int y)
{
	pnode l, r, mid;
	split(root, l, r, x - 1);
	split(r, mid, r, y);

	int ans = size(mid);

	merge(root, l, mid);
	merge(root, root, r);

	return ans;
}

int n, m;

void read()
{
	cin >> n >> m;
	for(int i = 1; i <= n; i++)
	{
		int h;
		cin >> h;
		insert(h);
	}
}

void solve()
{
	while(m--)
	{
		char t;
		int a1, a2;
		cin >> t >> a1 >> a2;
		if(t == 'F') add(a2, a1);
		else cout << query(a1, a2) << endl;
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

