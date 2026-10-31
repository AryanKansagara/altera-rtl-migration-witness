#pragma once
#include <string>
#include <vector>
namespace witness {
enum class Variant { saturating, wrapping, inverted_reset, ignored_enable };
struct Input {bool reset,enable;};
struct Step {Input input;unsigned reference,candidate;};
struct Result {int width,bound;Variant variant;bool found=false;unsigned explored=0;std::vector<Step> trace;std::string json() const;};
unsigned next(unsigned state,Input,int width,Variant);
Result search(int width,int bound,Variant);
bool replay(const Result&);
unsigned extend(unsigned input,int in_width,int out_width,bool sign);
std::string name(Variant);
}
