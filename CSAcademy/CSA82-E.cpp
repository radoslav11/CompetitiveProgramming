/*
	Let us fix the K vertices which will have fountains. Then the main observation is that we will use only edges in the MST to further connect the other N - K vertices. 
	This lets us build the MST and then do DP on that tree. The state will be DP[vertex][current K][flag depending if the vertex has water to it]. The complexity will be O(N^2).
	
	We can further improve the solution by applying aliens trick. This way the state will be only DP[vertex][flag if that vertex has water to it]. 
	The complexity will be O(N log Answer) after building the MST.
*/

#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 11);

struct dsu
{
	int sz;
	vector<int> par, psz;

	void init(int n)
	{
		sz = n;
		par.assign(sz + 1, 0);
		psz.assign(sz + 1, 0);
		for(int i = 0; i <= sz; i++)
			par[i] = i, psz[i] = 1;
	}

	int root(int u) { return par[u] = ((u == par[u]) ? u : root(par[u])); }
	
	bool connected(int x, int y) { return root(x) == root(y); }

	void unite(int x, int y)
	{
		x = root(x), y = root(y);
		if(x == y) return;
		if(psz[x] > psz[y]) swap(x, y);
		par[x] = y, psz[y] += psz[x]; 
	}
};

int n, k;
int F[MAXN], C[MAXN][MAXN];
vector<int> adj[MAXN];
vector<pair<int, int> > ed;

void read()
{
	cin >> n >> k;
	ed.clear();
	for(int i = 0; i < n; i++) adj[i].clear();
	for(int i = 0; i < n; i++) cin >> F[i];
	for(int i = 0; i < n; i++) 
		for(int j = 0; j < n; j++)
		{
			cin >> C[i][j];
			if(i < j)
				ed.push_back({i, j});
		}
}

dsu d;
bool cmp(pair<int, int> a, pair<int, int> b) { return C[a.first][a.second] < C[b.first][b.second]; }

pair<double, int> add(pair<double, int> A, pair<double, int> B) { return {A.first + B.first, A.second + B.second}; }

double Cost;
pair<double, int> dp[MAXN][2];

void dfs(int u, int pr)
{
	dp[u][0] = {0, 0};
	dp[u][1] = {Cost + F[u], 1};
	for(int v: adj[u])
		if(v != pr)
		{
			dfs(v, u);
			dp[u][1] = min(add(dp[u][1], add(make_pair(C[u][v], 0), dp[v][0])), min(add(dp[u][0], add(dp[v][1], make_pair(C[u][v], 0))), add(dp[u][1], dp[v][1])));
			dp[u][0] = min(add(dp[u][0], add(dp[v][0], make_pair(C[u][v], 0))), add(dp[u][0], dp[v][1]));
		}
}

pair<double, int> eval(double cost)
{
	Cost = cost;
	dfs(0, 0);
	return dp[0][1];
}

void solve()
{
	sort(ed.begin(), ed.end(), cmp);

	d.init(n);
	for(auto it: ed)
		if(!d.connected(it.first, it.second))
		{
			d.unite(it.first, it.second);
			adj[it.first].push_back(it.second);
			adj[it.second].push_back(it.first);
		}

	double low = 0, high = 1e9, mid, ret; 
	for(int ops = 0; ops < 100; ops++)
	{
		mid = (low + high) / 2.0;
		if(eval(mid).second <= k)
			ret = mid, high = mid;
		else
			low = mid;
	}

	cout << (int64_t)(eval(ret).first - k * ret + 0.5) << endl;
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

