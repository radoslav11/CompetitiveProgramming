/*
	https://codeforces.com/blog/entry/51010
*/

#include <bits/stdc++.h>
#include "park.h"
//#include "Lgrader.cpp"

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define pb push_back

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1400);

int n, state[MAXN];
static int Place[MAXN];
vector<int> adj[MAXN];

int query(int from, int to)
{
	if(from > to) swap(from, to);
	return Ask(from, to, Place);
}

void add_edge(int u, int v)
{
	adj[u].pb(v);
	adj[v].pb(u);

	if(u < v) Answer(u, v);
	else Answer(v, u);
}

int d[MAXN], rem[MAXN];
vector<int> bfs_order;

void gen_bfs_order(int src)
{
	bfs_order.clear();
	for(int i = 0; i < n; i++) d[i] = -1;

	queue<int> q;
	d[src] = 0;
	q.push(src);

	while(!q.empty())
	{
		int u = q.front();
		bfs_order.pb(u);
		q.pop();

		for(int v: adj[u])
			if(!rem[v] && d[v] == -1)
			{
				d[v] = d[u] + 1;
				q.push(v);
			}
	}
}

bool add_one_direct(int up, int u)
{
	gen_bfs_order(up);
	for(int i = 0; i < n; i++) Place[i] = 0;
	for(int v: bfs_order) Place[v] = 1;
	Place[u] = 1;

	if(!query(up, u)) return 0;

	int low = 0, high = SZ(bfs_order) - 1, ret, mid;
	while(low <= high)
	{
		mid = (low + high) >> 1;
		for(int i = 0; i < n; i++) Place[i] = 0;
		for(int i = 0; i <= mid; i++) Place[bfs_order[i]] = 1;
		Place[u] = 1;

		if(query(up, u))
			ret = mid, high = mid - 1;
		else 
			low = mid + 1;
	}

	add_edge(u, bfs_order[ret]);
	rem[bfs_order[ret]] = 1;
	return 1;
}

void dfs_clear(int u)
{
	rem[u] = 1;
	for(int v: adj[u])
		if(!rem[v])
			dfs_clear(v);
}

bool add_direct(int u)
{
	bool ret = 0;
	for(int i = 0; i < n; i++) rem[i] = 0;

	for(int up = 0; up < n; up++)
		while(!rem[up] && state[up] == 1)
		{
			if(add_one_direct(up, u)) ret = 1;
			else dfs_clear(up);			
		}

	return ret;
}

int get_new(int u)
{
	int low = 0, high = n - 1, mid, ret;
	while(low <= high)
	{
		mid = (low + high) >> 1;
		
		for(int i = 0; i < n; i++) Place[i] = (state[i] == 1);
		for(int i = 0; i <= mid; i++) if(state[i] != 2) Place[i] = 1;
		Place[u] = 1; 

		if(query(0, u))
			ret = mid, high = mid - 1;
		else
			low = mid + 1;
	}

	return ret;
}

void fix(int u)
{
	state[u] = 2;

	while(!add_direct(u))
	{
		int nw = get_new(u);
		fix(nw);
	}

	state[u] = 1;	
}

void Detect(int T, int N) 
{
	T = 0;
	n = N;

	for(int i = 0; i < n; i++) state[i] = 0;

	state[0] = 1;
	for(int i = 1; i < n; i++)
		if(!state[i])
			fix(i);
}

