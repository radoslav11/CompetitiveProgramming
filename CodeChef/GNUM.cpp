/*
   Let us create a bipartite graph. On the left side we will have all pairs (i, j) such that A[i] > B[j]. Similarly, the vertices on the right part are all pairs (i, j) with A[i] < B[j]. 
   Then we add all possible edges between these parts. An edge is possible if gcd(A[i_left], B[j_left], B[i_righ], A[j_right]) != 1. Now the answer will be the maximal matching. 
   To improve the solution we will create at most N * 20 new vertices - for all distinct prime divisors in the input. We will add these vertices in the "middle" of our graph. 
   The last part of the solution will be to connect these vertices to the correspoinding vertices from the left and right part. Now the maximum flow in this graph is the answer.
   If we use Dinic's algorithm the complexity will be O(K sqrt K) where K is the number of vertices, because all edges have capacity 1.
   */

#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 19);

struct max_flow
{
	static const int inf = (int)1e9;

	struct edge
	{
		int to, rev, cap, flow;
		edge() { to = rev = cap = flow = 0; }
		edge(int _to, int _rev, int _cap, int _flow)
		{
			to = _to;
			rev = _rev;
			cap = _cap;
			flow = _flow;
		}
	};

	int n;
	vector<edge> G[MAXN];

	void init(int N)
	{
		n = N;
		for(int i = 0; i <= n; i++)
			G[i].clear();
	}

	void add_edge(int u, int v, int C)
	{
		G[u].push_back(edge(v, G[v].size(), C, 0));
		G[v].push_back(edge(u, G[u].size() - 1, 0, 0));
	}

	int dist[MAXN], po[MAXN];

	bool bfs(int s, int t)
	{
		for(int i = 0; i <= n; i++)
			dist[i] = inf, po[i] = 0;

		queue<int> Q;
		dist[s] = 0;
		Q.push(s);

		while(!Q.empty())
		{
			int u = Q.front();
			Q.pop();

			for(auto e: G[u])
				if(e.cap > e.flow && chkmin(dist[e.to], dist[u] + 1))
					Q.push(e.to);
		}

		return dist[t] != inf;
	}

	int dfs(int u, int t, int fl = inf)
	{
		if(u == t) return fl;

		for(; po[u] < SZ(G[u]); )
		{
			int idx = po[u]++, v = G[u][idx].to;
			if(G[u][idx].flow < G[u][idx].cap && dist[v] == dist[u] + 1)
			{
				int f = dfs(v, t, min(fl, G[u][idx].cap - G[u][idx].flow));

				G[u][idx].flow += f;
				G[v][G[u][idx].rev].flow -= f;

				if(f)
					return f;
			}
		}

		return 0;
	}

	int flow(int s, int t)
	{
		int ret = 0, to_add;
		while(bfs(s, t))
		{
			while((to_add = dfs(s, t)))
				ret += to_add;
		}

		return ret;
	}
};

int n;
int a[MAXN], b[MAXN];
vector<int> primes;
vector<int> save[MAXN][2];

void add(int val, int i, int fl)
{
	vector<int> curr;
	for(int i = 2; i * 1ll * i <= val; i++)
		while(val % i == 0)
		{
			curr.push_back(i);
			val /= i;			
		}

	if(val != 1)
		curr.push_back(val);

	sort(ALL(curr));
	curr.erase(unique(ALL(curr)), curr.end());

	save[i][fl] = curr;
	for(auto it: curr)
		primes.push_back(it);
}

void read()
{
	cin >> n;
	primes.clear();
	for(int i = 0; i < n; i++)
		save[i][0].clear(), save[i][1].clear();

	for(int i = 0; i < n; i++)
	{
		cin >> a[i];
		add(a[i], i, 0);
	}

	for(int i = 0; i < n; i++)
	{
		cin >> b[i];
		add(b[i], i, 1);
	}
}

max_flow mf;
int OFFSET;

int get(int u) { return lower_bound(ALL(primes), u) - primes.begin(); }

void solve()
{
	sort(ALL(primes));
	primes.erase(unique(ALL(primes)), primes.end());

	int N = n * n + 10 + SZ(primes);
	OFFSET = n * n;
	mf.init(N);

	int S = N - 1, T = N - 2;
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			if(a[i] > b[j])
			{
				int u = i * n + j;
				mf.add_edge(S, u, 1);
				vector<int> cands = save[i][0];
				for(auto it: cands)
					if(b[j] % it == 0)
						mf.add_edge(u, OFFSET + get(it), 1);
			}
			else if(a[i] < b[j])
			{
				int u = i * n + j;
				mf.add_edge(u, T, 1);
				vector<int> cands = save[i][0];
				for(auto it: cands)
					if(b[j] % it == 0)
						mf.add_edge(OFFSET + get(it), u, 1);
			}

	cout << mf.flow(S, T) << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	int T;
	cin >> T;

	while(T--)
	{
		read();
		solve();
	}

	return 0;
}

