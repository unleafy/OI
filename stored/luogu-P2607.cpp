#include <bits/stdc++.h>

using namespace std;

constexpr int N = 1e6 + 5;

int n, s0, s1;
long long w[N], f[N][2], g[N][2];
bool vis[N];

int head[N], cnt = 0;
struct Edge {
  int to, next;
} e[N << 1];

inline void add_edge(int u, int v) {
  e[cnt] = (Edge) {v, head[u]};
  head[u] = cnt++;
}

inline bool get_circle(int u, int root) {
  vis[u] = true;
  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].to;
    if (v == root) {
      s0 = u;
      s1 = v;
      return true;
    }
    if (!vis[v] && get_circle(v, root)) return true;
  }
  return false;
}

inline void Dp(int u, int root) {
  f[u][0] = 0;
  f[u][1] = w[u];

  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].to;
    if (v == root) continue;
    Dp(v, root);
    f[u][0] += max(f[v][0], f[v][1]);
    f[u][1] += f[v][0];
  }
}

inline void Dp2(int u, int root) {
  g[u][0] = 0;
  g[u][1] = w[u];

  for (int i = head[u]; ~i; i = e[i].next) {
    int v = e[i].to;
    if (v == root) continue;
    Dp2(v, root);
    g[u][0] += max(g[v][0], g[v][1]);
    g[u][1] += g[v][0];
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  memset(head, -1, sizeof(head));

  cin >> n;

  for (int i = 1, u; i <= n; i++) {
    cin >> w[i] >> u;
    add_edge(u, i);
  }

  long long ans = 0;
  for (int i = 1; i <= n; i++) {
    if (vis[i]) continue;

    // if (!get_circle(i, i)) {
    //   Dp(i, i);
    //   ans += max(f[i][0], f[i][1]);
    // }
    if (get_circle(i, i)){
      Dp(s0, s0), Dp2(s1, s1);
      ans += max(f[s0][0], g[s1][0]);
    } 
  }

  cout << ans << "\n";

  return 0;
}
