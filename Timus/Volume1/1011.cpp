/*
	We are interested in the smallest solution to the following inequality:
		    
     minimal number of conductors	
	     ceil( (p/100) * x  + eps)      /      x     <=      q/100 

	Well fortunately the numbers have at most 2 digits after the decimal point, so we know that the answer is at most 10000 and we can just check all answers.
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

int64_t get_fract() {
	string s;
	cin >> s;

	int tmp = 0, l = 0;
	vector<int> parts;
	for(char c: s) {
		if(c == '.') {
			parts.pb(tmp);
			l = 0;
			tmp = 0;
		} else {
			tmp = tmp * 10 + c - '0';
			l++;
		}
	}

	parts.pb(tmp);

	if(SZ(parts) == 1) {
		tmp = 0;
		l = 0;
		parts.pb(0);
	}

	while(l < 2) {
		parts[1] *= 10;
		l++;
	}

	int64_t ret = 0;
	for(auto it: parts) {
		ret = ret * 100 + it;
	}

	return ret;
}

int64_t p, q;

void read() {
	p = get_fract();
	q = get_fract();
}

bool check(int64_t x) {
	int64_t conductors = p * x;
	conductors += (10000 - conductors % 10000);
	return conductors < q * x;
}

void solve() {
	int ans = 2;
	while(!check(ans)) {
		ans++;
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

