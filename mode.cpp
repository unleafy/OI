#include <bits/stdc++.h>
// #define int long long
#define lint long long
#define ldouble long double
#define endl '\n'
using namespace std;
const int N = 4e5+5;

struct edge{
	int idx,x,y,z;
	bitset<1> is;

	edge(int a = 0,int b = 0,int c = 0,int d = 0):
		idx(a),x(b),y(c),z(d) {}

	bool operator <(const edge &other) const {
		return this->z < other.z;
	}
	bool operator >(const edge &other) const {
		return this->z > other.z;
	}
}e[N];
int table[N];

struct node{
	int x,y,z;
}history[N];
int cnt;

int n,m,q;
int f[N<<1];
int limit;

vector<int > g[N<<1];
int cntId,id[N<<1];
int fa[N<<1][30],dep[N<<1];
int sz[N<<1],wson[N<<1];
int top[N<<1],val[N<<1];
int cntLeaves[N<<1];

inline int find(int x){
	if(f[x] == x) return x;
	return f[x] = find(f[x]);
}

inline void dfs1(int x,int father){
	fa[x][0] = father,sz[x] = 1;
	dep[x] = dep[father] + 1;
	for(int y : g[x]){
		if(y == father)
			continue;
		dfs1(y,x);
		sz[x] += sz[y];
		if(sz[y] > sz[wson[x]])
			wson[x] = y;
	}
}

inline void dfs2(int x,int TOP){
	id[x] = ++cntId;
	top[x] = TOP;
	if(wson[x]){
		dfs2(wson[x],TOP);
		cntLeaves[x] += cntLeaves[wson[x]];
	}else cntLeaves[x] = 1;
	for(int y : g[x]){
		if(y != fa[x][0] && y != wson[x]){
			dfs2(y,y);
			cntLeaves[x] += cntLeaves[y];
		}
	}
}

inline int lca(int x,int y){
	if(find(x) != find(y))
		return 0;

	while(top[x] != top[y]){
		if(dep[top[x]] < dep[top[y]])
			swap(x,y);
		x = fa[top[x]][0];
	}
	return dep[x] < dep[y] ? x : y;
}

inline int query(int x){
	int idx = 0;
	while(fa[x][idx] && val[fa[x][idx]] >= limit)
		idx++;
	
	if(idx == 0) return 1;

	idx--;
	while(idx >= 0){
		if(val[fa[x][idx]] >= limit)
			x = fa[x][idx];
		idx--;
	}

	return cntLeaves[x];
}

signed main(){
	ios::sync_with_stdio(0);
	cin.tie(0);
	
	cin >> n >> m >> q;

	int x,y,k;
	for(int i = 1; i <= m; i++){
		cin >> x >> y >> k;
		e[i] = edge(i,x,y,k);
		e[i].is.reset();
	}

	sort(e+1,e+1+m,greater<edge>());

	for(int i = 1; i <= (n<<1); i++)
		f[i] = i;

	for(int i = 1; i <= m; i++)
		table[e[i].idx] = i;

	int cntNode = n;
	for(int i = 1; i <= m; i++){
		int x = e[i].x,y = e[i].y;
		int fx = find(x),fy = find(y);
		if(fx != fy){
			e[i].is.set();
			val[++cntNode] = e[i].z;
			f[fx] = cntNode,f[fy] = cntNode;
			g[fx].push_back(cntNode),g[cntNode].push_back(fx);
			g[fy].push_back(cntNode),g[cntNode].push_back(fy);
		}
		if(cntNode == (n<<1)-1)
			break;
	}

	val[0] = -1;
	for(int idx = cntNode; idx >= 1; idx--){
		if(!id[idx]){
			dfs1(idx,0);
			dfs2(idx,idx);
		}
	}

	for(int j = 1; (1<<j) <= (n<<1); j++)
		for(int i = 1; i <= (n<<1); i++)
			fa[i][j] = fa[fa[i][j-1]][j-1];
	int op;
	while(q--){
		cin >> op;
		if(op == 1){
			cin >> x;
			limit = x;

			for(int i = 1; i <= cnt; i++)
				val[lca(history[i].x,history[i].y)] = history[i].z;
            cnt = 0;
		}else if(op == 2){
			cin >> x;
			cout << query(x) << endl;
		}else if(op == 3){
			cin >> x >> y;
			edge p = e[table[x]];
			if(p.is == true)
				history[++cnt] = (node){p.x,p.y,y};
		}
	}
	return 0;
}
