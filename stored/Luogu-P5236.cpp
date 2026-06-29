#include <bits/stdc++.h>

using namespace std;

constexpr int N = 3e4 + 5;
constexpr int M = 2e4 + 5;
constexpr int K = 23;

int n, m, q, u, v, w;
int dfn[N], low[N], timer = 0, cnt_nodes = 0;
int dep[N], dist[N], sum[N];
int siz[N], son[N], top[N];
int weight[N], fa[N];

vector<pair<int, int>> g1[N], g2[N];

inline void handle(int u, int v, int w) {
  cnt_nodes++;
  int cur = v, add = w;
  while (cur != fa[u]) {
    sum[cur] += add;
    add += weight[cur];
    cur = fa[cur];
  }

  sum[cnt_nodes] = sum[u];
  sum[u] = 0;

  cur = v;
  while (cur != fa[u]) {
    int min_w = min(sum[cur], sum[cnt_nodes] - sum[cur]);
    g2[cnt_nodes].push_back(make_pair(cur, min_w));
    g2[cur].push_back(make_pair(cnt_nodes, min_w));
    cur = fa[cur];
  }
}

inline void tarjan(int u, int father) {
  dfn[u] = low[u] = ++timer;
  for (auto [v, w] : g1[u]) {
    if (!dfn[v]) {
      fa[v] = u;
      weight[v] = w;
      tarjan(v, u);
      low[u] = min(low[u], low[v]);
    } else if (v != father) {
      low[u] = min(low[u], dfn[v]);
    }

    if (low[v] > dfn[u]) {
      g2[u].push_back(make_pair(v, w));
      g2[v].push_back(make_pair(u, w));
    }
  }

  for (auto [v, w] : g1[u]) {
    if (fa[v] == u || dfn[v] <= dfn[u]) continue;
    handle(u, v, w);
  }
}

inline void dfs0(int u, int father) {
  dep[u] = dep[father] + 1;
  fa[u] = father;
  siz[u] = 1;

  for (auto [v, w] : g2[u]) {
    if (v == father) continue;
    dist[v] = dist[u] + w;
    dfs0(v, u);
    siz[u] += siz[v];
    if (!son[u] || siz[v] > siz[son[u]])
      son[u] = v;
  }
}

inline void dfs1(int u, int topx) {
  top[u] = topx;
  if (!son[u]) return;
  dfs1(son[u], topx);
  for (auto [v, w] : g2[u]) {
    if (v == fa[u] || v == son[u]) continue;
    dfs1(v, v);
  }
}

inline int get_LCA(int u, int v) {
  while (top[u] != top[v]) {
    if (dep[top[u]] < dep[top[v]]) swap(u, v);
    u = fa[top[u]];
  }
  return dep[u] < dep[v] ? u : v;
}

inline int get_child(int u, int v) {
  int p = 0;
  while (top[u] != top[v]) {
    p = top[u];
    u = fa[top[u]];
  }
  return u == v ? p : son[v];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m >> q;

  cnt_nodes = n;
  for (int i = 1; i <= m; i++) {
    cin >> u >> v >> w;
    g1[u].push_back(make_pair(v, w));
    g1[v].push_back(make_pair(u, w));
  }

  tarjan(1, 0);
  dfs0(1, 0);
  dfs1(1, 1);

  for (int i = 1; i <= q; i++) {
    cin >> u >> v;

    int lca = get_LCA(u, v);

    if (lca > n) {
      int a = get_child(u, lca), b = get_child(v, lca);
      int ans = dist[u] + dist[v] - dist[a] - dist[b];
      if (sum[a] < sum[b]) swap(a, b);
      ans += min(sum[a] - sum[b], sum[lca] + sum[b] - sum[a]);
      cout << ans << "\n";
    } else {
      cout << dist[u] + dist[v] - 2 * dist[lca] << "\n";
    }
  }

  return 0;
}
