#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

const int N = 100005;
int n, p[N];
vector<int> adj[N];
long long dp[N][2];
bool vis[N], found = false;
int root1, root2;

// 找环上的一条边 (root1, root2)
void find_cycle(int u, int fa) {
    vis[u] = true;
    for (int v : adj[u]) {
        if (v == fa) continue;
        if (vis[v]) {
            root1 = u;
            root2 = v;
            found = true;
            return;
        }
        find_cycle(v, u);
        if (found) return;
    }
}

// 树形 DP，跳过 (u_skip, v_skip) 这条边
void dfs(int u, int fa, int u_skip, int v_skip) {
    dp[u][0] = 0;
    dp[u][1] = p[u];
    for (int v : adj[u]) {
        if (v == fa) continue;
        // 如果当前边是我们要断开的那条环边，跳过
        if ((u == u_skip && v == v_skip) || (u == v_skip && v == u_skip)) continue;
        
        dfs(v, u, u_skip, v_skip);
        dp[u][0] += max(dp[v][0], dp[v][1]);
        dp[u][1] += dp[v][0];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;
    // 题目编号 0 ~ n-1
    for (int i = 0; i < n; i++) cin >> p[i];

    for (int i = 0; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    double k;
    cin >> k;

    // 1. 找到环上的一条边
    find_cycle(0, -1);

    // 2. 第一次 DP：强制不选 root1
    dfs(root1, -1, root1, root2);
    long long res1 = dp[root1][0];

    // 3. 第二次 DP：强制不选 root2
    dfs(root2, -1, root1, root2);
    long long res2 = dp[root2][0];

    // 取两者的最大值
    long long final_ans = max(res1, res2);
    
    // 输出，注意精度控制
    cout << fixed << setprecision(1) << (double)final_ans * k << endl;

    return 0;
}
