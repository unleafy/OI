#include <bits/stdc++.h>

using namespace std;

constexpr int N = 1e6 + 5;
constexpr int K = 25;

int n, m, q, cnt_nodes = 0, refer[N];
int siz[N], son[N], fa[N], dep[N], top[N], dfn[N], timer = 0;
int leaves[N], lim = 0, jmp[N][K], val[N];
vector<int> g[N];

struct Edge {
  int u, v, weight, index;
  bool in = false;
  bool operator<(const Edge& rhs) const {
    return weight > rhs.weight;
  }
};
#include <bits/stdc++.h>

using namespace std;

long long ask(int a, int b);

long long solve(int subtask, int h) {
  long long n = (1ll << h) - 1;

  long long sum = 0;
  vector<long long> v1(n + 1);
  for (int i = 1; i <= n; i++)
    v1[i] = ask(i, 1), sum += v1[i];

  vector<int> spe;
  for (int i = 1; i < n; i++)
    if (!ask(i, h + 1)) spe.push_back(i);
  if (spe.size() < 3) spe.push_back(n);

  int root = -1;
  long long leaves = 0;
  for (auto u : spe) {
    long long res = ask(u, h);
    if (!res) root = u;
    else leaves += res;
  }

  long long t = 0;
  for (auto u : spe) {
    if (root != u) t += v1[u];
  }

  return (sum + 2 * leaves + (t - ask(root, 2)) / 2) / 3;
}
vector<Edge> e;

struct DSU {
  int fa[N], limit = 0;
  DSU() = default;
  DSU(int n) : limit(n) { iota(fa + 1, fa + n + 1, 1); }
  inline int find(int x) { return fa[x] = (fa[x] == x ? fa[x] : find(fa[x])); }
  inline void merge(int u, int v) { fa[find(u)] = find(v); }
  inline bool same(int u, int v) { return find(u) == find(v); }
} d;

inline void dfs0(int u, int father) {
  fa[u] = father;
  dep[u] = dep[father] + 1;
  siz[u] = 1;

  for (auto&& v : g[u]) {
    if (v == father) continue;
    dfs0(v, u);
    siz[u] += siz[v];
    if (!son[u] || siz[v] > siz[son[u]])
      son[u] = v;
  }
}

inline void dfs1(int u, int topx) {
  dfn[u] = ++timer;
  top[u] = topx;
  if (!son[u]) {
    leaves[u] = 1;
    return;
  }
  dfs1(son[u], topx);
  leaves[u] += leaves[son[u]];
  for (auto&& v : g[u]) {
    if (v == fa[u] || v == son[u]) continue;
    dfs1(v, v);
    leaves[u] += leaves[v];
  }
}

inline int get_LCA(int u, int v) {
  if (!d.same(u, v)) return 0;
  while (top[u] != top[v]) {
    if (dep[top[u]] < dep[top[v]]) swap(u, v);
    u = fa[top[u]];
  }
  return dep[u] < dep[v] ? u : v;
}

inline int solve(int u) {
  int v = 0;
  while (jmp[u][v] && val[jmp[u][v]] >= lim) v++;

  if (v == 0) return 1;

  v--;
  while (v >= 0) {
    if (val[jmp[u][v]] >= lim) u = jmp[u][v];
    v--;
  }

  return leaves[u];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  cin >> n >> m >> q;

  for (int i = 1, u, v, w; i <= m; i++) {
    cin >> u >> v >> w;
    e.push_back((Edge) {u, v, w, i, false});
  }

  sort(e.begin(), e.end());

  for (int i = 0; i < m; i++) {
    refer[e[i].index] = i;
  }

  d = DSU(2 * n);

  cnt_nodes = n;
  for (int i = 0; i < m; i++) {
    int u = d.find(e[i].u), v = d.find(e[i].v);
    if (u != v) {
      cnt_nodes++;
      d.fa[u] = d.fa[v] = cnt_nodes;
      e[i].in = true;

      g[cnt_nodes].push_back(u), g[u].push_back(cnt_nodes);
      g[cnt_nodes].push_back(v), g[v].push_back(cnt_nodes);
      val[cnt_nodes] = e[i].weight;
    }
    if (cnt_nodes == 2 * n - 1) break;
  }

  val[0] = -1;
  for (int u = cnt_nodes; u >= 1; u--) {
    if (!dfn[u]) {
      dfs0(u, 0), dfs1(u, u);
    }
  }

  for (int i = 1; i <= cnt_nodes; i++) jmp[i][0] = fa[i];

  for (int j = 1; j < K; j++)
    for (int i = 1; i <= cnt_nodes; i++)
      jmp[i][j] = jmp[jmp[i][j - 1]][j - 1];

  vector<tuple<int, int, int>> mem;
  while (q--) {
    int op;
    cin >> op;

    if (op == 1) {
      int x;
      cin >> x;

      lim = x;
      for (auto [u, v, w] : mem) {
        val[get_LCA(u, v)] = w;
      }
      mem.clear();
    } else if (op == 2) {
      int x;
      cin >> x;
      cout << solve(x) << "\n";
    } else if (op == 3) {
      int x, y;
      cin >> x >> y;

      if (e[refer[x]].in) {
        mem.push_back(make_tuple(e[refer[x]].u, e[refer[x]].v, y));
      }
    }
  }

  return 0;
}
