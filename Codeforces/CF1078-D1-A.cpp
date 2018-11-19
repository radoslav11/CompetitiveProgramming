/*
	The only observation we need is that from A and B the optimal path might go through points on the line with X and Y being A.x, A.y, B.x and B.y. So we find these 4 candidates and try all pairs.
*/

#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define y1 weqqwewqeqw 
#define double long double

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);

int64_t a, b, c, x1, y1, x2, y2;

void read()
{
	cin >> a >> b >> c >> x1 >> y1 >> x2 >> y2;
}

pair<double, double> withX(double x)
{
	double y = (-c - a * x) / (double)b;
	return {x, y};
}

pair<double, double> withY(double y)
{
	double x = (-c - b * y) / (double)a;
	return {x, y};
}

double sq(double x) { return x * x; }

void solve()
{
	double answer = abs(x1 - x2) + abs(y1 - y2);

	vector<pair<double, double> > cands;
	if(b != 0) cands.push_back(withX(x1));
	if(b != 0) cands.push_back(withX(x2));
	if(a != 0) cands.push_back(withY(y2));
	if(a != 0) cands.push_back(withY(y1));

	for(auto c1: cands)
		for(auto c2: cands)
		{
			double Curr = 0;
			Curr += abs(x1 - c1.first) + abs(y1 - c1.second);
			Curr += abs(x2 - c2.first) + abs(y2 - c2.second);
			Curr += sqrt(sq(c1.first - c2.first) + sq(c2.second - c1.second));
			chkmin(answer, Curr);
		}

	cout << setprecision(9) << fixed << answer << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}
