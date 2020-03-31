/*
	We will just use tree dp. Let's root the tree from node 1. Now the state dp[u][k] will represent the maximum number of apples that can be preserverd
	if we keep exactly k branches and we only consider the subtree of node u. To calculate this, we can just add the children one by one when computing dp[u][:].

	The complexity will actually be O(n * q), even though it looks like O(n * q * q). The trick is quite common so you should be able to easily find a blog 
	with the proof of the time complexity on Codeforces.
*/

#include <bits/stdc++.h>
#define endl '\n'

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define pb push_back

using namespace std;
template<class T, class T1> int chkmin(T &x, const T1 &y) { return x > y ? x = y, 1 : 0; }
template<class T, class T1> int chkmax(T &x, const T1 &y) { return x < y ? x = y, 1 : 0; }
const int MAXN = (1 << 10);

int n, q;
vector<pair<int, int > > adj[MAXN];

void read() {
	cin >> n >> q;
	for(int i = 0; i < n - 1; i++) {
		int u, v, w;
		cin >> u >> v >> w;
		adj[u].pb({v, w});
		adj[v].pb({u, w});
	}
}

int dp[MAXN][MAXN];
int tr_sz[MAXN];

void dfs(int u, int pr) {
	tr_sz[u] = 1;
	
	dp[u][0] = 0;
	for(int i = 1; i <= q; i++) {
		dp[u][i] = -(int)1e9;
	}

	for(auto e: adj[u]) {
		if(e.first == pr) continue;
		
		dfs(e.first, u);		

		for(int c = min(q, tr_sz[u]); c >= 0; c--) {
			for(int sz = min(q, tr_sz[e.first]); sz >= 0; sz--) {
				if(c + sz + 1 <= q) {
					chkmax(dp[u][c + sz + 1], dp[u][c] + dp[e.first][sz] + e.second);
				}
			}
		}
		
		tr_sz[u] += tr_sz[e.first];
	}
}

void solve() {
	dfs(1, 1);
	cout << dp[1][q] << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

