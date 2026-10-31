#include "contract.hpp"
#include <iostream>
#include <stdexcept>
using namespace witness;
int count=0;void check(bool b){++count;if(!b)throw std::runtime_error("assertion "+std::to_string(count));}
template<class F>void rejects(F f){bool ok=false;try{f();}catch(const std::invalid_argument&){ok=true;}check(ok);}
int main(){try{
 for(int w=1;w<=5;++w){auto wrap=search(w,32,Variant::wrapping);check(wrap.found);check(wrap.trace.size()==(1u<<w));check(replay(wrap));auto shorter=search(w,(1<<w)-1,Variant::wrapping);check(!shorter.found);auto equal=search(w,32,Variant::saturating);check(!equal.found&&equal.trace.empty());}
 for(auto v:{Variant::inverted_reset,Variant::ignored_enable}){auto r=search(3,10,v);check(r.found&&r.trace.size()==1&&replay(r));r.trace[0].reference=99;check(!replay(r));}
 check(!search(3,0,Variant::wrapping).found);check(next(7,{false,true},3,Variant::saturating)==7);check(next(7,{false,true},3,Variant::wrapping)==0);check(next(5,{true,true},3,Variant::saturating)==0);check(next(5,{false,false},3,Variant::saturating)==5);
 for(int iw=1;iw<=8;++iw)for(int ow=iw;ow<=16;++ow)for(unsigned x=0;x<(1u<<iw);++x){auto a=extend(x,iw,ow,true),z=extend(x,iw,ow,false);check(z==x);check((a&((1u<<iw)-1))==x);check(a<(1u<<ow));if(!(x&(1u<<(iw-1)))||iw==ow)check(a==z);else check(a!=z);}
 check(extend(4,3,6,true)==60);rejects([]{search(0,10,Variant::saturating);});rejects([]{search(9,10,Variant::saturating);});rejects([]{search(3,33,Variant::wrapping);});rejects([]{search(3,-1,Variant::wrapping);});rejects([]{next(8,{false,false},3,Variant::saturating);});rejects([]{extend(8,3,6,true);});rejects([]{extend(0,4,3,true);});rejects([]{extend(0,3,17,true);});
 std::cout<<count<<" assertions passed\n";
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
