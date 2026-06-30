#include <bits/stdc++.h>

using namespace std;

constexpr int N = 1e5 + 5;

int n, u, v, p[N], rt_u, rt_v;
long long dp[N][2];
bool vis[N];

vector<int> adj[N];

inline bool get_loop(int u, int fa) {
  vis[u] = true;

  for (auto v : adj[u]) {
    if (v == fa) continue;
    if (vis[v]) {
      rt_u = u, rt_v = v;
      return true;
    }
    if (get_loop(v, u)) return true;
  }
  return false;
}

inline void dfs(int u, int fa, int rt_u, int rt_v) {
  dp[u][0] = 0;
  dp[u][1] = p[u];

  for (auto v : adj[u]) {
    if (v == fa) continue;
    if ((u == rt_u && v == rt_v) || (v == rt_u && u == rt_v)) continue;

    dfs(v, u, rt_u, rt_v);

    dp[u][0] += max(dp[v][0], dp[v][1]);
    dp[u][1] += dp[v][0];
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n;

  for (int i = 1; i <= n; i++)
    cin >> p[i];

  for (int i = 1, u, v; i <= n; i++) {
    cin >> u >> v;
    u++, v++;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  double k;
  cin >> k;

  get_loop(1, 0);

  long long ans = 0;

  dfs(rt_u, 0, rt_u, rt_v);
  ans = max(ans, dp[rt_u][0]);

  dfs(rt_v, 0, rt_u, rt_v);
  ans = max(ans,dp[rt_v][0]);

  cout << setiosflags(ios::fixed) << setprecision(1) << (double) ans * k << "\n";

  return 0;
}
