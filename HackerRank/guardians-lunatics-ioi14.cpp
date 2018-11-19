/*
	We have a very obvious O(L * L * G) dp with state dp[prefix][number of guards] and O(L) transitions for every state. The cost(l, r) function is len(l, r) * sum(l, r). We can easily calculate it
	in constant time with prefix sums. 

	Let's look at cost(l, r) and cost(l, r + 1). 

	cost(l, r) = (r - l + 1) * (psum[r] - psum[l - 1])
	cost(l, r) = (r - l + 2) * (psum[r + 1] - psum[l - 1])

	As all C[i] >= 1 we know that psum[r] < psum[r + 1]. Then it's quite obvious that we have cost(l, r) < cost(l, r + 1) for all pairs (l, r). Similarly we prove that cost(l + 1, r) < cost(l, r).
	Let the optimal transition for dp[i][k] be opt[i][k]. Well the above conditions are enough to have opt[i][k] <= opt[i + 1][k]. Then we will simply apply divide and conquer optimization.

	The complexity will be O(L * G * log L).

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
const int MAXL = 8042;
const int MAXG = 842;
const int64_t inf = (int64_t)4e18;

int L, G;
int64_t psum[MAXL];

int64_t cost(int l, int r) { return (r - l + 1) * 1ll * (psum[r] - psum[l - 1]); }

void read()
{
	cin >> L >> G;
	for(int i = 1; i <= L; i++)
	{
		int x;
		cin >> x;
		psum[i] = psum[i - 1] + x;
	}
}

int64_t dp[MAXL][MAXG];

void rec(int l, int r, int opt_l, int opt_r, int k)
{
	if(r < l) return;

	int mid = (l + r) >> 1, opt = mid - 1;
	
	dp[mid][k] = inf;
	for(int i = min(mid, opt_r); i >= opt_l; i--)
		if(chkmin(dp[mid][k], dp[i][k - 1] + cost(i + 1, mid)))	
			opt = i;

	rec(l, mid - 1, opt_l, opt, k);
	rec(mid + 1, r, opt, opt_r, k);
}

void solve()
{
	for(int i = 1; i <= L; i++) dp[i][0] = inf;

	for(int k = 1; k <= G; k++)
	{
		dp[0][k] = inf;
		rec(1, L, 0, L, k);
	}

	cout << dp[L][G] << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

