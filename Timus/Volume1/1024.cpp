/*
	Let's build the directed graph with n vertices and edges of the form (i -> P(i)). Clearly, as we have a permutation, this graph will be a union of simple cycles.  
	The main observation is that when we apply P(P(n)), we basically cyclicly shift all vertices in every cycle. Well then the answer will be just the LCM of the sizes of
	all simple cycles. The complexity will be O(n).
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
const int MAXN = (1 << 20);

int n;
int p[MAXN];

void read() {
	cin >> n;
	for(int i = 1; i <= n; i++) {
		cin >> p[i];
	}
}

bool used[MAXN];

int gcd(int x, int y) {
	if(x > y) swap(x, y);

	int tmp = 0;
	while(x) {
		tmp = x;
		x = y % x;
		y = tmp;
	}

	return y;
}

void solve() {
	int ans = 0;
	for(int i = 1; i <= n; i++) {
		if(!used[i]) {
			int x = i, l = 0;
			while(!used[x]) {
				l++;
				used[x] = true;
				x = p[x];
			}

			if(ans == 0) ans = l;
			else ans = ans * 1ll * l / gcd(ans, l);
		}
	}

	cout << ans << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

