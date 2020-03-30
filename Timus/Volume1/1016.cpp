/*
   In this problem we just encode all possible states as vertices of a graph and then run Dijkstra/SPFA. The states can be represented:
   1) A permutation / orientation of the costs (i.e. for the given sides). 
   2) The current position.

   The complexity will be O(number_of_states * log) if we use dijkstra and something like O(number_of_states) if we use SPFA.

Note: In this question we don't really have any problems with the memory but sometimes we need to encode a larger permutation. 
Then the trick I used with enumerating the permutations with the number of permutations that are lexicographically less than them is quite useful.
Additionally sometimes this trick is used for combinations/variations (but then we need to use an additional DP recurrence to find this count).
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
const int COUNT_STATES = 64 * 720;

pair<int, int> get_cell() {
	string s;
	cin >> s;
	return {s[0] - 'a', s[1] - '1'};
}

int cost[6], fact[7] = {1, 1, 2, 6, 24, 120, 720};

// counts number of permutations less than a given one.
int enumerate_perm(const vector<int> &p) {
	int ret = 0, j = 5;
	for(int i: p) {
		ret += i * fact[j];
		j--;
	} 

	return ret;
}

struct state {
	// cell
	int i, j;

	// near, far, top, right, bottom and left
	vector<int> perm;

	state() { i = j = 0; for(int i = 0; i < 6; i++) perm.pb(i); }

	int encode() {
		int h = enumerate_perm(perm);
		h *= 8;
		h += i;
		h *= 8;
		h += j;
		return h;
	}

	bool operator!=(const state &other) {
		return i != other.i || j != other.j || perm != other.perm;
	}
};

pair<int, int> sta, fin;

int dist[COUNT_STATES];
state par[COUNT_STATES];
bool in_queue[COUNT_STATES];

void read() {
	sta = get_cell();
	fin = get_cell();

	for(int i = 0; i < 6; i++) {
		cin >> cost[i];
	}
}

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};
pair<int, int> trans[4] = {{3, 5}, {5, 3}, {1, 0}, {0, 1}};

void solve() {
	for(int i = 0; i < COUNT_STATES; i++) {
		dist[i] = (int)1e9;
	}

	state src;
	src.i = sta.first;
	src.j = sta.second
	;

	queue<state> Q;
	Q.push(src);
	dist[src.encode()] = cost[4];

	state best_end;
	int answer = (int)1e9;

	while(!Q.empty()) {
		state u = Q.front();
		Q.pop();

		int hu = u.encode();
		in_queue[hu] = false;
	

		/*
		cout << u.i << " " << u.j << "    perm=[";
		for(auto i: u.perm) cout << i << ", ";
		cout << "]    ---->     " << dist[hu] << endl;
		*/

		if(u.i == fin.first && u.j == fin.second && chkmin(answer, dist[hu])) {
			best_end = u;	
		}

		for(int dir = 0; dir < 4; dir++) {
			int nx = u.i + dx[dir];
			int ny = u.j + dy[dir];

			if(0 <= nx && nx < 8 && 0 <= ny && ny < 8) {
				state v = u;

				int top_to = trans[dir].first, bot_to = trans[dir].second;
				swap(v.perm[top_to], v.perm[2]);
				swap(v.perm[bot_to], v.perm[4]);
				swap(v.perm[2], v.perm[4]);

				v.i = nx;
				v.j = ny;

				int hv = v.encode();
				if(chkmin(dist[hv], dist[hu] + cost[v.perm[4]])) {
					par[hv] = u;
					if(!in_queue[hv]) {
						in_queue[hv] = true;
						Q.push(v);
					}
				}
			}
		}
	}

	cout << answer << " ";
	
	vector<string> path; 
	while(best_end != src) {
		string s = "  ";
		s[0] = best_end.i + 'a';
		s[1] = best_end.j + '1';
		path.pb(s);

		best_end = par[best_end.encode()];
	}

	string s = "  ";
	s[0] = best_end.i + 'a';
	s[1] = best_end.j + '1';
	path.pb(s);

	reverse(ALL(path));

	for(string s: path) cout << s << " ";
	cout << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

