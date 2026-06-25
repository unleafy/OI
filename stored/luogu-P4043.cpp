#include <bits/stdc++.h>

using namespace std;

constexpr int N = 305;
constexpr int M = 1e6 + 5;
constexpr long long INF = 0x3f3f3f3f3f3f3f3fLL;

int n;
int pre_edge[N], base_val[N];
long long dist[N];
bool vis[N];
int head[N], cnt = 0;

struct Edge {
  int u, v, next;
  long long flow, capacity, cost;
} e[M << 1];

inline void add_edge(int u, int v, long long cap, int w) {
  e[cnt] = (Edge) {u, v, head[u], 0, cap, w};
  head[u] = cnt++;
  e[cnt] = (Edge) {v, u, head[v], 0, 0, -w};
  head[v] = cnt++;
}

inline bool SPFA(int S, int T) {
  memset(dist, 0x3f, sizeof(dist));
  memset(vis, false, sizeof(vis));
  memset(pre_edge, -1, sizeof(pre_edge));

  queue<int> q;
  q.push(S);
  dist[S] = 0;
  vis[S] = true;

  while (!q.empty()) {
    int u = q.front(); q.pop();
    vis[u] = false;

    for (int i = head[u]; ~i; i = e[i].next) {
      int v = e[i].v;
      if (dist[v] <= dist[u] + e[i].cost || e[i].flow >= e[i].capacity)
        continue;
      dist[v] = dist[u] + e[i].cost;
      pre_edge[v] = i;
      if (!vis[v]) {
        q.push(v);
        vis[v] = true;
      }
    }
  }
  return pre_edge[T] != -1;
}

inline pair<long long, long long> MCMF(int S, int T) {
  long long flow = 0, cost = 0;
  while (SPFA(S, T)) {
    long long cur = INF;
    for (int i = pre_edge[T]; ~i; i = pre_edge[e[i].u])
      cur = min(cur, e[i].capacity - e[i].flow);
    for (int i = pre_edge[T]; ~i; i = pre_edge[e[i].u]) {
      e[i].flow += cur;
      e[i ^ 1].flow -= cur;
    }
    flow += cur;
    cost += cur * dist[T];
  }
  return make_pair(cost, flow);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  memset(head, -1, sizeof(head));

  cin >> n;

  long long ans = 0;
  for (int i = 1; i <= n; i++) {
    int c;
    cin >> c;

    for (int j = 1; j <= c; j++) {
      long long b, t;
      cin >> b >> t;

      add_edge(i, b, INF, t);
      base_val[i]--;
      base_val[b]++;
      ans += t;
    }
  }

  for (int i = 2; i <= n; i++) {
    add_edge(i, n + 1, INF, 0);
  }

  for (int i = 1; i <= n; i++) {
    if (base_val[i] > 0) add_edge(0, i, base_val[i], 0);
    else if (base_val[i] < 0) add_edge(i, n + 2, -base_val[i], 0);
  }

  add_edge(n + 1, 1, INF, 0);

  cout << MCMF(0, n + 2).first + ans << "\n";

  return 0;
}
