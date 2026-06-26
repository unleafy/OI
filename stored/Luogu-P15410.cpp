#include <bits/stdc++.h>

using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int test_case;
  cin >> test_case;

  while (test_case--) {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> a(n, vector(m, 0)), b(n, vector(m, 0));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        cin >> a[i][j];
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        cin >> b[i][j];

    bool able = true;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        pair<int, int> p1 = {i, j}, p2 = {n - i - 1, j}, p3 = {i, m - j - 1}, p4 = {n - i - 1, m - j - 1};
        int v1 = a[p1.first][p1.second] ^ b[p1.first][p1.second];
        int v2 = a[p2.first][p2.second] ^ b[p2.first][p2.second];
        int v3 = a[p3.first][p3.second] ^ b[p3.first][p3.second];
        int v4 = a[p4.first][p4.second] ^ b[p4.first][p4.second];

        // cout << "(" << i << ", " << j << ")" << " " << v1 << " " << v2 << " " << v3 << " " << v4 << "\n";

        if ((v1 + v2 + v3 + v4) & 1) able = false;
      }
    }

    // cout << boolalpha << able << "\n";

    cout << (able ? "Yes" : "No") << "\n";
  }

  return 0;
}
