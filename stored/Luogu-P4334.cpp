#include <bits/stdc++.h>

using namespace std;

constexpr int N = 2e5 + 5;
constexpr int M = 5e5 + 5;
constexpr int K = 25;

int n, m, q, u, v;
int dfn[N], out[N], low[N], timer = 0;
int dep[N], jmp[N][K];

int head[N], cnt = 0;

struct Edge {
  int to, next;
} e[M << 1];

inline void add_edge(int u, int v) {
  e[cnt] = (Edge) {v, head[u]};
  head[u] = cnt++;
}

inline void tarjan(int u, int par) {
  dfn[u] = low[u] = ++timer;

  dep[u] = dep[par] + 1;
  jmp[u][0] = par;
  for (int k = 1; k < K; k++)
    jmp[u][k] = jmp[jmp[u][k - 1]][k - 1];
  
  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].to;
    if (!dfn[v]) {
      tarjan(v, u);
      low[u] = min(low[u], low[v]);
    } else if (v != par) {
      low[u] = min(low[u], dfn[v]);
    }
  }

  out[u] = ++timer;
}

inline bool contains(int u, int v) {
  return dfn[u] <= dfn[v] && dfn[v] <= out[u];
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

inline int get_child(int u, int v) {
  for (int k = K - 1; k >= 0; k--)
    if (dep[jmp[v][k]] > dep[u]) v = jmp[v][k];
  return v;
}

inline bool query_edge(int u, int v, int g1, int g2) {
  if (u == v) return true;

  if (dfn[u] > dfn[v]) swap(u, v);
  if (dfn[g1] > dfn[g2]) swap(g1, g2);

  if (dep[g2] != dep[g1] + 1) return true;

  if (contains(u, v)) {
    if (contains(u, g1) && contains(g2, v))
      return low[g2] <= dfn[g1];
    return true;
  } else {
    int lca = get_LCA(u, v);
    return query_edge(u, lca, g1, g2) && query_edge(lca, v, g1, g2);
  }
}

inline bool query_node(int u, int v, int cut) {
  if (u == v) return u != cut;

  if (u == cut || v == cut) return false;

  if (dfn[u] > dfn[v]) swap(u, v);

  if (contains(u, v)) {
    if (contains(u, cut) && contains(cut, v))
      return low[get_child(cut, v)] < dfn[cut];
    return true;
  } else {
    int lca = get_LCA(u, v);
    if (contains(lca, cut) && (contains(cut, u) || contains(cut, v)))
      return query_node(u, jmp[lca][0], cut) && query_node(jmp[lca][0], v, cut);
    return true;
  }
} 

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  memset(head, -1, sizeof(head));

  cin >> n >> m;

  for (int i = 1; i <= m; i++) {
    cin >> u >> v;
    add_edge(u, v);
    add_edge(v, u);
  }

  tarjan(1, 1);

  cin >> q;

  for (int i = 1; i <= q; i++) {
    int typ;
    cin >> typ;
    
    if (typ == 1) {
      int u, v, g1, g2;
      cin >> u >> v >> g1 >> g2;
      cout << (query_edge(u, v, g1, g2) ? "yes" : "no") << "\n";
    } else {
      int u, v, cut;
      cin >> u >> v >> cut;
      cout << (query_node(u, v, cut) ? "yes" : "no") << "\n";
    }
  }

  return 0;
}
