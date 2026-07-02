#include <bits/stdc++.h>

using namespace std;

constexpr int N = 105;
constexpr int dx[] = {0, 0, 1, -1};
constexpr int dy[] = {1, -1, 0, 0};

int n, m, x, y;
int match[N * N];
bool blocked[N][N], vis[N * N];

vector<int> g[N * N];

inline int id(int i, int j) { return (i - 1) * n + j; }

inline bool extend(int u) {
  for (auto&& v : g[u]) {
    if (vis[v]) continue;
    vis[v] = true;
    if (!match[v] || extend(match[v])) {
      match[v] = u;
      return true;
    }
  }
  return false;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m;

  for (int i = 1; i <= m; i++) {
    cin >> x >> y;
    blocked[x][y] = true;
  }

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      if (((i + j) & 1) || blocked[i][j]) continue;
      
      for (int t = 0; t < 4; t++) {
        int x = i + dx[t], y = j + dy[t];
        if (x < 1 || x > n || y < 1 || y > n || blocked[x][y]) continue;
        g[id(i, j)].push_back(id(x, y));
      }
    }
  }

  int ans = 0;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      if (((i + j) & 1) || blocked[i][j]) continue;
      memset(vis, false, sizeof(vis));
      ans += extend(id(i, j));
    }
  }

  cout << ans << "\n";

  return 0;
}
