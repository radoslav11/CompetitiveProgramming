/*
   As the constraints are low, we can just find the LARGEST valid square and remove it. After we remove it, we set all of its cells to some value (for example #).
   This will represent that the cell can take any value. This way the complexity will be O(N * W * H * min(W, H)^2) as we have N sequential steps and for each of them:
   1) We fix a candidate low-right cell.     O(W * H)
   2) We fix the length of the side of the square.    O(min(W, H))
   3) We check if the square is valid.     O(min(W, H))

   Again, as the constraints are low enough, this will be fast enough. However, there exists a simple O(N * W * H * log) solution, which again does N steps, but
   find a valid square in O(W * H * log).

   Something we should consider during implementation is to not choose a square with all of its values equal to #. 
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
const int MAXN = (1 << 6);

// Corner codes
const vector<vector<int> > corner_codes = {
	{218, 191},
	{192, 217},
};

const int vertical = 179;
const int horizontal = 196;

/*
const vector<vector<int> > corner_codes = {
	{'+', '+'},
	{'+', '+'},
};

const int vertical = '*';
const int horizontal = ',';
*/

const int h = 20, w = 50;

unsigned char grid[h + 5][w + 5]; 

void read() {
	for(int i = 0; i < h; i++) {
		cin >> grid[i];
	}
}

bool check_cell(int val, const int &req, bool &flag) {
	if(val == req) {
		flag = true;
		return true;
	}	

	return val == '#';
}

bool valid(int ux, int uy, int dx, int dy) {
	// for all #
	bool special_flag = false;

	// horizontal
	for(int j = uy + 1; j <= dy - 1; j++) {
		if(!check_cell(grid[ux][j], horizontal, special_flag)) {
			return false;
		}

		if(!check_cell(grid[dx][j], horizontal, special_flag)) {
			return false;
		}
	}

	// vertical
	for(int i = ux + 1; i <= dx - 1; i++) {
		if(!check_cell(grid[i][dy], vertical, special_flag)) {
			return false;
		}

		if(!check_cell(grid[i][uy], vertical, special_flag)) {
			return false;
		}
	}

	// corners
	if(!check_cell(grid[ux][uy], corner_codes[0][0], special_flag)) {
		return false;
	}

	if(!check_cell(grid[ux][dy], corner_codes[0][1], special_flag)) {
		return false;
	}

	if(!check_cell(grid[dx][uy], corner_codes[1][0], special_flag)) {
		return false;
	}

	if(!check_cell(grid[dx][dy], corner_codes[1][1], special_flag)) {
		return false;
	}

	// all # check
	if(!special_flag) { 
		return false;
	}

	return true;
}

bool get_next(pair<int, pair<int, int> > &ret) {
	bool has_valid = false;
	int mx_len = -1;

	for(int i = 0; i < h; i++) {
		for(int j = 0; j < w; j++) {
			for(int len = 2; i - len + 1 >= 0 && j - len + 1 >= 0; len++) {
				if(valid(i - len + 1, j - len + 1, i, j) && chkmax(mx_len, len)) {
					ret = {len, {j - len + 1, i - len + 1}};
					has_valid = true;
				}
			}
		}
	}

	return has_valid;
}

void remove(pair<int, pair<int, int> > p) {
	// Can be made linear in time, but we don't care as get_next() is much slower
	for(int i = p.second.second; i < p.second.second + p.first; i++) {
		for(int j = p.second.first; j < p.second.first + p.first; j++) {
			if(i == p.second.second || i == p.second.second + p.first - 1 || j == p.second.first || j == p.second.first + p.first - 1) {
				// (i, j) is on the border
				grid[i][j] = '#';
			}
		}
	}
}

void solve() {
	// Size, (X, Y)
	vector<pair<int, pair<int, int> > > answer;

	pair<int, pair<int, int> > nxt;
	while(get_next(nxt)) {
		answer.pb(nxt);
		remove(nxt);
	}
	
	reverse(ALL(answer));

	cout << SZ(answer) << endl;
	for(auto &it: answer) {
		cout << it.second.first << " " << it.second.second << " " << it.first << endl; 
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

