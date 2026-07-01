#include <bits/stdc++.h>

using namespace std;

constexpr int N = 1e5 + 5;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int test_case;
  cin >> test_case;

  while (test_case--) {
    int n;
    cin >> n;
    
    vector<tuple<int, int, int, int>> vec;
    vector<pair<int, int>> ans(n + 1);
    vector<int> mark(n + 1);
    vector<bool> vis(n + 1);

    for (int i = 1; i <= n; i++) {
      for (int j = 1, x, y; j <= n + 1; j++) {
        cin >> x >> y;
        vec.push_back(make_tuple(x, y, i, j));
      }
    }

    sort(vec.begin(), vec.end());

    for (auto [x, y, col, id] : vec) {
      if (vis[col]) continue;

      mark[col]++;
      if (mark[col] == 1) {
        ans[col].first = id;
      } else {
        ans[col].second = id;
        for (int i = 1; i <= n; i++)
          mark[i] = 0;
        vis[col] = true;
      }
    }

    for (int i = 1; i <= n; i++) {
      cout << ans[i].first << " " << ans[i].second << "\n";
    }
  }

  return 0;
}
