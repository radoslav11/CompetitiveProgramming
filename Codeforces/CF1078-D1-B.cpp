/*
	We notice that the only way to reveal some weights is if all their weights are the same. Let's have dp[total weight of current subset][number of elements in the subset]. 
	Computing the DP can be done in O(N * N * sum) = O(N * N * N * MAXW) which is fast enough. Now we fix the value of all element in the set we will reveal and the size of the subset.
	If Comb(cnt[value], sz) is equal to dp[sz * value][sz] then all subsets with weight sz * value and number of elements equal to sz are of form (value, value, ... , value). 
	Then the answer is at least sz.

	We shouldn't forget the case when there are only two different weights in the array (for example 2 2 2 9 9 9). Then if we reveal the (2, 2, 2) we will also reveal (9, 9, 9).
	If one forgot this case, he would have gotten WA4 on pretests.
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
const int MAXN = (1 << 7);
const int mod = (int)1e9 + 9;

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

Modint<mod> fact[MAXN], ifact[MAXN];

void precompute()
{
	fact[0] = 1;
	for(int i = 1; i < MAXN; i++) fact[i] = fact[i - 1] * i;
	ifact[MAXN - 1] = fact[MAXN - 1].inv();
	for(int i = MAXN - 2; i >= 0; i--) ifact[i] = ifact[i + 1] * (i + 1);
}

Modint<mod> C(int n, int k) 
{  
	if(n < k || n < 0 || k < 0) return Modint<mod>(0);
	return fact[n] * ifact[n - k] * ifact[k];
} 


int n;
int a[MAXN];

void read()
{
	cin >> n;
	for(int i = 1; i <= n; i++)
		cin >> a[i];
}

int cnt[MAXN];
Modint<mod> dp[MAXN * MAXN][MAXN];

void solve()
{
	int answer = 1;

	dp[0][0] = 1;
	for(int i = 1; i <= n; i++)
		for(int v = n * 100; v >= 0; v--)
			for(int c = n; c >= 0; c--)
				if(v >= a[i] && c >= 1)
					dp[v][c] = dp[v][c] + dp[v - a[i]][c - 1];

	int types = 0;
	for(int i = 1; i <= n; i++)
	{
		cnt[a[i]]++;
		if(cnt[a[i]] == 1)
			types++;
	}

	for(int i = 1; i <= 100; i++)
		for(int c = 2; c <= cnt[i]; c++)
			if(dp[c * i][c] == C(cnt[i], c))
			{
				chkmax(answer, c);
				if(types == 2 && c == cnt[i])
					chkmax(answer, n);
			}

	cout << answer << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	precompute();

	read();
	solve();
	return 0;
}
