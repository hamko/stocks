#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

#define rep(i,n) for(long long i = 0; i < (long long)(n); i++)
#define repi(i,a,b) for(long long i = (long long)(a); i < (long long)(b); i++)
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define fi first
#define se second
#define mt make_tuple
#define mp make_pair
template<class T1, class T2> bool chmin(T1 &a, T2 b) { return b < a && (a = b, true); }
template<class T1, class T2> bool chmax(T1 &a, T2 b) { return a < b && (a = b, true); }

using ll = long long; using vll = vector<ll>; using vvll = vector<vll>; using P = pair<ll, ll>;
ll ugauss(ll a, ll b) { if (!a) return 0; if (a>0^b>0) return a/b; else return (a+(a>0?-1:1))/b+1; }
ll lgauss(ll a, ll b) { if (!a) return 0; if (a>0^b>0) return (a+(a>0?-1:1))/b-1; else return a/b; }
template <typename T, typename U> ostream &operator<<(ostream &o, const pair<T, U> &v) {  o << "(" << v.first << ", " << v.second << ")"; return o; }
template<size_t...> struct seq{}; template<size_t N, size_t... Is> struct gen_seq : gen_seq<N-1, N-1, Is...>{}; template<size_t... Is> struct gen_seq<0, Is...> : seq<Is...>{};
template<class Ch, class Tr, class Tuple, size_t... Is>
void print_tuple(basic_ostream<Ch,Tr>& os, Tuple const& t, seq<Is...>){ using s = int[]; (void)s{0, (void(os << (Is == 0? "" : ", ") << get<Is>(t)), 0)...}; }
template<class Ch, class Tr, class... Args>
auto operator<<(basic_ostream<Ch, Tr>& os, tuple<Args...> const& t) -> basic_ostream<Ch, Tr>& { os << "("; print_tuple(os, t, gen_seq<sizeof...(Args)>()); return os << ")"; }
ostream &operator<<(ostream &o, const vvll &v) { rep(i, v.size()) { rep(j, v[i].size()) o << v[i][j] << " "; o << endl; } return o; }
template <typename T> ostream &operator<<(ostream &o, const vector<T> &v) { o << '['; rep(i, v.size()) o << v[i] << (i != v.size()-1 ? ", " : ""); o << "]";  return o; }
template <typename T> ostream &operator<<(ostream &o, const deque<T> &v) { o << '['; rep(i, v.size()) o << v[i] << (i != v.size()-1 ? ", " : ""); o << "]";  return o; }
template <typename T>  ostream &operator<<(ostream &o, const set<T> &m) { o << '['; for (auto it = m.begin(); it != m.end(); it++) o << *it << (next(it) != m.end() ? ", " : ""); o << "]";  return o; }
template <typename T>  ostream &operator<<(ostream &o, const unordered_set<T> &m) { o << '['; for (auto it = m.begin(); it != m.end(); it++) o << *it << (next(it) != m.end() ? ", " : ""); o << "]";  return o; }
template <typename T, typename U>  ostream &operator<<(ostream &o, const map<T, U> &m) { o << '['; for (auto it = m.begin(); it != m.end(); it++) o << *it << (next(it) != m.end() ? ", " : ""); o << "]";  return o; }
template <typename T, typename U, typename V>  ostream &operator<<(ostream &o, const unordered_map<T, U, V> &m) { o << '['; for (auto it = m.begin(); it != m.end(); it++) o << *it; o << "]";  return o; }
vector<int> range(const int x, const int y) { vector<int> v(y - x + 1); iota(v.begin(), v.end(), x); return v; }
template <typename T> istream& operator>>(istream& i, vector<T>& o) { rep(j, o.size()) i >> o[j]; return i;}
template <typename T, typename S, typename U> ostream &operator<<(ostream &o, const priority_queue<T, S, U> &v) { auto tmp = v; while (tmp.size()) { auto x = tmp.top(); tmp.pop(); o << x << " ";} return o; }
template <typename T> ostream &operator<<(ostream &o, const queue<T> &v) { auto tmp = v; while (tmp.size()) { auto x = tmp.front(); tmp.pop(); o << x << " ";} return o; }
template <typename T> ostream &operator<<(ostream &o, const stack<T> &v) { auto tmp = v; while (tmp.size()) { auto x = tmp.top(); tmp.pop(); o << x << " ";} return o; }
template <typename T> unordered_map<T, ll> counter(vector<T> vec){unordered_map<T, ll> ret; for (auto&& x : vec) ret[x]++; return ret;};
void vizGraph(vvll& g, int mode = 0, string filename = "out.png") { ofstream ofs("./out.dot"); ofs << "digraph graph_name {" << endl; set<P> memo; rep(i, g.size())  rep(j, g[i].size()) { if (mode && (memo.count(P(i, g[i][j])) || memo.count(P(g[i][j], i)))) continue; memo.insert(P(i, g[i][j])); ofs << "    " << i << " -> " << g[i][j] << (mode ? " [arrowhead = none]" : "")<< endl;  } ofs << "}" << endl; ofs.close(); system(((string)"dot -T png out.dot >" + filename).c_str()); }
struct timeval start; double sec() { struct timeval tv; gettimeofday(&tv, NULL); return (tv.tv_sec - start.tv_sec) + (tv.tv_usec - start.tv_usec) * 1e-6; }
size_t random_seed; struct init_{init_(){ ios::sync_with_stdio(false); cin.tie(0); gettimeofday(&start, NULL); struct timeval myTime; struct tm *time_st; gettimeofday(&myTime, NULL); time_st = localtime(&myTime.tv_sec); srand(myTime.tv_usec); random_seed = RAND_MAX / 2 + rand() / 2; }} init__;
#define ldout fixed << setprecision(40)

#define EPS (double)1e-14
#define INF (ll)1e18
#define mo  (ll)(1e9+7)

// 平均x円・最低y円を毎月積み立てるドルコスト平均法でS&P 500にインデックス投資
// 短中長期で割安度を偏差値で計算して、線形結合で投資額に反映
// 最適化前後で46%資産が変わる（リークしまくりなので3割がけだとしてもやらない理由はない）

double getSigma(vector<double>& a, double x) {
    if (a.size() <= 1) return 0;
    double m = accumulate(all(a), 0.0) / a.size(), var = 0;
    for (auto&& v : a) v -= m, var += v * v;
    x -= m;
    return x / sqrt(var);
}
vector<double> getSigmaArray(vector<double>& a, ll len_max) {
    vector<double> ret;
    ll len = 0;
    rep(i, a.size()) {
        len = min(i + 1, len_max);
        vector<double> dataset; repi(j, max(0ll, i-len+1), i+1) dataset.pb(a[j]);
        double sigma = getSigma(dataset, a[i]);
        ret.pb(sigma);
    }
    return ret;
}

// param[0]: min investment, param[1~]: len_max_range
const vll len_max_range = {2, 3, 4, 6, 12, 24, 48};
ll minYen = 33333, meanYen = 200000;
tuple<double, vector<double>> evaluate(vector<double>& a, vector<double>& params) {
   ll n = a.size();
   vector<double> invest_amount_yen(n, params[0]);
   rep(j, len_max_range.size()) {
       auto len_max = len_max_range[j];
       auto sigma_array = getSigmaArray(a, len_max);
       assert(sigma_array.size() == n);
       rep(i, n) if (sigma_array[i] < 0)
           invest_amount_yen[i] += -sigma_array[i] * params[j+2];
   }
   if (accumulate(all(invest_amount_yen), 0.0) / invest_amount_yen.size() > params[1]) {
       return mt(-1, vector<double>());
   } else {
       double stock_num = 0; rep(i, n) stock_num += invest_amount_yen[i] / a[i];
       return mt(stock_num * a.back() - accumulate(all(invest_amount_yen), 0.0), invest_amount_yen);
   }
}

ll cap = 1000000;
vector<double> generateParams(void) {
    vector<double> ret(2+len_max_range.size());
    ret[0] = minYen, ret[1] = meanYen;
    repi(i, 2, 2+len_max_range.size())
        ret[i] = rand() % cap;
    return ret;
}
vector<double> baseline(void) {
    vector<double> ret(2+len_max_range.size());
    ret[0] = minYen, ret[1] = meanYen;
    return ret;
}

void randomSearch(vector<double>& a) {
    double best = -INF; vector<double> best_params;
    while (1) {
        auto params = generateParams(); auto tmp = evaluate(a, params);
        double v = get<0>(tmp); vector<double> invest_amount_yen = get<1>(tmp);
        if (v < 0) continue;
        if (best < v) {
            best = v; best_params = params;
            cout << "################" << endl;
            rep(i, a.size()) cout << a[i] << " " << invest_amount_yen[i] << endl;
            cout << best << " yen [earning]" << endl;
            cout << params << " params" << endl;
            auto baseline_params = baseline();
            cout << get<0>(evaluate(a, baseline_params)) << endl;
        }
    }
}

int main(int argc, char** argv) {
    double check_num = -1;
    if (argc >= 2)
        check_num = atof(argv[1]);
    if (argc >= 3)
        minYen = atoll(argv[2]);
    if (argc >= 4)
        meanYen = atoll(argv[3]);

    string dummy;
    vector<double> a; double x; while (cin >> dummy >> dummy >> dummy >> x) a.pb(x); // monthly sp500
    cout << a << endl;
    if (check_num >= 0) a.pb(check_num);
    randomSearch(a);
    return 0;
