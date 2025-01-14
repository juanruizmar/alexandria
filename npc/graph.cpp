#include "graph.hpp"
#include "CNF_formula.hpp"

#include <fstream>

using namespace std;

void graph::graph_core::set_colours() const{
    std::list<const node*> sorted_nodes_;

    for(auto i=begin(); i!=end(); ++i){
        colours[&*i] = 0;
        sorted_nodes_.push_back(&*i);
    }

    sorted_nodes_.sort( [](const node *u, const node *v){return u->size() > v->size();} );

    set_colours(sorted_nodes_.begin(), sorted_nodes_.end());

    coloured = true;
}

bool graph::graph_core::set_colours(std::list<const node*>::iterator current, std::list<const node*>::iterator end) const{
    if(current!=end){
        for(colour c=1; c<=chromatic_number_; ++c) if(colour_is_valid(**current, c)){
            colours[*current] = c;

            if(set_colours(++current, end)) return true;
            else colours[*--current] = 0;
        }
        return false;
    }
    else return true;
}

bool graph::contains_a_clique(size_t degree) const{
    return build_CNF_formula_from_graph(*this, degree).is_satisfiable();
}

void graph::erase_nodes_with_degree_under_bound(std::size_t bound){
    bool there_are_probably_more_nodes_to_erase=true;
    while(there_are_probably_more_nodes_to_erase){
        there_are_probably_more_nodes_to_erase=false;

        for(auto i=begin(); i!=end(); ){
            if(degree(**i)<bound){
                there_are_probably_more_nodes_to_erase=true;
                i=erase(i);
            }
            else ++i;
        }
    }
}
void graph::erase_non_adjacents_nodes(const node &n){
    for(auto i=begin(); i!=end(); ){
        if(!(*i)->is_adjacent_to(n)) i=erase(i);
        else ++i;
    }
}

graph graph::get_a_clique(size_t degree) const{
    graph res(*this);

    std::set<const node*> selected_nodes_for_clique;
    std::set<const node*>::node_type stl_node_wrapper;

    res.erase_nodes_with_degree_under_bound(degree-1);
    
    while(!res.empty()){
        stl_node_wrapper = res.nodes_.extract(res.select_node());

        if(!res.contains_a_clique(degree)){
            selected_nodes_for_clique.insert(stl_node_wrapper.value());
            res.erase_non_adjacents_nodes(*stl_node_wrapper.value());
            
            --degree;
        }
    }

    res.nodes_ = move(selected_nodes_for_clique);

    return res;
}

graph::graph(const adj_list &l): structure_(new graph_core(l)), is_root(true){
    assert(structure().size()==l.n_nodes());
    for(auto i=structure().begin(); i!=structure().end(); ++i) nodes_.insert(&*i);
    assert(structure().size()==size());
}

std::size_t graph::Yangzhao(std::size_t last_colour, std::size_t upper_bound, colour_removal_controller &table){
    if(empty()) upper_bound=last_colour;
    else if(!table.some_vertex_is_not_coloureable_under_bound(upper_bound)){
        std::set<const node *>::iterator current_node_it = select_node();

        for(colour current_colour=1; current_colour<upper_bound; ++current_colour) if(table.is_available(**current_node_it, current_colour)){
            table.remove_colour_from_its_current_adjacents(*this, **current_node_it, current_colour);
            upper_bound = substract_node(**current_node_it).Yangzhao(std::max(last_colour, current_colour), upper_bound, table);
            table.restore_colour_on_its_current_adjacents(*this, **current_node_it, current_colour);
        }
    }

    return upper_bound;
}

void graph::CLIQUE_to_CNF_SAT_translator::initialize_variables(const graph &G, std::size_t k_number, CNF_formula &target){
    for(std::size_t i=1; i<=k_number; ++i) for(const auto &n: G.nodes()){
        target.variables.insert(1+target.variables.size());
        variables_table[make_pair(n,i)]=target.variables.size();
    }
}
void graph::CLIQUE_to_CNF_SAT_translator::initialize_clauses(const graph &G, std::size_t k_number, CNF_formula &target) const{
    for(std::size_t i=1; i<=k_number; ++i) insert_P1_conditions(G, i, target);
    for(std::size_t i=1; i<=k_number; ++i) insert_P2_conditions(G, i, target);
    for(std::size_t i=1; i<=k_number; ++i) for(std::size_t j=1; j<i; ++j) insert_P3_conditions(G, i, j, target);
}

void graph::CLIQUE_to_CNF_SAT_translator::insert_P1_conditions(const graph &G, std::size_t i, CNF_formula &target) const{
    target.clauses.push_back(CNF_formula::clause());
    for(auto r=G.nodes().begin(); r!=G.nodes().end(); ++r){
        target.clauses.back().insert_in_positive(variable_id(*r,i));
    }
}
void graph::CLIQUE_to_CNF_SAT_translator::insert_P2_conditions(const graph &G, std::size_t i, CNF_formula &target) const{
    for(auto r=G.nodes().begin(); r!=G.nodes().end(); ++r){
        for(auto s=G.nodes().begin(); s!=r; ++s){
            target.clauses.push_back(CNF_formula::clause());
            target.clauses.back().insert_in_negative(variable_id(*r,i));
            target.clauses.back().insert_in_negative(variable_id(*s,i));
        }
    }
}
void graph::CLIQUE_to_CNF_SAT_translator::insert_P3_conditions(const graph &G, std::size_t i, std::size_t j, CNF_formula &target) const{
    for(auto r=G.nodes().begin(); r!=G.nodes().end(); ++r){
        for(auto s=G.nodes().begin(); s!=G.nodes().end(); ++s){
            if(!G.are_adjacent(**r,**s)){
                target.clauses.push_back(CNF_formula::clause());
                target.clauses.back().insert_in_negative(variable_id(*r,i));
                target.clauses.back().insert_in_negative(variable_id(*s,j));
            }
        }
    }
}

CNF_formula graph::CLIQUE_to_CNF_SAT_translator::build_trivially_satisfiable_formula() const{
    CNF_formula res; 
    res.variables.insert(1); 
    res.clauses.push_back(CNF_formula::clause());
    res.clauses.back()[1]=true;
    return res;
}
CNF_formula graph::CLIQUE_to_CNF_SAT_translator::build_from(const graph &G, std::size_t k_number){
    CNF_formula res;

    if(k_number==0) res = build_trivially_satisfiable_formula();
    else{
        initialize_variables(G, k_number, res);
        initialize_clauses(G, k_number, res);
    }

    res.assert_invariant();

    return res;
}

CNF_formula build_CNF_formula_from_graph(const graph &G, std::size_t k_number) { return graph::CLIQUE_to_CNF_SAT_translator().build_from(G, k_number); }
