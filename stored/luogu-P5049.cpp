#include <bits/stdc++.h>

using namespace std;

constexpr int N = 5e5 + 5;
constexpr int INF = 0x3f3f3f3f;

int n, m, u, v;
int head[N], cnt = 0;
bool vis[N];
vector<int> path;

struct Edge {
  int to, next;
} e[N << 1];

inline void add_edge(int u, int v) {
  e[cnt] = (Edge) {v, head[u]};
  head[u] = cnt++;
}

inline void solve_tree(int u) {
  path.push_back(u);
  vis[u] = true;

  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].to;
    if (vis[v]) continue;
    solve_tree(v);
  }
}

int pre[N];
bool in_cir[N];
inline bool get_circle(int u, int fa) {
  if (vis[u]) {
    int cur = pre[u];
    in_cir[u] = true;
    while (cur != u) {
      in_cir[cur] = true;
      cur = pre[cur];
    }
    return true;
  }

  vis[u] = true;
  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].to;
    if (v == fa) continue;
    pre[v] = u;
    if (get_circle(v, u)) return true;
  }
  return false;
}

int mx = INF;
bool rev = false;
inline void solve_cir(int u) {
  path.push_back(u);
  vis[u] = true;

  if (in_cir[u]) {
    bool need = false;
    for (int i = head[u]; ~i; i = e[i].next) {
      if (rev) break;
      int v = e[i].to;

      if (vis[v]) continue;

      if (in_cir[v]) {
        i = e[i].next;
        while (vis[e[i].to])
          i = e[i].next;

        if (~i)
          mx = e[i].to;
        else if (v > mx)
          need = rev = true;
        break;
      }
    }

    for (int i = head[u]; ~i; i = e[i].next) {
      int v = e[i].to;
      if (vis[v]) continue;
      if (in_cir[v] && need) continue;
      solve_cir(v);
    }
  } else {
    for (int i = head[u]; ~i; i = e[i].next) {
      int v = e[i].to;
      if (vis[v]) continue;
      solve_cir(v);
    }
  }
}

int main() {
  // ios::sync_with_stdio(false);
  // cin.tie(nullptr);
  // cout.tie(nullptr);

  memset(head, -1, sizeof(head));

  cin >> n >> m;

  vector<pair<int, int>> tmp;
  for (int i = 1; i <= m; i++) {
    cin >> u >> v;
    tmp.push_back(make_pair(u, v));
    tmp.push_back(make_pair(v, u));
  }

  sort(tmp.begin(), tmp.end(), [&] (auto a, auto b) { return a.second > b.second; });

  for (auto [u, v] : tmp) {
    add_edge(u, v);
  }

  if (m == n - 1) {
    solve_tree(1);
  } else {
    get_circle(1, 1);
    memset(vis, false, sizeof(vis));
    solve_cir(1);
  }

  for (auto u : path) cout << u << " ";

  return 0;
}
