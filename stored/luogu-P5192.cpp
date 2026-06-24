#include <bits/stdc++.h>

using namespace std;

constexpr int N = 1505;
constexpr int M = 1e6 + 5;
constexpr long long INF = 0x3f3f3f3f3f3f3f3f;

int n, m, g[N], c[N], d[N], base_val[N];
int dist[N];
int head[N], cur[N], cnt = 0;

struct Edge {
  int u, v, next;
  long long flow, capacity, bottom;
} e[M << 1];

inline void add_edge(int u, int v, long long cap, long long l) {
  e[cnt] = (Edge) {u, v, head[u], 0, cap, l};
  head[u] = cnt++;
  e[cnt] = (Edge) {v, u, head[v], 0, 0, l};
  head[v] = cnt++;
}

inline bool bfs(int S, int T) {
  memset(dist, 0, sizeof(dist));
  memcpy(cur, head, sizeof(head));

  queue<int> q;
  q.push(S);
  dist[S] = 1;

  while (!q.empty()) {
    int u = q.front();
    q.pop();

#ifdef DEBUG
    cout << u << "\n";
#endif

    for (int i = head[u]; ~i; i = e[i].next) {
      int v = e[i].v;
      if (dist[v] || e[i].flow >= e[i].capacity) continue;
      dist[v] = dist[u] + 1;
      q.push(v);
      if (v == T) return true;
    }
  }
  return false;
}

inline long long dfs(int u, int T, long long flow) {
  if (u == T) return flow;
  long long rest = flow;
  for (int& i = cur[u]; ~i; i = e[i].next) {
    int v = e[i].v;
    if (dist[v] == dist[u] + 1 && e[i].flow < e[i].capacity) {
      long long res = dfs(v, T, min(rest, e[i].capacity - e[i].flow));
      e[i].flow += res;
      e[i ^ 1].flow -= res;
      rest -= res;
      if (!rest) break;
    }
  }
  return flow - rest;
}

inline long long dinic(int S, int T) {
  long long flow = 0;
  while (bfs(S, T))
    flow += dfs(S, T, INF);
  return flow;
}

inline void clear() {
  memset(head, -1, sizeof(head));
  memset(base_val, 0, sizeof(base_val));
  cnt = 0;
}

void solve(int n, int m) {
  int S = 0, T = n + m + 1;
  int SS = n + m + 2, TT = n + m + 3;

  for (int i = 1; i <= m; i++) {
    cin >> g[i];
    add_edge(i + n, T, INF, g[i]);
    base_val[i + n] -= g[i];
    base_val[T] += g[i];
  }

  for (int i = 1; i <= n; i++) {
    cin >> c[i] >> d[i];
    for (int j = 1, T, L, R; j <= c[i]; j++) {
      cin >> T >> L >> R;
      T++;
      add_edge(i, T + n, R - L, L);
      base_val[i] -= L;
      base_val[T + n] += L;
    }
    add_edge(S, i, d[i], 0);
  }

  int spe = cnt;
  add_edge(T, S, INF, 0);

  long long sum = 0;
  for (int i = 0; i <= n + m + 1; i++) {
    if (base_val[i] > 0) {
      add_edge(SS, i, base_val[i], 0);
      sum += base_val[i];
    } else if (base_val[i] < 0) {
      add_edge(i, TT, -base_val[i], 0);
    }
  }

  long long base_flow = dinic(SS, TT);
  
#ifdef DEBUG
  cout << "base_flow = " << base_flow << "\n";
#endif

  if (base_flow != sum) {
    cout << -1 <<"\n";
    return;
  }

  long long ans = e[spe].flow;
  e[spe].flow = e[spe ^ 1].flow = e[spe].capacity = 0;

  cout << ans + dinic(S, T) << "\n";

  return;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  while (cin >> n >> m) {
    clear();
    solve(n, m);
    cout << "\n";
  }
  
  return 0;
}
