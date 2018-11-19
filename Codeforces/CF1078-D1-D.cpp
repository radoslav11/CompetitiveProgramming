/*
	We will use binary lifting. In to[x][i] we will store the interval that position x can cover after 2^i seconds. On each layer we will store a segment tree for maximum and minumum over to[][i].
	Then for each position we can find when it will cover everything in O(log^3(n)) with binary search or O(log^2(n)) by walking on the binary lifting. The building of the segment trees and initial
	computation of to[x][i] can again be done in O(n * log^2(n)) leading to O(n * log^2(n)) time solution. 
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
const int MAXN = (int)3e5 + 42;
const int inf = (int)1e9;

pair<int, int> to[MAXN][20];

struct segment_tree
{
	int mx[4 * MAXN], mn[4 * MAXN];

	void init(int l, int r, int idx, int Layer)
	{
		if(l == r)
		{
			mn[idx] = to[l][Layer].first;
			mx[idx] = to[l][Layer].second;
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1, Layer);
		init(mid + 1, r, 2 * idx + 2, Layer);

		mx[idx] = max(mx[2 * idx + 1], mx[2 * idx + 2]);
		mn[idx] = min(mn[2 * idx + 1], mn[2 * idx + 2]);
	}

	int query_max(int ql, int qr, int l, int r, int idx)
	{
		if(ql > r || qr < l)
			return -inf;

		if(ql <= l && r <= qr)
			return mx[idx];

		int mid = (l + r) >> 1;
		return max(query_max(ql, qr, l, mid, 2 * idx + 1), query_max(ql, qr, mid + 1, r, 2 * idx + 2));
	}

	int query_min(int ql, int qr, int l, int r, int idx)
	{
		if(ql > r || qr < l)
			return inf;

		if(ql <= l && r <= qr)
			return mn[idx];

		int mid = (l + r) >> 1;
		return min(query_min(ql, qr, l, mid, 2 * idx + 1), query_min(ql, qr, mid + 1, r, 2 * idx + 2));
	}

} t[20];

int n;
int N;
int a[MAXN];

int read_int();

void read()
{
	n = read_int();
	for(int i = 0; i < n; i++)
	{
		a[i] = read_int();
		a[i + n] = a[i];
		a[i + n + n] = a[i];
	}
}

pair<int, int> get(int Layer, int L, int R)
{
	if(R - L + 1 >= n) return {0, n - 1};
	if(to[L][Layer].second - to[L][Layer].first + 1 >= n) return {0, n - 1};
	if(to[R][Layer].second - to[R][Layer].first + 1 >= n) return {0, n - 1};

	int nL = t[Layer].query_min(L, R, 0, N - 1, 0);
	if(R - nL + 1 >= n) return {0, n - 1};

	int nR = t[Layer].query_max(L, R, 0, N - 1, 0);
	if(nR - nL + 1 >= n) return {0, n - 1};

	return {nL, nR};
}

int solve(int p)
{
	int answer = 0;

	int L = p, R = p;
	for(int i = 19; i >= 0; i--)
	{
		pair<int, int> nw = get(i, L, R);
		if(nw.second - nw.first + 1 < n)
		{
			answer += (1 << i);
			L = nw.first;
			R = nw.second;
		}
	}

	return answer + 1;
}

void solve()
{
	if(n == 1)
	{
		cout << 0 << endl;
		return;
	}

	N = 3 * n;
	for(int i = 0; i < N; i++)
		to[i][0] = {max(0, i - a[i]), min(N - 1, i + a[i])};
	t[0].init(0, N - 1, 0, 0);
	
	for(int i = 1; i < 20; i++)
	{
		for(int x = 0; x < N; x++)
			to[x][i] = get(i - 1, to[x][i - 1].first, to[x][i - 1].second);
		t[i].init(0, N - 1, 0, i);
	}

	for(int i = 0; i < n; i++)
		cout << solve(i + n) << " ";
	cout << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

const int maxl = 100000;
char buff[maxl];
int ret_int, pos_buff = 0;

void next_char() { if(++pos_buff == maxl) fread(buff, 1, maxl, stdin), pos_buff = 0; }

int read_int()
{
	ret_int = 0;
	for(; buff[pos_buff] < '0' || buff[pos_buff] > '9'; next_char());
	for(; buff[pos_buff] >= '0' && buff[pos_buff] <= '9'; next_char())
		ret_int = ret_int * 10 + buff[pos_buff] - '0';
	return ret_int;
}
