#include <bits/stdc++.h>

using namespace std;

constexpr int N = 1e6 + 5;

int n, m, u, v;
int head[N], cnt = 0;
bool vis[N];

struct Edge {
  int to, next;
  bool del = false;
} e[N << 1];

inline void add_edge(int u, int v) {
  e[cnt] = (Edge) {v, head[u]};
  head[u] = cnt++;
}

inline void check(vector<int> &a, vector<int> b) {
  for (int i = 0; i < n; i++) {
    if (a[i] != b[i]) {
      if (a[i] > b[i]) a = b;
      break;
    }
  }
}

vector<int> path;
inline void solve_tree(int u) {
  vis[u] = true;
  path.push_back(u);

  priority_queue<int, vector<int>, greater<int>> q;
  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].to;
    if (vis[v] || e[i].del) continue;
    q.push(v);
  }

  while (!q.empty()) {
    int u = q.top(); q.pop();
    solve_tree(u);
  }
}

int pre[N];
vector<int> edges;
inline bool get_circle(int u, int fa) {
  if (vis[u] == true) {
    edges.push_back(pre[u]);
    int cur = e[pre[u] ^ 1].to;
    while (cur != u) {
      edges.push_back(pre[cur]);
      cur = e[pre[cur] ^ 1].to;
    }
    return true;
  }

  vis[u] = true;

  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].to;
    if (v == fa) continue;
    pre[v] = i;
    bool res = get_circle(v, u);
    if (res) return true;
  }
  return false;
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

  if (m == n - 1) {
    solve_tree(1);
    for (auto u : path) cout << u << " ";
  } else {
    get_circle(1, 1);
    // memset(vis, false, sizeof(vis));

    vector<int> ans(n, 1e9);
    for (auto id : edges) {
      e[id].del = e[id ^ 1].del = true;
      path.clear();
      for (int i = 1; i <= n; i++) vis[i] = false;
      solve_tree(1);
      check(ans, path);
      e[id].del = e[id ^ 1].del = false;
    }

    for (auto u : ans) cout << u << " ";
  }

  return 0;
}
