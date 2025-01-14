#ifndef __CNF_FORMULA___hpp__
#define __CNF_FORMULA___hpp__

#include <fstream>

#include <set>
#include <map>
#include <list>
#include <vector>

#include <cassert>

class graph;

class CNF_formula{
    // INVARIANT: 
    // - There is at least one variable
    // - Every variable appears as a key in at least one clause
    // - Every variable that appears in the clauses is in the set of variables
    // - Every clause has at least one variable

    // OPTIMIZATION OPTIONS (flyweight design):
    //  - The sets of variables and clauses are shared for every instance.
    //  - Only another set of variables and a set of pointers to clauses are inside each implementation.
    //  - Observer methods are responsible of giving the following info: 
    //      - Is this variable in this specific formula?
    //      - How many variables are there in this clause?
    //      - How many clauses are still in this formula?
    //  - Before stablishing this new design this definitive version, it is necessary to create a tests batery and measure execution times.

    private:
        typedef std::size_t variable;
        class clause: private std::map<const variable, bool>{
            public:
                inline void insert_in_positive(const variable v) { emplace(v, true); }
                inline void insert_in_negative(const variable v) { emplace(v, false); }

                inline bool value(const variable v) const { assert(contains(v)); return at(v); }
                inline bool is_in_positive(const variable v) const { assert(contains(v)); return at(v); }
                inline bool is_in_negative(const variable v) const { assert(contains(v)); return !is_in_positive(v); }

                friend class CNF_formula;
                friend class graph;
        };

        std::list<clause> clauses;
        std::set<variable> variables;

        void assert_invariant() const{
            assert(!variables.empty());         // - There is at least one variable
            for(const auto &i: variables){
                bool res=false;
                for(auto j=clauses.begin(); j!=clauses.end() && !res; ++j) res=j->contains(i);
                assert(res);                    // - Every variable appears as a key in at least one clause
            }
            for(const auto &i: clauses){
                assert(!i.empty());             // - Every clause has at least one variable
                bool res=false;
                for(auto j=i.begin(); j!=i.end() && !res; ++j) res=variables.contains(j->first);
                assert(res);                    // - Every variable that appears in the clauses is in the set of variables
            }
        }

        inline CNF_formula() = default;

        friend class graph;

    public:
        inline size_t n_variables() const { return variables.size(); }
        inline size_t n_clauses() const { return clauses.size(); }

        void print(const std::string &path);

        bool is_satisfiable_on_value(variable v, bool value) const;

        bool is_satisfiable() const;

        std::size_t n_apariciones(variable v) const;

        std::size_t select_variable() const;

        friend CNF_formula build_CNF_formula_from_graph(const graph &G, std::size_t k_number);
};

#endif
