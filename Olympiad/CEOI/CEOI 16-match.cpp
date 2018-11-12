/*
   We will solve the problem recursively. It's intuitive that we want to match the bracket at position 0 to the rightmost possible position. Lets have T = S[p + 1; N - 1].
   If both S and T have at least one possible solving pattern then S[1; p - 1] also has a solving pattern and we can match 0 with p. So we will find the largest such p with S[p] == S[0].
   Then we will solve the problem recursively on S[1, p - 1] and S[p + 1, N - 1]. If we can find p fast, the complexity of the recursion is O(N).
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
const int MAXN = (int)1e5 + 42;
const int SIGMA = 26;
 
string s;
 
void read()
{
	cin >> s;
}
 
char answer[MAXN];
int last[MAXN][SIGMA];
 
void rec(int l, int r)
{
	if(r < l)
		return;
 
	int mid = last[r][s[l] - 'a'];
 
	answer[l] = '(';
	answer[mid] = ')';
 
	rec(l + 1, mid - 1);
	rec(mid + 1, r);
}
 
void solve()
{
	stack<int> st;
	for(int i = 0; i < SZ(s); i++)
		if(!st.empty() && s[st.top()] == s[i]) st.pop();
		else st.push(i);
 
	if(!st.empty())
	{
		cout << -1 << endl;
		return;
	}
 
	memset(last, -1, sizeof(last));
 
	for(int i = 0; i < SZ(s); i++)
	{
		last[i][s[i] - 'a'] = i;
		if(i >= 1)
		{
			int pos = last[i - 1][s[i] - 'a'];
			for(int c = 0; c < SIGMA; c++)
				if(pos > 0 && c != (s[i] - 'a'))
					last[i][c] = last[pos - 1][c];
		}
	}
 
	rec(0, SZ(s) - 1);
 
	for(int i = 0; i < SZ(s); i++)
		cout << answer[i];
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
