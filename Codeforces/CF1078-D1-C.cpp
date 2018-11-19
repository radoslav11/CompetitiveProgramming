/*
	The main observation is the following:
	A tree has a unique maximum matching if and only if the (max matching * 2) == (number of vertices) or if (max matching) == 0. 
	It can be easily proven by induction or some general observations on the tree structure.
	This means that during the maximum matching building, we don't want to leave a vertex unmatched. We will do dp[vertex][flag having values 0, 1 and 2].
	1) Flag equal to 0 means that the current vertex is unmatched but it has adjacent vertices.
	2) Flag equal to 1 means that the current vertex is already matched.
	3) Flag equal to 2 means that the current vertex is alone (it has no edges adjacent to it).
	The transitions can be easily figured after that. The complexity will be O(N).
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
const int mod = (int)998244353;

template<class T>
T pw(T a, int pw)
{
	T ret(1);
	while(pw)
	{
		if(pw & 1) ret *= a; 
		a *= a;
		pw >>= 1;
	}

	return ret;
}

template<unsigned mod>
class Modint 
{
	private:
		unsigned x;
	
	public:
		Modint() { x = 0; }
		Modint(unsigned _x) { x = _x; }
		operator unsigned() { return x; }
		
		Modint operator==(const Modint& m) const { return x == m.x; }
		Modint operator!=(const Modint& m) const { return x != m.x; }
		
		Modint operator+=(const Modint& m) { x = (x + m.x >= mod ? x + m.x - mod : x + m.x); return *this; }
		Modint operator-=(const Modint& m) { x = (x < m.x ? x - m.x + mod : x - m.x); return *this; }
		Modint operator*=(const Modint& m) { x = 1ULL * x * m.x % mod; return *this; }
	
		Modint operator+=(const int32_t m) { x = (x + (m % mod) >= mod ? x + (m % mod) - mod : x + (m % mod)); return *this; }
		Modint operator-=(const int32_t m) { x = (x < (m % mod) ? x - (m % mod) + mod : x - (m % mod)); return *this; }
		Modint operator*=(const int32_t m) { x = 1ULL * x * (m % mod) % mod; return *this; }

		Modint operator+=(const int64_t m) { x = (x + (m % mod) >= mod ? x + (m % mod) - mod : x + (m % mod)); return *this; }
		Modint operator-=(const int64_t m) { x = (x < (m % mod) ? x - (m % mod) + mod : x - (m % mod)); return *this; }
		Modint operator*=(const int64_t m) { x = 1ULL * x * (m % mod) % mod; return *this; }

		Modint operator+(const Modint& m) const { return Modint(*this) += m; }
		Modint operator-(const Modint& m) const { return Modint(*this) -= m; }
		Modint operator*(const Modint& m) const { return Modint(*this) *= m; }

		Modint operator+(const int32_t m) const { return Modint(*this) += m; }
		Modint operator-(const int32_t m) const { return Modint(*this) -= m; }
		Modint operator*(const int32_t m) const { return Modint(*this) *= m; }

		Modint operator+(const int64_t m) const { return Modint(*this) += m; }
		Modint operator-(const int64_t m) const { return Modint(*this) -= m; }
		Modint operator*(const int64_t m) const { return Modint(*this) *= m; }

		Modint inv() { return pw(Modint(*this), mod - 2); }		
};

int n;
vector<int> adj[MAXN];

void read()
{
	cin >> n;
	for(int i = 0; i + 1 < n; i++)
	{
		int u, v;
		cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
}

Modint<mod> dp[MAXN][3];

void dfs(int u, int pr)
{
	dp[u][0] = 0;
	dp[u][1] = 0;
	dp[u][2] = 1;
	for(int v: adj[u])
		if(v != pr)
		{
			dfs(v, u);
		
			/// No edge
			Modint<mod> nw0 = dp[u][0] * (dp[v][1] + dp[v][2]);
			Modint<mod> nw1 = dp[u][1] * (dp[v][1] + dp[v][2]);
			Modint<mod> nw2 = dp[u][2] * (dp[v][1] + dp[v][2]);

			// With edge 0
			nw0 += (dp[u][0] + dp[u][2]) * dp[v][1];

			// With edge 1
			nw1 += (dp[v][0] + dp[v][2]) * (dp[u][0] + dp[u][2]);
			nw1 += dp[u][1] * dp[v][1];
			
			dp[u][0] = nw0;
			dp[u][1] = nw1;
			dp[u][2] = nw2;
		}
}

void solve()
{
	dfs(1, 1);
	Modint<mod> ans = dp[1][1];
	ans += dp[1][2];
	cout << ans << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}
