#include "CNF_formula.hpp"

using namespace std;

std::size_t CNF_formula::n_apariciones(variable v) const{
    std::size_t res=0;
    for(const auto &i: clauses) if(i.contains(v)) ++res;
    return res;
}

std::size_t CNF_formula::select_variable() const{
    std::size_t max=0;
    variable res;

    for(auto i: variables){
        std::size_t count = n_apariciones(i);
        if(count>max){
            max=count;
            res=i;
        }
    }

    return res;
}

bool CNF_formula::is_satisfiable_on_value(variable v, bool value) const{
    assert_invariant();

    CNF_formula copy=*this;
    copy.assert_invariant();
    
    for(auto i=copy.clauses.begin(); i!=copy.clauses.end(); ){
        if(i->contains(v)){
            if(i->value(v)==value) i=copy.clauses.erase(i);
            else{
                i->erase(v);
                if(i->empty()) return false;
                ++i;
            }
        }
        else ++i;
    }

    copy.variables.erase(v);

    if(copy.clauses.empty()) return true;
    else{
        variable most_frequent_one = copy.select_variable();
        return copy.is_satisfiable_on_value(most_frequent_one, true) || copy.is_satisfiable_on_value(most_frequent_one, false);
    }
}
bool CNF_formula::is_satisfiable() const{
    assert_invariant();
    
    variable most_frequent_one = select_variable();
    return is_satisfiable_on_value(most_frequent_one, true) || is_satisfiable_on_value(most_frequent_one, false);
}

void CNF_formula::print(const std::string &path){
    ofstream os(path);

    os << "p cnf " << variables.size() << " " << clauses.size() << endl;

    for(const auto &i: clauses){
        for(const auto &j: i) os << (i.is_in_negative(j.first) ? "-" : "" ) << j.first << " ";
        os << "0" << endl;
    }
}
