#include <bits/stdc++.h>

using namespace std;

constexpr int N = 1e6 + 5;

int n, m, u, v, dfn[N], low[N], timer = 0;
int cnt_scc = 0, bel[N], mark[N];
int fa[N], son[N], siz[N], top[N], dep[N], dist[N];
bool ins[N];
stack<int> stk;

struct Graph {
  int head[N], cnt = 0;
  struct Edge { int to, next; } e[N << 1];

  Graph() : cnt(0) { memset(head, -1, sizeof(head)); }
  inline void add_edge(int u, int v) {
    e[cnt] = (Edge) {v, head[u]};
    head[u] = cnt++;
  }
} g1, g2;

inline void tarjan(int u) {
  dfn[u] = low[u] = ++timer;
  ins[u] = true;
  stk.push(u);

  for (int i = g1.head[u]; ~i; i = g1.e[i].next) {
    int v = g1.e[i].to;
    if (!dfn[v]) {
      tarjan(v);
      low[u] = min(low[u], low[v]);

      if (low[v] == dfn[u]) {
        cnt_scc++;
        mark[u] = 1;

        int t;
        do {
          t = stk.top();
          stk.pop();
          ins[t] = false;

          g2.add_edge(cnt_scc, t);
          g2.add_edge(t, cnt_scc);
        } while (t != v);

        g2.add_edge(u, cnt_scc);
        g2.add_edge(cnt_scc, u);
      }
    } else if (ins[v]) {
      low[u] = min(low[u], dfn[v]);
    }
  }
}

inline void dfs0(int u, int father) {
  fa[u] = father;
  dep[u] = dep[father] + 1;
  siz[u] = 1;

  for (int i = g2.head[u]; ~i; i = g2.e[i].next) {
    int v = g2.e[i].to;
    if (v == father) continue;
    dfs0(v, u);

    siz[u] += siz[v];
    if (!son[u] || siz[son[u]] < siz[v])
      son[u] = v;
  }
}

inline void dfs1(int u, int topx) {
  top[u] = topx;
  if (!son[u]) return;
  dfs1(son[u], topx);

  for (int i = g2.head[u]; ~i; i = g2.e[i].next) {
    int v = g2.e[i].to;
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m;

  for (int i = 1; i <= m; i++) {
    cin >> u >> v;
    g1.add_edge(u, v);
    g1.add_edge(v, u);
  }

  cnt_scc = n;
  tarjan(1);

  dfs0(1, 0);
  dfs1(1, 1);

  int q;
  cin >> q;

  for (int i = 1; i <= q; i++) {
    cin >> u >> v;
    int LCA = get_LCA(u, v);
    cout << (dep[u] + dep[v] - 2 * dep[LCA]) / 2 + 1 << "\n";
  }

  return 0;
}
