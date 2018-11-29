/*
    The main observation is that the driver can be a robot only if it for every pair of cities that appear in every two routes in the same order, all cities between these two cities should be the same (and in the same order).
    This is equivalent to the city exactly after the first city in the pair being equal in all pairs. So here we have an easy O(K ^ 2) solution.

    To achieve a better solution we will divide the routes into two types - heavy and light. A route is heavy if it has length greater than or equal to B (a constant that we will fix).

    Obviously there are O(Q / B) heavy routes and for each of them we will fix every other route and do the stupid solution in O(min(K[i], K[j])) time.

    Now we need to check only each pair of light routes. The number of such pairs can be large, but fortunately we can keep an hash table P[from][to] which gives the city after "from" on the path from "from" to "to".
    We will fix all pairs of positions in every light route and check/update P[L][R]. As the lengths are small this will have complexity O(Q * B) or O(Q * B * log K).

    The overall complexity is O(Q * K / B + Q * B * log K). If we choose B = sqrt(K \ log K), the complexity will be O(K * sqrt(K * log Q)).
    By using a hash table we can avoid the log.
*/

#include <bits/stdc++.h>
#define endl '\n'

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define pb push_back

using namespace std;
template<class T, class T1> int chkmax(T &x, const T1 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T1> int chkmin(T &x, const T1 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (int)3e5 + 42;
const int B = 60;

int n, Q;

void read()
{
    cin >> n >> Q;
}

map<int, int> memo[MAXN];
vector<int> vec[MAXN];

int pos[MAXN];

void solve()
{
    for(int i = 0; i < Q; i++)
    {
        int sz;
        cin >> sz;

        vec[i].clear();
        while(sz--)
        {
            int val;
            cin >> val;
            vec[i].pb(val);
        }
    }

    for(int i = 0; i <= n; i++) memo[i].clear();
    for(int i = 0; i < Q; i++)
        if(SZ(vec[i]) < B)
            for(int p = 0; p < SZ(vec[i]); p++)
                for(int q = p + 1; q < SZ(vec[i]); q++)
                    if(memo[vec[i][p]].count(vec[i][q]))
                    {
                        if(memo[vec[i][p]][vec[i][q]] != vec[i][p + 1])
                        {
                            cout << "Human" << endl;
                            return;
                        }
                    }
                    else memo[vec[i][p]][vec[i][q]] = vec[i][p + 1];

    for(int i = 1; i <= n; i++) pos[i] = -1;
    for(int i = 0; i < Q; i++)
        if(SZ(vec[i]) >= B)
        {
            for(int p = 0; p < SZ(vec[i]); p++) pos[vec[i][p]] = p;
            for(int j = 0; j < Q; j++)
                if(j != i)
                {
                    int mx = -1;
                    for(int q = SZ(vec[j]) - 1; q >= 0; q--)
                        if(pos[vec[j][q]] != -1)
                        {
                            if(pos[vec[j][q]] < mx && vec[j][q + 1] != vec[i][pos[vec[j][q]] + 1])
                            {
                                cout << "Human" << endl;
                                return;
                            }

                            chkmax(mx, pos[vec[j][q]]);
                        }
                }

            for(int p = 0; p < SZ(vec[i]); p++) pos[vec[i][p]] = -1;
        }

    cout << "Robot" << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;

    while(t--)
    {
        read();
        solve();
    }

    return 0;
}
