#include <bits/stdc++.h>

#define int long long

using namespace std;

constexpr int N = 4e5 + 5;
constexpr int K = 25;

int n, m;
int val_l[N], val_h[N];
int fa[N][K], dep[N], dis[N], global_dis[N];
int cnt_nodes = 0;
bool vis[N];

vector<int> tr[N];
vector<pair<int, int>> adj[N];

struct Edge {
  int u, v, length, height;
  bool operator>(const Edge& rhs) const { return height > rhs.height; }
};
vector<Edge> e;

struct DSU {
  int fa[N], limit = 0;
  DSU() = default;
  DSU(int n) : limit(n) { iota(fa + 1, fa + limit + 1, 1); }
  inline int find(int x) { return fa[x] = (fa[x] == x ? fa[x] : find(fa[x])); }
  inline bool same(int u, int v) { return find(u) == find(v); }
} d;

inline void clear() {
  memset(val_l, 0, sizeof(val_l));
  memset(val_h, 0, sizeof(val_h));
  memset(fa, 0, sizeof(fa));
  memset(dep, 0, sizeof(dep));
  memset(dis, 0, sizeof(dis));
  fill(tr, tr + N, vector<int>());
  fill(adj, adj + N, vector<pair<int, int>>());
}

inline void dijstra(int s) {
  memset(global_dis, 0x3f, sizeof(global_dis));
  memset(vis, false, sizeof(vis));
  struct Data { int u, dist; bool operator<(const Data& rhs) const { return dist > rhs.dist; }};
  priority_queue<Data> q;

  q.push((Data) {s, 0});
  global_dis[s] = 0;
  
  while (!q.empty()) {
    int u = q.top().u; q.pop();

    if (vis[u]) continue;
    vis[u] = true;

    for (auto [v, w] : adj[u]) {
      if (global_dis[v] > global_dis[u] + w) {
        global_dis[v] = global_dis[u] + w;
        q.push((Data) {v, global_dis[v]});
      }
    }
  }
}

inline void build_KRT(vector<Edge> e) {
  cnt_nodes = n;
  d = DSU(2 * n);
  sort(e.begin(), e.end(), greater<>());

  for (auto [u, v, l, h] : e) {
    if (!d.same(u, v)) {
      u = d.find(u), v = d.find(v);

      cnt_nodes++;
      d.fa[u] = cnt_nodes;
      d.fa[v] = cnt_nodes;

      val_l[cnt_nodes] = l;
      val_h[cnt_nodes] = h;

      tr[cnt_nodes].push_back(u), tr[u].push_back(cnt_nodes);
      tr[cnt_nodes].push_back(v), tr[v].push_back(cnt_nodes);
    }

    if (cnt_nodes == 2 * n - 1) break;
  }
}

inline void dfs(int u, int father) {
#ifdef DEBUG
  cout << "dfs(u = " << u << ", father = " << father << ")\n";
#endif

  dep[u] = dep[father] + 1;
  dis[u] = 0x3f3f3f3f;
  fa[u][0] = father;

  for (int k = 1; k < K; k++)
    fa[u][k] = fa[fa[u][k - 1]][k - 1];

  for (auto v : tr[u]) {
    if (v == father) continue;
    dfs(v, u);
    dis[u] = min(dis[u], dis[v]);
  }

  if (u <= n) dis[u] = global_dis[u];
}

inline int LCA(int u, int v) {
  if (dep[u] < dep[v]) swap(u, v);
  for (int k = K - 1; k >= 0; k--)
    if (dep[fa[u][k]] >= dep[v]) u = fa[u][k];
  if (u == v) return u;
  for (int k = K - 1; k >= 0; k--)
    if (fa[u][k] != fa[v][k]) u = fa[u][k], v = fa[v][k];
  return fa[u][0];
}

inline void solve() {
  cin >> n >> m;

  e.clear();

  for (int i = 1, u, v, l, h; i <= m; i++) {
    cin >> u >> v >> l >> h;
    e.push_back((Edge) {u, v, l, h});
    adj[u].push_back(make_pair(v, l));
    adj[v].push_back(make_pair(u, l));
  }

  build_KRT(e);
  dijstra(1);

  dfs(cnt_nodes, 0);

#ifdef DEBUG
  for (int i = 1; i <= cnt_nodes; i++)
    cout << "{" << i << ", " << val_l[i] << "} ";
  cout << "\n";
  for (int i = 1; i <= cnt_nodes; i++)
    cout << "{" << i << ", " << val_h[i] << "} ";
  cout << "\n";
  for (int i = 1; i <= cnt_nodes; i++)
    cout << "{" << i << ", " << dis[i] << "} ";
  cout << "\n";
#endif

  int q, k, s;
  cin >> q >> k >> s;

  int lastans = 0;
  while (q--) {
    int v, p;
    cin >> v >> p;

    v = (v + k * lastans - 1) % n + 1;
    p = (p + k * lastans) % (s + 1);

    int x = v;
    for (int k = K - 1; k >= 0; k--)
      if (val_h[fa[x][k]] > p) x = fa[x][k];

    lastans = dis[x];

    cout << lastans << "\n";
  }
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int test_case;
  cin >> test_case;

  while (test_case--) {
    clear();
    solve();
  }

  return 0;
}
