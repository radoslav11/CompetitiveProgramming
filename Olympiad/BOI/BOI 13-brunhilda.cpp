/*
	There is a simple solution with upper bound on the time equal to O(MAX * log(log(MAX)) * log(MAX)) with DP and maintaining a heap. Probably the actual complexity is smaller.
	Unfortunately this was too slow, but we can notice that there is monotonicity in the answers and so if at each step we remove the largest number of people we will have an optimal strategy.
	This also means we can remove the heap from the dp and just find the largest transition.
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
const int B = (int)2e7 + 42;
const int inf = B + 42;
 
int n, q;
int dp[B + 42];
int a[MAXN], trans[B + 42];
 
void read()
{
	cin >> n >> q;
	for(int i = 0; i < n; i++)
		cin >> a[i];
}
 
void solve()
{
	for(int i = 0; i < a[n - 1]; i++) dp[i] = 1;
	for(int i = a[n - 1]; i <= B; i++) dp[i] = inf;
 
	for(int i = 0; i < n; i++)
		for(int j = a[i] - 1; j <= B; j += a[i])
			trans[j] = a[i] - 1;
 
	for(int i = B; i >= 0; i--)
		chkmax(trans[i], trans[i + 1] - 1);
 
	for(int d = a[n - 1]; d <= B; d++)
		chkmin(dp[d], 1 + dp[d - trans[d]]);
 
	while(q--)
	{
		int x;
		cin >> x;
		if(dp[x] == inf) cout << "oo" << endl;
		else cout << dp[x] << endl;
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
