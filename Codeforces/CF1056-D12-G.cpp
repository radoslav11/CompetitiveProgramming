/*
   We notice that there are only N^2 important states - (postition, t mod N). Let's make T mod N steps and see where we will be after that. This can be done in the most stupid way in O(N) time. 
   Also let's for each position store the position we will arrive at after N steps. If this value for position x is to[x] the answer of our problem will be to[to[...[x]...]] (applied T / N times).
   Then after calculating the to[] array, we can simply find the cycle length of to[] and the reminder of (T / N) when divided by it. Then (T / N) <= N and we can easily find the answer in O(N).

   Now the hard part is to find the to[] array. We will go backwards. Initially set to[x] = x. Now we will use persistent treap (actually RBST) to find the actual to values. The main observation
   is that when decreasing T the to[] array will change just in two intervals. Then we can do the movements with a couple of merges and splits. The complexity will be O(N log N).

   As persistent treaps (RBST) tend to to increase their depth fast, we will rebuild our structure a couple of times.
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
template<class T, class T1> int chkmax(T &x, const T1 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T1> int chkmin(T &x, const T1 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);
const int MAX_MEM = (1 << 23);
const int B = 5000;

random_device rd;
mt19937 mt(rd());

struct node
{
	int sz, id;
	node *l, *r;

	node() { sz = id = 0; l = r = nullptr; }
	node(int i)
	{
		l = nullptr;
		r = nullptr;
		id = i;
		sz = 1;
	}
};

using pnode = node*;

int cnt_nodes = 0;
pnode ptr_list[MAX_MEM];

void init_memory()
{
	cnt_nodes = 0;
	for(int i = 0; i < MAX_MEM; i++)
		ptr_list[i] = new node();
}

pnode new_node(int id)
{
	ptr_list[cnt_nodes]->l = ptr_list[cnt_nodes]->r = nullptr;
	ptr_list[cnt_nodes]->sz = 1, ptr_list[cnt_nodes]->id = id;
	return ptr_list[cnt_nodes++];
}

inline int size(pnode t) { return t ? t->sz : 0; }

bool hey(int l, int r) { return (int)mt() % (l + r) < l; }

void pull(pnode &t)
{
	if(!t) return;
	t->sz = 1 + size(t->l) + size(t->r);
}

pnode copy(pnode &t)
{
	if(!t) return nullptr;

	pnode ret = new_node(0);
	ret->id = t->id;
	ret->l = t->l;
	ret->r = t->r;
	ret->sz = t->sz;

	return ret;
}

void split(pnode t, pnode &l, pnode &r, int k, int add = 0)
{
	if(!t) { l = r = nullptr; return; }

	int idx = size(t->l) + add;
	if(idx <= k)
	{
		l = copy(t);
		split(l->r, l->r, r, k, idx + 1);
		pull(l);
	}
	else
	{
		r = copy(t);
		split(r->l, l, r->l, k, add);
		pull(r);
	}
}

void merge(pnode &t, pnode l, pnode r)
{
	if(!l) { t = copy(r); return; }
	if(!r) { t = copy(l); return; }

	if(hey(size(l), size(r)))
		t = copy(l), merge(t->r, t->r, r);
	else
		t = copy(r), merge(t->l, l, t->l);

	pull(t);
}

vector<int> tmp_rec;

void get_li_tmp(pnode t)
{
	if(!t) return;
	get_li_tmp(t->l);
	tmp_rec.pb(t->id);
	get_li_tmp(t->r);
}

vector<int> get_li(pnode t)
{
	tmp_rec.clear();
	get_li_tmp(t);
	return tmp_rec;
}

pnode build(int l, int r, vector<int> &li)
{
	if(l > r) return nullptr;

	int mid = (l + r) >> 1;
	
	pnode ret = new_node(li[mid]);
	ret->l = build(l, mid - 1, li);
	ret->r = build(mid + 1, r, li);

	pull(ret);

	return ret;
}

int n, m, start_pos;
int64_t t;

void read()
{
	cin >> n >> m;
	cin >> start_pos >> t;
	start_pos--;
}

int to[MAXN];

void dfs(pnode t, int p = 0)
{
	if(!t) return;
	dfs(t->l, p);
	to[p] = t->id;
	dfs(t->r, p + size(t->l) + 1);
}

pnode root = nullptr;
vector<int> tmpp;

void rebuild()
{
	tmpp = get_li(root);
	cnt_nodes = 0;
	root = build(0, n - 1, tmpp);
}

pnode get(int l, int r)
{
	if(l < 0) 
	{
		pnode ret;
		merge(ret, get(n + l, n - 1), get(0, r));
		return ret;
	}

	if(r >= n)
	{
		pnode ret;
		merge(ret, get(l, n - 1), get(0, r - n));
		return ret;
	}

	pnode dummy, dummy2, ret, mid;
	split(root, mid, dummy, r);	
	split(mid, dummy2, ret, l - 1);
	return ret;
}

void solve()
{
	while(t % n)
	{
		if(start_pos < m) start_pos = (start_pos + (t % n)) % n;
		else start_pos = (start_pos + n - (t % n)) % n;
		t--;
	}

	t /= n;
	
	for(int i = 0; i < n; i++) tmpp.pb(i);
	root = build(0, n - 1, tmpp);

	for(int i = 0; i < n; i++)
	{
		pnode L = get(i, m + i - 1);
		pnode R = get(m - i, n - 1 - i);
		
		merge(root, L, R);
		if(i && i % B == 0) 
			rebuild();
	}

	vector<int> clist = get_li(root);
	for(int i = 0; i < n; i++) to[i] = clist[i];

	for(int cnt = 0; t > 0 && cnt < n; cnt++, t--)
		start_pos = to[start_pos];

	if(t != 0)
	{
		int cycle_len = 1, u = to[start_pos];
		while(u != start_pos)
		{
			cycle_len++;
			u = to[u];
		}

		t %= cycle_len;
		while(t--)
			start_pos = to[start_pos];
	}

	cout << start_pos + 1 << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	
	init_memory();

	read();
	solve();
	return 0;
}
