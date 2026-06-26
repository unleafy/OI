#include <bits/stdc++.h>

using namespace std;

constexpr int N = 1e5 + 5;

int n, u, v;
long long w;

int head[N], cnt = 0;

bool vis[N], on_cycle[N];
int from[N], from_edge[N];

vector<int> cycle;
vector<long long> cycle_weight;

long long max_dep[N], max_sub_diam = 0;
long long pre_dep[N], pre_diam[N];
long long suf_dep[N], suf_diam[N];

struct Edge {
  int u, v, next;
  long long w;
} e[N << 1];

inline void add_edge(int u, int v, long long w) {
  e[cnt] = (Edge) {u, v, head[u], w};
  head[u] = cnt++;
  e[cnt] = (Edge) {v, u, head[v], w};
  head[v] = cnt++;
}

bool found = false;
inline void get_cycle(int u, int father) {
  if (vis[u]) {
    cycle.push_back(u);
    cycle_weight.push_back(e[from_edge[u]].w);
    on_cycle[u] = true;

    int cur = from[u];

    while (cur != u) {
      cycle.push_back(cur);
      cycle_weight.push_back(e[from_edge[cur]].w);
      on_cycle[cur] = true;
      cur = from[cur];
    }

    found = true;

    return;
  }

  vis[u] = true;

  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].v;
    if (v == father || (found && vis[v])) continue;

    from[v] = u;
    from_edge[v] = i;
    get_cycle(v, u);
  }
}

inline void dfs(int u, int father) {
  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].v;
    if (v == father || on_cycle[v]) continue;
    dfs(v, u);

    max_sub_diam = max(max_sub_diam, max_dep[u] + max_dep[v] + e[i].w);
    max_dep[u] = max(max_dep[u], max_dep[v] + e[i].w);
  } 
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  memset(head, -1, sizeof(head));
  
  cin >> n;

  for (int i = 1; i <= n; i++) {
    cin >> u >> v >> w;
    add_edge(u, v, w);
  }

  get_cycle(1, 1);

#ifdef DEBUG
  cout << "cycle[] = ";
  for (auto node : cycle) cout << node << " ";
  cout << "\n";
  cout << "cycle_weight[] = ";
  for (auto weight : cycle_weight) cout << weight << " ";
  cout << "\n";
#endif

  for (auto node : cycle)
    dfs(node, -1);

  int m = (int) cycle.size();

  long long cur = 0, dist = max_dep[cycle.front()];
  pre_dep[0] = max_dep[cycle.front()];
  pre_diam[0] = 0;

  for (int i = 1; i < m; i++) {
    cur += cycle_weight[i - 1];
    int u = cycle[i];
    pre_dep[i] = max(pre_dep[i - 1], max_dep[u] + cur);
    pre_diam[i] = max(pre_diam[i - 1], max_dep[u] + cur + dist);
    dist = max(dist, max_dep[u] - cur);
  }

  cur = 0; dist = max_dep[cycle.back()];
  suf_dep[m - 1] = max_dep[cycle.back()];
  suf_diam[m - 1] = 0;

  for (int i = m - 2; i >= 0; i--) {
    cur += cycle_weight[i];
    int u = cycle[i];
    suf_dep[i] = max(suf_dep[i + 1], max_dep[u] + cur);
    suf_diam[i] = max(suf_diam[i + 1], max_dep[u] + cur + dist);
    dist = max(dist, max_dep[u] - cur);
  }

  long long min_diam = pre_diam[m - 1];
  for (int i = 0; i < m - 1; i++) 
    min_diam = min(min_diam, max({pre_diam[i], suf_diam[i + 1], pre_dep[i] + suf_dep[i + 1] + cycle_weight.back()}));

  long long ans = max(min_diam, max_sub_diam);

  cout << (ans >> 1) << (ans & 1 ? ".5" : ".0") << "\n";

  return 0;
}
