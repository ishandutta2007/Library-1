#ifdef __LOCAL
// clang-format off
#undef checkpoint
#undef debug
#undef debugArray
#undef debugMatrix

// --- operator<< overloads ---

// pair
template<class T,class U>std::ostream&operator<<(std::ostream&os,const std::pair<T,U>&x){return os<<"("<<x.first<<", "<<x.second<<")";}
// tuple
template<class Tup,std::size_t...I>void dbg_print_tuple(std::ostream&os,const Tup&x,std::index_sequence<I...>){(void)(int[]){(os<<std::get<I>(x)<<", ",0)...};}
template<class...Args>std::ostream&operator<<(std::ostream&os,const std::tuple<Args...>&x){constexpr std::size_t N=sizeof...(Args);os<<"(";if constexpr(N>=2)dbg_print_tuple(os,x,std::make_index_sequence<N-1>());return os<<std::get<N-1>(x)<<")";}
// vector → [...]
template<class T>std::ostream&operator<<(std::ostream&os,const std::vector<T>&v){os<<'[';for(int i=0,n=v.size();i<n;++i)os<<(i?", ":"")<<v[i];return os<<']';}
// deque → [...]
template<class T>std::ostream&operator<<(std::ostream&os,const std::deque<T>&v){os<<'[';int i=0;for(const auto&x:v)os<<(i++?", ":"")<<x;return os<<']';}
// array → [...]
template<class T,std::size_t N>std::ostream&operator<<(std::ostream&os,const std::array<T,N>&a){os<<'[';for(std::size_t i=0;i<N;++i)os<<(i?", ":"")<<a[i];return os<<']';}
// set, multiset → {...}
template<class T>std::ostream&operator<<(std::ostream&os,const std::set<T>&s){os<<'{';int i=0;for(const auto&x:s)os<<(i++?", ":"")<<x;return os<<'}';}
template<class T>std::ostream&operator<<(std::ostream&os,const std::multiset<T>&s){os<<'{';int i=0;for(const auto&x:s)os<<(i++?", ":"")<<x;return os<<'}';}
template<class T>std::ostream&operator<<(std::ostream&os,const std::unordered_set<T>&s){os<<'{';int i=0;for(const auto&x:s)os<<(i++?", ":"")<<x;return os<<'}';}
// map, unordered_map → {k: v, ...}
template<class K,class V>std::ostream&operator<<(std::ostream&os,const std::map<K,V>&m){os<<'{';int i=0;for(const auto&[k,v]:m)os<<(i++?", ":"")<<k<<": "<<v;return os<<'}';}
template<class K,class V>std::ostream&operator<<(std::ostream&os,const std::unordered_map<K,V>&m){os<<'{';int i=0;for(const auto&[k,v]:m)os<<(i++?", ":"")<<k<<": "<<v;return os<<'}';}
// iterable fallback → [...] (上記で個別定義されていない iterable 型用)
template<class T,class=std::void_t<decltype(std::begin(std::declval<T&>()))>,std::enable_if_t<!std::is_same_v<T,std::string>&&!std::is_same_v<std::decay_t<T>,char*>&&!std::is_array_v<T>,int> =0>
std::ostream&operator<<(std::ostream&os,const T&c){os<<'[';int i=0;for(const auto&x:c)os<<(i++?", ":"")<<x;return os<<']';}

// --- debug macros ---

namespace dbg {
const std::string RESET="\033[0m",RED="\033[1;31m",CYAN="\033[1;36m",ITALIC="\033[3m",BOLD="\033[1m",FAINT="\033[0;2m";
template<class T,class...Args>void print_vars(const std::string&s,const T&a,const Args&...x){std::cerr<<CYAN<<s<<RESET<<" = "<<a;if constexpr(sizeof...(Args)>0)(std::cerr<<...<<(std::cerr<<", ",x));}
}

#define DBG_LOC dbg::FAINT<<" in "<<dbg::BOLD<<__func__<<dbg::FAINT<<dbg::ITALIC<<" (L"<<__LINE__<<") "<<__FILE__<<dbg::RESET
#define checkpoint() std::cerr<<dbg::RED<<"< checkpoint >"<<DBG_LOC<<'\n'
#define debug(...) do{dbg::print_vars(#__VA_ARGS__,__VA_ARGS__);std::cerr<<DBG_LOC<<'\n';}while(0)
#define debugArray(x,n) do{std::cerr<<dbg::CYAN<<#x<<dbg::RESET<<" = ["<<x[0];for(int i=1;i<(int)(n);++i)std::cerr<<", "<<x[i];std::cerr<<"]"<<DBG_LOC<<'\n';}while(0)
#define debugMatrix(x,h,w) do{std::cerr<<dbg::CYAN<<#x<<"\n"<<dbg::RESET<<"= ";for(int i=0;i<(int)(h);++i){std::cerr<<(i?"   [":"[[");for(int j=0;j<(int)(w);++j)std::cerr<<(j?", ":"")<<x[i][j];std::cerr<<"]"<<(i+1==(int)(h)?"]":",\n");}std::cerr<<DBG_LOC<<'\n';}while(0)

#endif
// clang-format on
