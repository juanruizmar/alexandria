#include "name_and_symbol.hpp"

using namespace std;

name_and_symbol::opperation_defined_name_and_symbol name_and_symbol::user_defined_name_and_symbol::square() const{
    return map<user_defined_name_and_symbol, rational>{std::pair(*this, 2)};
}
name_and_symbol::opperation_defined_name_and_symbol name_and_symbol::user_defined_name_and_symbol::sq_root() const{
    return map<user_defined_name_and_symbol, rational>{std::pair(*this, rational(1,2))};
}
name_and_symbol::opperation_defined_name_and_symbol name_and_symbol::user_defined_name_and_symbol::cube() const{
    return map<user_defined_name_and_symbol, rational>{std::pair(*this, 3)};
}
name_and_symbol::opperation_defined_name_and_symbol name_and_symbol::user_defined_name_and_symbol::inverse() const{
    return map<user_defined_name_and_symbol, rational>{std::pair(*this, -1)};
}

name_and_symbol::opperation_defined_name_and_symbol name_and_symbol::opperation_defined_name_and_symbol::square() const{
    opperation_defined_name_and_symbol res(*this);
    for(auto &i: res.map) i.second*=2;
    return res;
}
name_and_symbol::opperation_defined_name_and_symbol name_and_symbol::opperation_defined_name_and_symbol::sq_root() const{
    opperation_defined_name_and_symbol res(*this);
    for(auto &i: res.map) i.second/=2;
    return res;
}
name_and_symbol::opperation_defined_name_and_symbol name_and_symbol::opperation_defined_name_and_symbol::cube() const{
    opperation_defined_name_and_symbol res(*this);
    for(auto &i: res.map) i.second*=3;
    return res;
}
name_and_symbol::opperation_defined_name_and_symbol name_and_symbol::opperation_defined_name_and_symbol::inverse() const{
    opperation_defined_name_and_symbol res(*this);
    for(auto &i: res.map) i.second*=-1;
    return res;
}

name_and_symbol::opperation_defined_name_and_symbol name_and_symbol::name_and_symbol_wrapee_interface::operator *(const name_and_symbol_wrapee_interface &other) const{
    return opperation_defined_name_and_symbol(as_raw_map(), other.as_raw_map());
}
name_and_symbol::opperation_defined_name_and_symbol name_and_symbol::name_and_symbol_wrapee_interface::operator /(const name_and_symbol_wrapee_interface &other) const{
    return opperation_defined_name_and_symbol(as_raw_map(), other.inverse().as_raw_map());
}
name_and_symbol::opperation_defined_name_and_symbol::opperation_defined_name_and_symbol( const std::map<user_defined_name_and_symbol, rational> &m1,  const std::map<user_defined_name_and_symbol, rational> &m2){
    for(const auto &i: m1) map.emplace(i.first, i.second + (m2.contains(i.first) ? m2.at(i.first) : 0) );
    for(const auto &i: m2) if(!m1.contains(i.first)) map.emplace(i.first, i.second + m1.at(i.first));
}

string name_and_symbol::opperation_defined_name_and_symbol::name() const{
    string res;
    for(auto i=map.begin();;){
        res+=i->first.name();
        if(i->second!=1) res+="^(" + to_string(i->second) + ")";
        ++i;
        if(i==map.end()) break;
        res+="*";
    }
    return res;
}
string name_and_symbol::opperation_defined_name_and_symbol::symbol() const{
    string res;
    for(auto i=map.begin();;){
        res+=i->first.symbol();
        if(i->second!=1) res+="^(" + to_string(i->second) + ")";
        ++i;
        if(i==map.end()) break;
        res+="*";
    }
    return res;
}