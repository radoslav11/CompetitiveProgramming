/*
   So the tricky part about the problem is to somehow handle the circular shapes of the polygon vertices. Well we can see that the covered part of the circle's circumference
   is (inverse) proportionate to the angle for the corresponding vertex. When we look at the arcs and use the fact that the sum of the angles in a convex polygon with n vertices
   is (n-2)*PI we can see that the circumferences contribute to (n*r*PI - (n-2)*r*PI) = 2*r*PI. Well now we only need to add the lengths of the sides of the actual polygon. 
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
const double PI = acos(-1);

int n;
double r;
vector<pair<double, double> > li;

void read() {
	cin >> n;
	cin >> r;
	for(int i = 0; i < n; i++) {
		double a, b;
		cin >> a >> b;
		li.pb({a, b});
	}
}

inline double sq(double a) { return a * a; }

double dist(pair<double, double> f, pair<double, double> s) {
	return sqrt(sq(f.first - s.first) + sq(f.second - s.second));
}

void solve() {
	double answer = 2 * r * PI;
	pair<double, double> last = li.back();
	for(auto it: li) {
		answer += dist(last, it);
		last = it;
	}

	cout << setprecision(2) << fixed << answer << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

