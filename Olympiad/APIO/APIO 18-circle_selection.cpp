/*
    https://codeforces.com/blog/entry/59650
*/
 
#include <bits/stdc++.h>
#define endl '\n'
 
#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
 
using namespace std;
template<class T, class T1> int chkmax(T &x, const T1 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T1> int chkmin(T &x, const T1 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);
 
struct Circle
{
    int idx, rad, x, y;
    Circle() { idx = rad = x = y = 0; }
    Circle(int _i, int _r, int _x, int _y)
    {
        idx = _i;
        rad = _r;
        x = _x;
        y = _y;
    }
};
 
inline int64_t sq(int x) { return x * 1ll * x; }
bool cmp(const Circle &a, const Circle &b) { return a.rad != b.rad ? a.rad > b.rad : a.idx < b.idx; }
bool intersect(const Circle &a, const Circle &b) { return sq(a.x - b.x) + sq(a.y - b.y) <= sq(a.rad + b.rad); }
 
int n;
int answer[MAXN];
Circle a[MAXN];
 
void read()
{
    cin >> n;
    for(int i = 1; i <= n; i++)
    {
        cin >> a[i].x >> a[i].y >> a[i].rad;
        a[i].idx = i;
    }
}
 
map<pair<int, int>, vector<int> > M;
 
void rebuild(int block_size)
{
    M.clear();
    for(int i = 1; i <= n; i++)
        if(!answer[a[i].idx])
            M[{a[i].x / block_size, a[i].y / block_size}].push_back(i);
}
 
void solve()
{
    sort(a + 1, a + n + 1, cmp);
 
    int B = a[1].rad;
    rebuild(B);
 
    for(int i = 1; i <= n; i++)
    {
        if(answer[a[i].idx]) continue;
 
        while(a[i].rad * 2 <= B)
        {
            B >>= 1;
            rebuild(B);
        }
 
        answer[a[i].idx] = a[i].idx;
 
        int x = a[i].x / B, y = a[i].y / B;
        for(int dx = -2; dx <= 2; dx++)
            for(int dy = -2; dy <= 2; dy++)
            {
                int nx = dx + x;
                int ny = dy + y;
 
                if(M.count({nx, ny}))
                    for(int j: M[{nx, ny}])
                        if(!answer[a[j].idx] && intersect(a[i], a[j]))
                            answer[a[j].idx] = a[i].idx;
            }
    }
 
    for(int i = 1; i <= n; i++)
        cout << answer[i] << " ";
    cout << endl;
}
 
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
 
    read();
    solve();
    return 0;
}
