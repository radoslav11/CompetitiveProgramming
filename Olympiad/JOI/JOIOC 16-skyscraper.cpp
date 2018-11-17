/*
	We will use DP on connected componnents. The main idea is to add the elements one by one beginning from the one with the least value. 
	Also we will add the contribution of the difference between two consecutive values to the sum. The DP will have the following state: 
	dp[i][number of connected componnents (excluding corner componnents)][current sum][flag for left corner][flag for right corner].
	The complexity will be O(N * N * L).
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
const int MAXN = 102;
const int MAXL = 1042;
const int mod = (int)1e9 + 7;

int n, l;
int a[MAXN];

void read()
{
	cin >> n >> l;
	for(int i = 1; i <= n; i++)
		cin >> a[i];
}

int b[MAXN];
int dp[MAXN][MAXN][MAXL][2][2];

void add(int &x, int y)
{
	x += y;
	if(x >= mod) 
		x -= mod;
}

int mult(int x, int y) { return x * 1ll * y % mod; }

int rec(int i, int cnt, int S, int L, int R)
{
	S += L * b[i];
	S += R * b[i];
	S += cnt * 2 * b[i];

	if(S > l || cnt < 0) 
		return 0;

	if(i == n - 1) /// Last position
		return cnt == 0;

	int &memo = dp[i][cnt][S][L][R];
	if(memo != -1) 
		return memo;

	memo = 0;

	add(memo, mult(cnt * (cnt - 1), rec(i + 1, cnt - 1, S, L, R)));  /// merge two comps 
	
	add(memo, mult(cnt, rec(i + 1, cnt - 1, S, 1, R)));  /// add to L corner 
	add(memo, mult(cnt, rec(i + 1, cnt - 1, S, L, 1)));  /// add to R corner 
	add(memo, rec(i + 1, cnt, S, 1, R));  /// new comp to L corner
	add(memo, rec(i + 1, cnt, S, L, 1));  /// new comp to R corner
	
	add(memo, rec(i + 1, cnt + 1, S, L, R));  /// new comp
	add(memo, mult(2 * cnt, rec(i + 1, cnt, S, L, R)));  /// extend existing comp

	return memo;
}

void solve()
{
	memset(dp, -1, sizeof(dp));
	sort(a + 1, a + n + 1);
	for(int i = 0; i < n; i++)
		b[i] = i ? (a[i + 1] - a[i]) : 0;

	cout << rec(0, 0, 0, 0, 0) << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

