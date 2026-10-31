#include "contract.hpp"
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
namespace witness {
namespace {void widthcheck(int w){if(w<1||w>8)throw std::invalid_argument("counter width must be 1..8");}}
std::string name(Variant v){switch(v){case Variant::saturating:return "saturating";case Variant::wrapping:return "wrapping";case Variant::inverted_reset:return "inverted_reset";case Variant::ignored_enable:return "ignored_enable";}throw std::invalid_argument("unknown variant");}
unsigned next(unsigned state,Input i,int w,Variant v){widthcheck(w);unsigned mask=(1u<<w)-1;if(state>mask)throw std::invalid_argument("state exceeds width");if(v==Variant::inverted_reset? !i.reset:i.reset)return 0;if(!i.enable&&v!=Variant::ignored_enable)return state;if(v==Variant::wrapping)return (state+1)&mask;return state==mask?mask:state+1;}
Result search(int width,int bound,Variant variant){widthcheck(width);if(bound<0||bound>32)throw std::invalid_argument("bound must be 0..32");(void)name(variant);Result r{width,bound,variant,false,0,{}};
 struct State {unsigned a,b;std::vector<Step> trace;};std::queue<State> q;q.push({0,0,{}});std::set<std::pair<unsigned,unsigned>> seen{{0,0}};
 while(!q.empty()){auto s=q.front();q.pop();if(int(s.trace.size())>=bound)continue;
  for(Input i:std::vector<Input>{{false,false},{false,true},{true,false},{true,true}}){++r.explored;unsigned a=next(s.a,i,width,Variant::saturating),b=next(s.b,i,width,variant);auto trace=s.trace;trace.push_back({i,a,b});if(a!=b){r.found=true;r.trace=std::move(trace);return r;}if(seen.insert({a,b}).second)q.push({a,b,std::move(trace)});}
 }return r;
}
bool replay(const Result&r){if(!r.found||r.trace.empty()||int(r.trace.size())>r.bound)return false;unsigned a=0,b=0;for(unsigned j=0;j<r.trace.size();++j){const auto&s=r.trace[j];a=next(a,s.input,r.width,Variant::saturating);b=next(b,s.input,r.width,r.variant);if(a!=s.reference||b!=s.candidate)return false;if(j+1<r.trace.size()&&a!=b)return false;}return a!=b;}
unsigned extend(unsigned input,int iw,int ow,bool sign){if(iw<1||iw>8||ow<iw||ow>16)throw std::invalid_argument("extension widths must satisfy 1<=input<=8 and input<=output<=16");unsigned im=(1u<<iw)-1,om=(1u<<ow)-1;if(input>im)throw std::invalid_argument("input exceeds width");return sign&&(input&(1u<<(iw-1))) ? input|(om^im):input;}
std::string Result::json() const {std::ostringstream s;s<<"{\"model\":\"bounded C++ behavioral contract; not RTL simulation\",\"reference\":\"saturating\",\"candidate\":\""<<name(variant)<<"\",\"width\":"<<width<<",\"bound_cycles\":"<<bound<<",\"transitions_examined\":"<<explored<<",\"status\":\""<<(found?"counterexample":"no_witness_within_bound")<<"\",\"witness_cycles\":"<<trace.size()<<",\"trace\":[";bool comma=false;for(auto&t:trace){if(comma)s<<',';comma=true;s<<"{\"reset\":"<<(t.input.reset?"true":"false")<<",\"enable\":"<<(t.input.enable?"true":"false")<<",\"reference\":"<<t.reference<<",\"candidate\":"<<t.candidate<<'}';}s<<"]}";return s.str();}
}
