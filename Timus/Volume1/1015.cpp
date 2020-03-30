/*
	We can hash every die and use map (BST) or unordered_map (hash_table) to store the indices for every hash value. The tricky part is how to do the hashing.

	Well we are searching for a function that gives the same value for isomorphic dice. Well something that will work is:
		1) We first fix 1 to be on the bottom (in any way).
		2) Now we rotate the dice around the bottom so that we will have the smallest number on the left.
		3) Well the above two steps are enough to enumerate the distinct dice.

	Also we can note that we have much less than 7^6 possible hash codes, so we don't really need a BST/hash_table, but a simple array of lists will be enough. 

	The complexity will be O(N).
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

void read() {
	cin >> n;
}

vector<int> indices[MAXN];
int answer_pos[MAXN];

void solve() {
	memset(answer_pos, -1, sizeof(answer_pos));

	vector<vector<int> > answer;

	for(int i = 1; i <= n; i++) {
		int l, r, f, b, u, d;
		cin >> l >> r >> u >> f >> d >> b;
	
		if(d != 1) {
			if(u == 1) {
				swap(u, d);
				swap(l, r);
			} else {
				if(l == 1) {
					swap(l, d);
					swap(r, u);
					swap(l, r);
				} 

				if(r == 1) {
					swap(r, d);
					swap(l, u);
					swap(l, r);
				}

				if(f == 1) {
					swap(f, d);
					swap(b, u);
					swap(f, b);
				}

				if(b == 1) {
					swap(b, d);
					swap(f, u);
					swap(f, b);
				}
			}
		}

		// Now 1 is on the bottom.

		vector<int> li = {l, f, r, b};

		// Find lexicographically smallest shift (i.e. step 2 where we rotate the 4 sides around the bottom).
		int mn = min(min(l, f), min(r, b));
		for(int i = 0; i < 4; i++) {
			if(li[i] == mn) {
				for(int j = 0; j < i; j++) {
					li.pb(li[j]);
				}

				li.erase(li.begin(), li.begin() + i);
				break;
			}
		}
		
		// Add top to the encoding (as we always have 1 as bottom)
		li.pb(u);

		int64_t h = 0;
		for(auto p: li) {
			h *= 7;
			h += p;
		}

		if(answer_pos[h] == -1) {
			answer_pos[h] = SZ(answer);
			answer.pb({});
		}

		answer[answer_pos[h]].pb(i);
 	}
	
	cout << SZ(answer) << endl;
	for(auto li: answer) {
		for(int val: li) {
			cout << val << " ";
		}
		
		cout << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

