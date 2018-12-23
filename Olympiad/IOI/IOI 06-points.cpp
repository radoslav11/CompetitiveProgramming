/*
	We will solve the problem recursively by considering triangles with vertices of two colours. Initially we will have two triangles. The first on is with points (0, 0), (0, s) and (s, 0). 
	The second one is with vertices (s, s), (0, s) and (s, 0). Now for each of these triangles we find the points inside of it. Now we recursively will split these triangles.

	The main observation is that at each moment the triangles will have vertices of both colours.

	We have several cases:

	1) There are no points inside of a triangle. We stop this part of the recursion.
	2) All points inside of a triangle have the same colour. Then we will simply connect all to a vertex of the triangle.
	3) As the vertices of the current triangle are coloured in two colours we will pick a new point inside of it, such that its colour is the same as the colour of the vertices that appears once. 
	   After that we will make the connection between these two points and split the current triangle into 3 new triangles.

	If we always pick a random point inside of the colour we want, the complexity will be O((r + b) * log), because it's similar to quick sort.
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

random_device rd;
mt19937 mt(rd());

struct PT
{
	int x, y;
	PT() { x = y = 0; }
	PT(int _x, int _y) { x = _x; y = _y; }
	PT operator-(const PT &other) { return PT(x - other.x, y - other.y); }
	PT operator+(const PT &other) { return PT(x + other.x, y + other.y); }
	void operator+=(const PT &other) { x += other.x, y += other.y; }
	void operator-=(const PT &other) { x -= other.x, y -= other.y; }
	int64_t operator%(const PT &other) { return x * 1ll * other.y - y * 1ll * other.x; } 
	bool operator==(const PT &other) { return x == other.x && y == other.y; } 
};

struct Point
{
	PT p;
	int idx, col;

	Point() { p = PT(); idx = col = 0; }
	Point(int x, int y, int i, int c)
	{
		p = PT(x, y);
		idx = i;
		col = c;
	}
};

int n, m, s;
vector<Point> a;

int sign(int64_t x) { return x > 0 ? 1 : (x < 0 ? -1 : 0); }

bool is_inside(Point x, Point a, Point b, Point c)
{
	int o1 = sign((a.p - x.p) % (b.p - a.p));
	int o2 = sign((b.p - x.p) % (c.p - b.p));
	int o3 = sign((c.p - x.p) % (a.p - c.p));	
	return (o1 == o2 && o2 == o3);
}

vector<pair<pair<int, int>, char> > answer;

void add_edge(Point a, Point b)
{
	char col = (a.col == 1) ? 'r' : 'g';
	answer.pb({{a.idx, b.idx}, col});
}

void read()
{
	cin >> n;
	for(int i = 1; i <= n; i++)
	{
		int x, y;
		cin >> x >> y;
		
		a.pb(Point(x, y, i, 0));
		chkmax(s, x);
		chkmax(s, y);
	}

	cin >> m;
	for(int i = 1; i <= m; i++)
	{
		int x, y;
		cin >> x >> y;
		
		a.pb(Point(x, y, i, 1));
		chkmax(s, x);
		chkmax(s, y);
	}
}

void rec(Point a, Point b, Point c, vector<Point> green, vector<Point> red)
{
	if(green.empty())
	{
		if(a.col == 0) swap(a, b);
		if(a.col == 0) swap(a, c);
		for(auto it: red) add_edge(a, it);
		return;
	}
	
	if(red.empty())
	{
		if(a.col == 1) swap(a, b);
		if(a.col == 1) swap(a, c);
		for(auto it: green) add_edge(a, it);
		return;
	}

	int cnt1 = a.col + b.col + c.col, cnt0 = 3 - cnt1;
	if(cnt0 < cnt1)
	{
		Point Mid = green[mt() % SZ(green)];
		
		if(Mid.col == a.col) add_edge(Mid, a);
		if(Mid.col == b.col) add_edge(Mid, b);
		if(Mid.col == c.col) add_edge(Mid, c);

		vector<Point> new_green[3], new_red[3];
		
		for(auto it: green) if(is_inside(it, a, b, Mid)) new_green[0].pb(it); 
		for(auto it: green) if(is_inside(it, b, c, Mid)) new_green[1].pb(it); 
		for(auto it: green) if(is_inside(it, c, a, Mid)) new_green[2].pb(it); 
		
		for(auto it: red) if(is_inside(it, a, b, Mid)) new_red[0].pb(it); 
		for(auto it: red) if(is_inside(it, b, c, Mid)) new_red[1].pb(it); 
		for(auto it: red) if(is_inside(it, c, a, Mid)) new_red[2].pb(it); 
	
		rec(a, b, Mid, new_green[0], new_red[0]);
		rec(b, c, Mid, new_green[1], new_red[1]);
		rec(c, a, Mid, new_green[2], new_red[2]);
	}
	else
	{
		Point Mid = red[mt() % SZ(red)];
		
		if(Mid.col == a.col) add_edge(Mid, a);
		if(Mid.col == b.col) add_edge(Mid, b);
		if(Mid.col == c.col) add_edge(Mid, c);

		vector<Point> new_green[3], new_red[3];
		
		for(auto it: green) if(is_inside(it, a, b, Mid)) new_green[0].pb(it); 
		for(auto it: green) if(is_inside(it, b, c, Mid)) new_green[1].pb(it); 
		for(auto it: green) if(is_inside(it, c, a, Mid)) new_green[2].pb(it); 
		
		for(auto it: red) if(is_inside(it, a, b, Mid)) new_red[0].pb(it); 
		for(auto it: red) if(is_inside(it, b, c, Mid)) new_red[1].pb(it); 
		for(auto it: red) if(is_inside(it, c, a, Mid)) new_red[2].pb(it); 
	
		rec(a, b, Mid, new_green[0], new_red[0]);
		rec(b, c, Mid, new_green[1], new_red[1]);
		rec(c, a, Mid, new_green[2], new_red[2]);
	}
}

void solve()
{
	Point DL, DR, UL, UR;
	for(auto it: a)
	{
		if(it.p == PT(0, 0)) DL = it;
		if(it.p == PT(s, 0)) DR = it;
		if(it.p == PT(0, s)) UL = it;
		if(it.p == PT(s, s)) UR = it;
	}

	add_edge(DL, DR);
	add_edge(UL, UR);

	vector<Point> li1[2];
	vector<Point> li2[2];

	for(auto it: a)
	{
		if(is_inside(it, DL, DR, UL)) li1[it.col].pb(it);
		if(is_inside(it, UL, UR, DR)) li2[it.col].pb(it);
	}

	rec(DL, DR, UL, li1[0], li1[1]);
	rec(UL, UR, DR, li2[0], li2[1]);

	for(auto it: answer) cout << it.first.first << " " << it.first.second << " " << it.second << endl; 
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

