/*
		The main observation in this problem is that when we have A different cards already present in the previous packs, we can assume that these cards are pairwise distinct. 
		Same goes for the other C - A cards, not present in the previous packs. This lets us create dp with state (A = number of present cards).
	
		We will have dp[A] = 1 + SUM(prob(Z) * dp[A + Z]) for all valid Z. Z is the number of new cards we will get with 1 pack. As there might be a transition from dp[A] to dp[A], we will
		move the terms with dp[A] to the left part. Then we will divide the right sum by the coefficient before dp[A].

		With some precomputes the complexity will be O(T * N * C).
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
const int MAXN = 100;

int c, n;

void read()
{
	cin >> c >> n;
}

bool used[MAXN];
double dp[MAXN];

long double prec[MAXN][MAXN];

void precompute()
{
	prec[0][0] = 1;
	for(int i = 1; i < MAXN; i++)
	{
		prec[i][0] = 1;
		for(int j = 1; j <= i; j++)
			prec[i][j] = prec[i - 1][j] + prec[i - 1][j - 1];
	}
}

double rec(int A)
{
	if(A == c)
		return 0;

	if(used[A])
		return dp[A];

	used[A] = 1;
	dp[A] = 1;

	double coef = 1;
	for(int curr_new = 0; curr_new <= n && curr_new + A <= c; curr_new++)
	{
		int prv = n - curr_new;
		if(prv <= A)
		{
			double prob = (prec[A][prv] * prec[c - A][curr_new]) / prec[c][n];
			if(curr_new == 0) coef -= prob;
			else dp[A] += prob * rec(A + curr_new); 
		}
	}

	dp[A] /= coef;
	return dp[A];
}

void solve()
{
	memset(used, 0, sizeof(used));
	cout << setprecision(6) << fixed << rec(0) << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	int T;
	cin >> T;

	precompute();

	for(int test = 1; test <= T; test++)
	{
		read();

		cout << "Case #" << test << ": ";
		solve();
	}

	return 0;
}

