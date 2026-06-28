#include <bits/stdc++.h>

using namespace std;

constexpr int N = 2e5 + 5;
constexpr int M = 2e5 + 5;

int n, m, u, v;
int dfn[N], low[N], timer = 0, tot;
int weight[N], siz[N], cnt_scc = 0;
long long ans = 0;
bool ins[N];
stack<int> stk;

struct Graph {
  int head[N], cnt = 0;
  struct Edge { int to, next; } e[M << 1];
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
  weight[u] = -1;
  tot++;

  for (int i = g1.head[u]; ~i; i = g1.e[i].next) {
    int v = g1.e[i].to;
    if (!dfn[v]) {
      tarjan(v);
      low[u] = min(low[u], low[v]);
      if (low[v] == dfn[u]) {
        cnt_scc++;
        int t;
        do {
          t = stk.top(); stk.pop();
          ins[t] = false;
          weight[cnt_scc]++;
          g2.add_edge(cnt_scc, t);
          g2.add_edge(t, cnt_scc);
        } while (t != v);
        weight[cnt_scc]++;
        g2.add_edge(u, cnt_scc);
        g2.add_edge(cnt_scc, u);
      }
    } else if (ins[v]) {
      low[u] = min(low[u], dfn[v]);
    }
  }
}

inline void dfs(int u, int father) {
  siz[u] = u <= n;
  for (int i = g2.head[u]; ~i; i = g2.e[i].next) {
    int v = g2.e[i].to;
    if (v == father) continue;
    dfs(v, u);
    ans += 2LL * siz[u] * siz[v] * weight[u];
    siz[u] += siz[v];
  }
  ans += 2LL * siz[u] * (tot - siz[u]) * weight[u];
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
  for (int i = 1; i <= n; i++) {
    if (!dfn[i]) {
      tot = 0;
      tarjan(i);
      dfs(i, i);
    }
  }

  cout << ans << "\n";

  return 0;
}
