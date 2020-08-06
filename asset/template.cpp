#include <bits/stdc++.h>
#include <sys/time.h>
#include <memory.h>
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

class property {
    string name = "";
    double num = 0;
    double price = 0;
    double cash = 0;
    double debt = 0;
public:
    property() {}
    property(string name) {this->name = name;}
    string getName(void) {return name;}
    void setName(string name_) {name=name_;}
    double getNum(void) {return num;}
    void setNum(double num_) {num=num_;}
    void setPrice(double price_) {price=price_;}
    double getPrice(void) {return price;}
    void setCash(double cash_) {cash=cash_;}
    double getCash(void) {return cash;}
    void setDebt(double debt_) {debt=debt_;}
    double getDebt(void) {return debt;}
    friend ostream& operator<< (ostream& os, const shared_ptr<property> p);
};
ostream& operator<< (ostream& os, const shared_ptr<property> p) {
    os << "(name = " << p->name << ", num = " <<  p->num << ", price = " << p->price << ", cash = " << p->cash << ", debt = " << p->debt << ")";
    return os;
}

class transaction {
    shared_ptr<property> target;
    string date = "1900-01-01";
    string type; // buy, sell, update
    string description = ""; 
    shared_ptr<property> diff;
    friend ostream& operator<< (ostream& os, const shared_ptr<transaction> t);
    bool applied = false;
public:
    transaction(shared_ptr<property> target_, string date_, string description_ = "") : target(target_), date(date_), description(description_) {
        diff = make_shared<property>();
    }
    string getDate(void) {return date;}
    string getType(void) {return type;}
    string getDescription(void) {return description;}

    void buy(double num, double fee = 0) { 
        type = "buy"; diff->setNum(+num); diff->setCash(-fee); 
    }
    void sell(double num, double fee = 0) { 
        type = "sell"; diff->setNum(-num); diff->setCash(-fee); 
    }
    void update(double price) {
        type = "update"; diff->setPrice(price); 
    }
    void get(double cash) {
        type = "earn"; diff->setCash(cash); 
    }
    void pay(double cash) { 
        type = "pay"; diff->setCash(-cash); 
    }

    void apply(void) {
        if (applied) assert(0);
        if (type == "buy" || type == "sell" || type == "earn" || type == "pay") {
            target->setNum(target->getNum() + diff->getNum());
            target->setCash(target->getCash() + diff->getCash());
        } else if (type == "update") {
            target->setPrice(diff->getPrice());
        } else {
            assert(0);
        }
        applied = true;
    }
};
ostream& operator<< (ostream& os, const shared_ptr<transaction> t) {
    os << "date = " << t->date << ", name.type = " << t->target->getName() << "." << t->type << ", diff = " << t->diff << " #" << t->description;
    return os;
}

void test(void) {
    shared_ptr<property> hamko = make_shared<property>();
    hamko->setName("hamko");
    cout << hamko << endl;

    shared_ptr<transaction> salary = make_shared<transaction>(hamko, "2020-06-25", "test hamko hamham");
    salary->get(1000.);
    cout << salary << endl;
    salary->apply();

    cout << hamko << endl;
}
void interactive(void) {
    string tmp;
    map<string, shared_ptr<property>> properties;
    while (getline(cin, tmp)) {
        stringstream ss(tmp);
        vector<string> params;
        { string t; while (ss >> t) params.push_back(t); }
        if (!params.size()) continue;
        cout << params << endl;
        assert(params.size() >= 3);
        string date = params[0];
        string name = params[1];
        string type = params[2];
        if (!properties.count(name)) {
            shared_ptr<property> new_property = make_shared<property>();
            new_property->setName(name);
            properties[name] = new_property;
        }
        shared_ptr<transaction> t = make_shared<transaction>(properties[name], date, tmp);
        if (type == "buy") {
            assert(params.size() >= 4);
            double num = stod(params[3]);
            double fee = (params.size() >= 5 ?  stod(params[4]) : 0);
            t->buy(num, fee), t->apply();
        } else if (type == "sell") {
            assert(params.size() >= 4);
            double num = stod(params[3]);
            double fee = (params.size() >= 5 ?  stod(params[4]) : 0);
            t->sell(num, fee), t->apply();
        } else if (type == "earn") {
            assert(params.size() >= 4);
            double money = stod(params[3]);
            t->get(money), t->apply();
        } else if (type == "pay") {
            assert(params.size() >= 4);
            double money = stod(params[3]);
            t->pay(money), t->apply();
        } else if (type == "update") {
            assert(params.size() >= 4);
            double price = stod(params[3]);
            t->update(price), t->apply();
        } else {
            assert(0);
        }
        cout << t << endl;
        for (auto x : properties) 
            cout << x.se << endl;
    }
}

int main(void) {
    interactive();

    return 0;
}
