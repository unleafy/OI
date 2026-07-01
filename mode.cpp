#include<bits/stdc++.h>
using namespace std;
#define pii pair<pair<int,int>,pair<int,int> >
#define mk make_pair
#define fi first
#define se second
vector<pii > v;
int cnt[1100];
bool vis[1100];
pair<int,int> ans[1100];
int main()
{
    int T;
    cin >> T;
    while(T--)
    {
        v.clear();
        memset(vis,0,sizeof(vis));
        memset(cnt,0,sizeof(cnt));
        int n;
        cin >> n;
        for(int i = 1;i <= n;i++)
        {
            for(int j = 1;j <= n + 1;j++)
            {
                int x,y;
                cin >> x >> y;
                v.push_back(mk(mk(x,y),mk(i,j)));
            }
        }
        sort(v.begin(),v.end());
        for(int i = 0;i < v.size();i++)
        {
            if(vis[v[i].se.fi])
            {
                continue;
            }
            cnt[v[i].se.fi]++;
            if(cnt[v[i].se.fi] == 1)
            {
                ans[v[i].se.fi].fi = v[i].se.se;
            }
            else
            {
                ans[v[i].se.fi].se = v[i].se.se;
                for(int i = 1;i <= n;i++)
                {
                    cnt[i] = 0;
                }
                vis[v[i].se.fi] = 1;
            }
        }
        for(int i = 1;i <= n;i++)
        {
            cout << ans[i].fi << " " << ans[i].se << endl;
        }
    }
    return 0;
}
