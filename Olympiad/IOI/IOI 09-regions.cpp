/*
	We will split regions in two types - heavy and light. Formally if CountVertices(R) >= B then R is heavy and if CountVertices(R) < B it's light.

	We have 3 cases:
	1) R1 and R2 are light. We can easly solve such queries in O((CountVertices(R1) + CountVertices(R2)) * log N) < O(B * log N) 
	2) R1 is heavy 
	3) R2 is heavy

	Let's precompute the answers for all queries of type 2 and 3. The main observation is that there are O(N * N / B) such queries and we can solve them all in O(N * N / B * log N) time. 
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
const int MAXN = (int)200042;
const int MAXR = 25042;
const int B = 500;

struct fenwick
{
	int tr[MAXN];
	int n;

	void init(int sz)
	{
		n = sz;
		for(int i = 0; i <= n; i++)
			tr[i] = 0;
	}

	void add(int idx, int x)
	{
		for(; idx <= n; idx += (idx & -idx))
			tr[idx] += x;
	}

	int query(int idx)
	{
		int ret = 0;
		for(; idx > 0; idx -= (idx & -idx))
			ret += tr[idx];
		return ret;
	}

	int query(int l, int r) { return query(r) - query(l - 1); }
};

struct fenwick_range
{
	int tr[MAXN];
	int n;

	void init(int sz)
	{
		n = sz + 1;
		for(int i = 0; i <= n; i++)
			tr[i] = 0;
	}

	void add(int idx, int x)
	{
		for(; idx <= n; idx += (idx & -idx))
			tr[idx] += x;
	}

	int query(int idx)
	{
		int ret = 0;
		for(; idx > 0; idx -= (idx & -idx))
			ret += tr[idx];
		return ret;
	}

	void update(int l, int r, int x)
	{
		add(l, x);
		add(r + 1, -x);
	}
};

fenwick T1;
fenwick_range T2;

int n, k, q;
vector<int> r[MAXN], adj[MAXN];

void read()
{
	cin >> n >> k >> q;

	int dummy;
	cin >> dummy;

	r[dummy].push_back(1);

	for(int i = 2; i <= n; i++)
	{
		int par;
		cin >> par >> dummy;
		adj[par].push_back(i);
		r[dummy].push_back(i);
	}
}

int st[MAXN], en[MAXN], dfs_time;

void dfs(int u, int pr)
{
	st[u] = ++dfs_time;
	for(int v: adj[u])
		if(v != pr)
			dfs(v, u);
	en[u] = dfs_time;
}

int h_id[MAXR], rv[B];
int64_t answer_t2[B][MAXR];
int64_t answer_t3[MAXR][B];

void solve()
{
	dfs(1, 1);
	T1.init(n);
	T2.init(n);

	int id = 0;
	for(int i = 1; i <= k; i++)
		if(SZ(r[i]) >= B) 
			h_id[i] = ++id, rv[id] = i;

	for(int i = 1; i <= id; i++)
	{
		int r1 = rv[i];
		for(int i: r[r1])
			T2.update(st[i], en[i], 1);

		for(int r2 = 1; r2 <= k; r2++)
			for(int v: r[r2])
				answer_t2[i][r2] += T2.query(st[v]);

		for(int i: r[r1])
			T2.update(st[i], en[i], -1);
	}
	
	for(int i = 1; i <= id; i++)
	{
		int r2 = rv[i];
		for(int i: r[r2])
			T1.add(st[i], 1);

		for(int r1 = 1; r1 <= k; r1++)
			for(int v: r[r1])
				answer_t3[r1][i] += T1.query(st[v], en[v]);

		for(int i: r[r2])
			T1.add(st[i], -1);
	}

	while(q--)
	{
		int r1, r2;
		cin >> r1 >> r2;

		if(h_id[r1]) cout << answer_t2[h_id[r1]][r2];
		else if(h_id[r2]) cout << answer_t3[r1][h_id[r2]];
		else
		{
			for(int i: r[r2]) T1.add(st[i], 1);
			int64_t answer = 0;
			for(int j: r[r1]) answer += T1.query(st[j], en[j]);
			for(int i: r[r2]) T1.add(st[i], -1);

			cout << answer << endl;
		}
		
		cout << endl << flush;
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

