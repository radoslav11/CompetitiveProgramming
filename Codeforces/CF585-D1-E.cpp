/*
	We will use the standard reduction gcd trick (I don't know the name). We will replace each a[i] by the product of its prime divisors. For example we will replace 4 and 8 by 2.
	Now the important observation is that the amount of different values will be reduced by a factor ~= 1/2 and also every number will have at most 8 prime divisors. 

	So we can easily count for every number X, the number of subsets with gcd equal to X. Let this number be dp[X]. Then for all X, such that dp[X] != 0, we will do inclusion-exclusion to find
	the amount of numbers Y, such that GCD(X, Y) = 1. This can be done in O(2 ^ (# prime divisors of X)) which may pass the problem.
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
const int mod = (int)1e9 + 7;
const int MAXV = (int)1e7 + 42;

int n;
int a[MAXN];

void read()
{
	cin >> n;
	for(int i = 0; i < n; i++)
		cin >> a[i];
}

int pw2[MAXN], dp[MAXN * 10], lp[MAXN * 10], cnt[MAXN * 10];

void solve()
{
	for(int v = 2; v <= MAXV; v++)
		if(lp[v] == 0) 
			for(int j = v; j <= MAXV; j += v)
				if(lp[j] == 0) lp[j] = v;			
			
	for(int i = 0; i < n; i++)
	{
		int nw = 1;
		while(a[i] != 1)
		{
			int x = lp[a[i]];
			while(a[i] % x == 0)
				a[i] /= x;
			nw *= x;
		}

		a[i] = nw;
	}

	pw2[0] = 1;
	for(int i = 1; i <= n; i++)
		pw2[i] = (pw2[i - 1] * 2ll) % mod;

	for(int i = 0; i < n; i++) cnt[a[i]]++;
	for(int v = 1; v <= MAXV; v++)
		for(int j = 2 * v; j <= MAXV; j += v)
			cnt[v] += cnt[j];

	int answer = 0;
	for(int v = MAXV; v >= 2; v--)
	{
		dp[v] = (pw2[cnt[v]] + mod - 1) % mod;
		for(int k = 2 * v; k <= MAXV; k += v)
			dp[v] = (dp[v] + mod - dp[k]) % mod;
	
		if(dp[v])
		{
			vector<int> divs;
			
			int tmp = v;
			while(tmp != 1)
			{
				divs.pb(lp[tmp]);
				tmp /= lp[tmp];
			}

			int C = 0;
			for(int mask = 0; mask < (1 << SZ(divs)); mask++)
			{
				int value = 1, c = 0;
				for(int i = 0; i < SZ(divs); i++)
					if(mask & (1 << i)) c ^= 1, value *= divs[i];

				if(c) C = (C - cnt[value] + mod) % mod;
				else C = (C + cnt[value]) % mod;
			}

			answer = (answer + C * 1ll * dp[v]) % mod;
		}

	}

	cout << answer << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

