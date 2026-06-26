#include <bits/stdc++.h>

using namespace std;

constexpr int N = 1e6 + 5;

int n, v[N], deg[N];
long long w[N], f[N], g[N];
queue<int> q;

inline long long calc(int u) {
  int _u = u;
  u = v[u];
  long long mx1 = f[_u], mx2 = f[_u];
  long long len = w[_u];
  long long ans1 = g[_u], ans2 = LONG_LONG_MIN;

  while (u != _u) {
    deg[u] = 0;
    ans1 = max({ans1, g[u], f[u] + len + mx1});
    ans2 = max({ans2, f[u] - len + mx2});
    mx1 = max(mx1, f[u] - len);
    mx2 = max(mx2, f[u] + len);
    len += w[u];
    u = v[u];
  }

  return max(ans1, ans2 + len);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n;

  for (int i = 1; i <= n; i++) {
    cin >> v[i] >> w[i];
    deg[v[i]]++;
  }

  for (int i = 1; i <= n; i++)
    if (!deg[i]) q.push(i);

  while (!q.empty()) {
    int x = q.front(); q.pop();
    int y = v[x];

    g[y] = max({g[y], f[y] + f[x] + w[x], g[x]});
    f[y] = max(f[y], f[x] + w[x]);

    deg[y]--;
    if (!deg[y]) q.push(y);
  }

  long long ans = 0;
  for (int i = 1; i <= n; i++)
    if (deg[i]) ans += calc(i);

  cout << ans << "\n";

  return 0;
}
