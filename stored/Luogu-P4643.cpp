#include <bits/stdc++.h>

using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int n, m;
  cin >> n >> m;

  vector<int> w(n);
  for (auto&& x : w) {
    cin >> x;
    x *= 2;
  }
  
  for (int i = 1; i <= m; i++) {
    int u, v, c;
    cin >> u >> v >> c;
    w[u - 1] += c;
    w[v - 1] += c;
  }

  sort(w.begin(), w.end());

  int ans = 0;
  for (int i = 0; i < n; i += 2) ans -= w[i];
  for (int i = 1; i < n; i += 2) ans += w[i];

  cout << ans / 2 << "\n";

  return 0;
}
