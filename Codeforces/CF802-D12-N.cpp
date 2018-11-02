/*
	We will solve the problem with mincost flow. We will have a chain with n elements. Then we have edges from i-th to (i+1)-th with capacity eqaul to inf and no cost. 
	We also add edges from source to each vertex with capacity 1 and cost a[i]. We end the graph construction by adding edges from the nodes to the sink with capacity 1 and cost b[i].
	Now we are interested in the minimum cost K-flow. If we run the most standard approach the complexity will be O(K * M * log M) or O(K * M), where M is the number of edges.

	Another possible solution is the following (I'm implementing the previous one). Let's have a function F(k) which gives the answer for a fixed k. We can easily notice that F(k) > F(k + 1). 
	Also F(k + 1) - F(k) <= F(k) - F(k - 1). This means we can apply "Aliens" dp optimization. This way we achieve O(K * N * log ANSWER). 
*/

#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);
const int64_t inf = (int64_t)1e18;

struct edge
{
	int to, rev, flow, cap, cost;
	edge() { to = rev = flow = cap = cost = 0; }
	edge(int _to, int _rev, int _cap, int _flow, int _cost)
	{
		to = _to;
		rev = _rev;
		cap = _cap;
		flow = _flow;
		cost = _cost;
	}
};

int a[MAXN], b[MAXN];
int S = MAXN - 1, T = MAXN - 2, n;
vector<edge> G[MAXN];

void add_edge(int u, int v, int cap, int cost)
{
	G[u].push_back(edge(v, G[v].size(), cap, 0, cost));
	G[v].push_back(edge(u, G[u].size() - 1, 0, 0, -cost));
}

int par[MAXN], par_idx[MAXN];
int64_t dist[MAXN];
bool in_queue[MAXN];

bool spfa()
{
	for(int i = 0; i <= n; i++) dist[i] = inf;
	dist[S] = dist[T] = inf;

	queue<int> Q;
	Q.push(S);
	dist[S] = 0;
	par[S] = -1;
	in_queue[S] = 1;

	while(!Q.empty())
	{
		int u = Q.front();
		Q.pop();
		in_queue[u] = 0;

		for(auto ed: G[u])
		{
			if(ed.flow >= ed.cap)
				continue;

			int v = ed.to;
			if(chkmin(dist[v], dist[u] + ed.cost))
			{
				par[v] = u;
				par_idx[v] = G[v][ed.rev].rev;
				if(!in_queue[v])
				{
					in_queue[v] = 1;
					Q.push(v);
				}
			}
		}
	}

	return dist[T] != inf;
}

int k;

void read()
{
	cin >> n >> k;
	for(int i = 1; i <= n; i++)
		cin >> a[i];
	for(int i = 1; i <= n; i++)
		cin >> b[i];
}

int64_t mincost_flow(int k)
{
	int64_t answer = 0;
	while(k--)
	{
		if(!spfa()) break;
	
		int u = T;
		while(u != S)
		{
			int v = par[u];
			
			answer += G[v][par_idx[u]].cost;
			G[v][par_idx[u]].flow += 1;
			G[u][G[v][par_idx[u]].rev].flow -= 1;
			
			u = v;
		}
	}

	return answer;
}

void solve()
{
	for(int i = 1; i < n; i++) add_edge(i, i + 1, (int)1e9, 0);
	for(int i = 1; i <= n; i++)
	{
		add_edge(S, i, 1, a[i]);
		add_edge(i, T, 1, b[i]);
	}

	cout << mincost_flow(k) << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

