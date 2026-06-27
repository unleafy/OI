#include <bits/stdc++.h>

using namespace std;

constexpr int N = 5e5 + 5;
constexpr int K = 25;

int n, q, u, v;
int head[N], cnt = 0;
int deg[N], jmp[N][K], dep[N];
int dist_cyc[N], bel_cyc[N], root[N], len[N], cnt_cyc = 0;
bool on_cyc[N];

struct Edge {
  int u, v, next;
} e[N << 1];

struct DSU {
  int fa[N], limit = 0;
  DSU() = default;
  DSU(int n) : limit(n) { iota(fa, fa + limit + 1, 0); }
  inline int find(int x) { return fa[x] == x ? fa[x] : fa[x] = find(fa[x]); }
  inline void merge(int u, int v) {
    u = find(u), v = find(v);
    if (u == v) return;
    fa[u] = v;
  }
  inline bool same(int u, int v) { return find(u) == find(v); }
} dsu;

inline void add_edge(int u, int v) {
  e[cnt] = (Edge) {u, v, head[u]};
  head[u] = cnt++;
}

inline void mark(int u, int cyc, int stp) {
  bel_cyc[u] = cyc;
  dist_cyc[u] = stp;
  on_cyc[u] = true;
  len[cnt_cyc]++;
  deg[u] = 0;
  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].v;
    if (!deg[v]) continue;
    mark(v, cyc, stp + 1);
  }
}

inline void dfs(int u, int fa, int rt) {
  root[u] = rt;
  jmp[u][0] = fa;

  for (int k = 1; k < K; k++)
    jmp[u][k] = jmp[jmp[u][k - 1]][k - 1];

  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].v;
    if (on_cyc[v]) continue;
    dep[v] = dep[u] + 1;
    dfs(v, u, rt);
  }
}

inline int get_LCA(int u, int v) {
  if (dep[u] < dep[v]) swap(u, v);
  for (int k = K - 1; k >= 0; k--)
    if (dep[jmp[u][k]] >= dep[v]) u = jmp[u][k];
  if (u == v) return u;
  for (int k = K - 1; k >= 0; k--)
    if (jmp[u][k] != jmp[v][k]) u = jmp[u][k], v = jmp[v][k];
  return jmp[u][0];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  memset(head, -1, sizeof(head));

  cin >> n >> q;

  dsu = DSU(n);

  vector<int> to(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> v;
    add_edge(i, v);
    dsu.merge(i, v);
    deg[v]++;
    to[i] = v;
  }

  queue<int> que;
  for (int i = 1; i <= n; i++) {
    if (!deg[i]) que.push(i);
  }

  while (!que.empty()) {
    int u = que.front(); que.pop();
    for (int i = head[u]; ~i; i = e[i].next) {
      int v = e[i].v;
      deg[v]--;
      if (!deg[v]) que.push(v);
    }
  }

  for (int i = 1; i <= n; i++) {
    if (deg[i]) {
      mark(i, ++cnt_cyc, 1);
    }
  }

  memset(head, -1, sizeof(head));
  cnt = 0;

  for (int i = 1; i <= n; i++) {
    add_edge(to[i], i);
  }

  for (int i = 1; i <= n; i++) {
    if (on_cyc[i]) {
      dfs(i, i, i);
    }
  }

  for (int i = 1; i <= q; i++) {
    cin >> u >> v;
    
    if (!dsu.same(u, v)) {
      cout << "-1 -1\n";
    } else if (root[u] == root[v]) {
      int LCA = get_LCA(u, v);
      cout << dep[u] - dep[LCA] << " " << dep[v] - dep[LCA] << "\n";
    } else {
      int rt_u = root[u], rt_v = root[v];
      int ans1 = dep[v] + (dist_cyc[rt_u] - dist_cyc[rt_v] + len[bel_cyc[rt_u]]) % len[bel_cyc[rt_u]];
      int ans2 = dep[u] + (dist_cyc[rt_v] - dist_cyc[rt_u] + len[bel_cyc[rt_v]]) % len[bel_cyc[rt_v]];

      auto check = [] (int a, int b, int c, int d) -> bool {
        if (max(a, b) != max(c, d))
          return max(a, b) < max(c, d);
        if (min(a, b) != min(c, d))
          return min(a, b) < min(c, d);
        return a >= b;
      };

      if (check(dep[u], ans1, ans2, dep[v]))
        cout << dep[u] << " " << ans1 << "\n";
      else
        cout << ans2 << " " << dep[v] << "\n";
    }
  }

  return 0;
}
