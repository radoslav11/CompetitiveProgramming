/*
   We will use sweep line to solve the problem. We split the stores into 2 queries: 
   1) Add store i at time a[i]
   2) Remove store i at time b[i] + 1
   We will also have queries in the sweep line. Everything will be sorted by time in increasing order.

   Now to handle queries we will maintain K sets - the available positions of j-type stores. Then if A and B are two consecutive stores, the closest elements to all positions in [A; B] are A or B.
   Then let's have a two segment trees wtih sets - one for closest elements to the left and one for closest elements to the right. Now addition of store with type X will be done like that:

   1) Let A <= X <= B and A and B are the closest stores of the same type. 
   2) We remove the interval [A; B] from the DS.
   3) We add the intervals [A; X] and [X; B].

   Adding or removing an interval is done by finding the middle position and then considering the two ranges - [L; Mid] and [Mid + 1; R].

   The complexity will be O(N * log N * log N).

   As sets are slow, we will compress the input in each segment tree node beforehand and then use priority queue instead of sets.

   Unfortunately the above data structure was too slow. So my second idea is to change the data structure to two simple treaps and do binary search on them. 
   The complexity will be O(N log N) this way. 

   Again unfortunately the treap solution was too slow (it got 47). So the third idea is to make the data structure offline. Then the treap can be replaced with segment tree.
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
const int MAXN = (1 << 21);
const int inf = (int)1e9 + 42;

vector<pair<int, int> > Li, Li2;

struct segment_tree_L
{
	vector<pair<int, int> > a;

	struct node
	{
		int mx;
		node() { mx = -inf; }
		node(int val) { mx = val; }
	};

	node temp, broken;

	node merge(node l, node r)
	{
		temp.mx = max(l.mx, r.mx);
		return temp;
	}

	int bound_L[4 * MAXN], bound_R[4 * MAXN];

	node tr[4 * MAXN];
	int CNT[MAXN], lf[MAXN];

	void init(int l, int r, int idx)
	{
		bound_L[idx] = l;
		bound_R[idx] = r;
		if(l == r)
		{
			CNT[l] = 0;
			lf[l] = idx;
			tr[idx] = node();
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void change_cnt(int pos, int d)
	{
		CNT[pos] += d;
		if(CNT[pos] == 0 || CNT[pos] == d)
		{
			int idx = lf[pos];
			tr[idx].mx = CNT[pos] ? a[pos].second : -inf;

			while(idx)
			{
				idx = (idx - 1) >> 1;
				tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
			}
		}
	}

	void add(int pos, int l, int r, int idx)
	{
		if(l > pos || r < pos)
			return;

		if(l == r && l == pos)
		{
			CNT[l]++;
			tr[idx].mx = CNT[l] ? a[l].second : -inf;
			return;
		}

		int mid = (l + r) >> 1;
		add(pos, l, mid, 2 * idx + 1);
		add(pos, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void rem(int pos, int l, int r, int idx)
	{
		if(l > pos || r < pos)
			return;

		if(l == r && l == pos)
		{
			CNT[l]--;
			tr[idx].mx = CNT[l] ? a[l].second : -inf;
			return;
		}

		int mid = (l + r) >> 1;
		rem(pos, l, mid, 2 * idx + 1);
		rem(pos, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void get_nodes(int qL, int qR, int l, int r, int idx, vector<int> &li)
	{
		if(l > qR || r < qL) return;
		if(qL <= l && r <= qR)
		{
			li.push_back(idx);
			return;
		}

		int mid = (l + r) >> 1;
		get_nodes(qL, qR, l, mid, 2 * idx + 1, li);
		get_nodes(qL, qR, mid + 1, r, 2 * idx + 2, li);
	}

	int get_right(int l, int r, int idx, int X)
	{
		if(l == r) return l;
		int mid = (l + r) >> 1;
		if(tr[2 * idx + 1].mx >= X) return get_right(l, mid, 2 * idx + 1, X);
		else return get_right(mid + 1, r, 2 * idx + 2, X);
	}

	int N;

	void init()
	{
		N = SZ(Li);
		a = Li;
		init(0, N - 1, 0);
	}

	void add_interval(int l, int r)
	{
		int pos = L_B(ALL(a), make_pair(l, r)) - a.begin();
		change_cnt(pos, 1);
	}

	void rem_interval(int l, int r)
	{
		int pos = L_B(ALL(a), make_pair(l, r)) - a.begin();
		change_cnt(pos, -1);
	}

	int query(int x)
	{
		return x - a[get_right(0, N - 1, 0, x)].first;
	}

} L;

struct segment_tree_R
{
	vector<pair<int, int> > a;

	struct node
	{
		int mn;
		node() { mn = inf; }
		node(int val) { mn = val; }
	};

	node temp, broken;

	node merge(node l, node r)
	{
		temp.mn = min(l.mn, r.mn);
		return temp;
	}

	int bound_L[4 * MAXN], bound_R[4 * MAXN];

	node tr[4 * MAXN];
	int CNT[MAXN], lf[MAXN];

	void init(int l, int r, int idx)
	{
		bound_L[idx] = l;
		bound_R[idx] = r;
		if(l == r)
		{
			CNT[l] = 0;
			lf[l] = idx;
			tr[idx] = node();
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void change_cnt(int pos, int d)
	{
		CNT[pos] += d;
		if(CNT[pos] == 0 || CNT[pos] == d)
		{
			int idx = lf[pos];
			tr[idx].mn = CNT[pos] ? a[pos].second : inf;

			while(idx)
			{
				idx = (idx - 1) >> 1;
				tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
			}
		}
	}

	void add(int pos, int l, int r, int idx)
	{
		if(l > pos || r < pos)
			return;

		if(l == r && l == pos)
		{
			CNT[l]++;
			tr[idx].mn = CNT[l] ? a[l].second : inf;
			return;
		}

		int mid = (l + r) >> 1;
		add(pos, l, mid, 2 * idx + 1);
		add(pos, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void rem(int pos, int l, int r, int idx)
	{
		if(l > pos || r < pos)
			return;

		if(l == r && l == pos)
		{
			CNT[l]--;
			tr[idx].mn = CNT[l] ? a[l].second : inf;
			return;
		}

		int mid = (l + r) >> 1;
		rem(pos, l, mid, 2 * idx + 1);
		rem(pos, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void get_nodes(int qL, int qR, int l, int r, int idx, vector<int> &li)
	{
		if(l > qR || r < qL) return;
		if(qL <= l && r <= qR)
		{
			li.push_back(idx);
			return;
		}

		int mid = (l + r) >> 1;
		get_nodes(qL, qR, l, mid, 2 * idx + 1, li);
		get_nodes(qL, qR, mid + 1, r, 2 * idx + 2, li);
	}

	int get_left(int l, int r, int idx, int X)
	{
		if(l == r) return l;
		int mid = (l + r) >> 1;
		if(tr[2 * idx + 2].mn <= X) return get_left(mid + 1, r, 2 * idx + 2, X);
		else return get_left(l, mid, 2 * idx + 1, X);
	}

	int N;

	void init()
	{
		N = SZ(Li);
		a = Li2;
		init(0, N - 1, 0);
	}

	void add_interval(int l, int r)
	{
		int pos = L_B(ALL(a), make_pair(r, l)) - a.begin();
		change_cnt(pos, 1);
	}

	void rem_interval(int l, int r)
	{
		int pos = L_B(ALL(a), make_pair(r, l)) - a.begin();
		change_cnt(pos, -1);
	}

	int query(int x)
	{
		return a[get_left(0, N - 1, 0, x)].first - x;
	}

} R;

int read_int();

int n, k, q;

struct event
{
	int type;
	int T, x, tp, idx;

	event() { type = tp = T = x = 0; idx = -1; }
	event(int t, int Tm, int X, int i, int pp = -1)
	{
		type = t;
		T = Tm;
		x = X;
		idx = i;
		tp = pp;
	}
};

bool cmp(event a, event b) 
{ 
	if(a.T != b.T) return a.T < b.T; 
	return a.type < b.type;
}

vector<event> Ev;
int answer[MAXN];

void read()
{
	n = read_int();
	k = read_int();
	q = read_int();

	for(int i = 0; i < n; i++)
	{
		int x, t, a, b;
		x = read_int();
		t = read_int();
		a = read_int();
		b = read_int();

		Ev.push_back(event(0, a, x, i, t));
		Ev.push_back(event(1, b + 1, x, i, t));
	}

	for(int i = 0; i < q; i++)
	{
		int x, t;
		x = read_int();
		t = read_int();
		Ev.push_back(event(2, t, x, i));
	}
}

set<pair<int, int> > ST[MAXN];

void add_interval(int l, int r)
{
	int mid = (l + r) / 2;
	if(l <= mid) L.add_interval(l, mid);
	if(mid + 1 <= r) R.add_interval(mid + 1, r);
}

void rem_interval(int l, int r)
{
	int mid = (l + r) / 2;
	if(l <= mid) L.rem_interval(l, mid);
	if(mid + 1 <= r) R.rem_interval(mid + 1, r);
}

int query(int x) { return max(L.query(x), R.query(x)); }

void add(int y, int x, int i)
{
	auto it = ST[y].insert({x, i}).first;
	auto aft = next(it);
	auto bef = prev(it);

	rem_interval(bef->first, aft->first);
	add_interval(bef->first, x);
	add_interval(x, aft->first);
}

void rem(int y, int x, int i)
{
	auto aft = ST[y].erase(ST[y].find({x, i}));
	auto bef = prev(aft);

	rem_interval(bef->first, x);
	rem_interval(x, aft->first);
	add_interval(bef->first, aft->first);
}

void prep_add_interval(int l, int r)
{
	int mid = (l + r) / 2;
	if(l <= mid) Li.push_back({l, mid});
	if(mid + 1 <= r) Li2.push_back({r, mid + 1});
}

void prep_add(int y, int x, int i)
{
	auto it = ST[y].insert({x, i}).first;
	auto aft = next(it);
	auto bef = prev(it);

	prep_add_interval(bef->first, x);
	prep_add_interval(x, aft->first);
}

void prep_rem(int y, int x, int i)
{
	auto aft = ST[y].erase(ST[y].find({x, i}));
	auto bef = prev(aft);

	prep_add_interval(bef->first, aft->first);
}

void solve()
{
	for(int i = 1; i <= k; i++)
		ST[i].insert({-inf, -1}), ST[i].insert({inf, -1});

	prep_add_interval(-inf, inf);

	sort(ALL(Ev), cmp);

	for(auto it: Ev)
		if(it.type == 0)
			prep_add(it.tp, it.x, it.idx);
		else if(it.type == 1)
			prep_rem(it.tp, it.x, it.idx);

	sort(ALL(Li));
	Li.erase(unique(ALL(Li)), Li.end());

	sort(ALL(Li2));
	Li2.erase(unique(ALL(Li2)), Li2.end());

	L.init();
	R.init();

	for(int i = 0; i < k; i++)
		add_interval(-inf, inf);

	for(auto it: Ev)
		if(it.type == 0)
			add(it.tp, it.x, it.idx);
		else if(it.type == 1)
			rem(it.tp, it.x, it.idx);
		else 
			answer[it.idx] = query(it.x);

	for(int i = 0; i < q; i++)
		if(answer[i] < (int)2e8) cout << answer[i] << endl;
		else cout << -1 << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

const int maxl = 100000;
char buff[maxl];
int ret_int, pos_buff = 0;

void next_char() { if(++pos_buff == maxl) fread(buff, 1, maxl, stdin), pos_buff = 0; }

int read_int()
{
	ret_int = 0;
	for(; buff[pos_buff] < '0' || buff[pos_buff] > '9'; next_char());
	for(; buff[pos_buff] >= '0' && buff[pos_buff] <= '9'; next_char())
		ret_int = ret_int * 10 + buff[pos_buff] - '0';
	return ret_int;
}
