#ifndef GRAPH__hpp
#define GRAPH__hpp

#include <memory>

#include <set>
#include <map>
#include <vector>

#include <cassert>

#include "adj_list.hpp"
#include "CNF_formula.hpp"

class CNF_formula;

class graph{
    // OPTIMIZATION NOTES:
    //  - Consider the following possibility: To create internal classes and encapsulate methods:
    //      - A "graph painter", that performs all the operations for colouring the graph
    //      - A "clique finder", that performs all the operations related with cliques
    public:
        class node;

    private:
        typedef std::size_t colour;

        class graph_core;

        class colour_removal_controller{
            private:
                std::map<const node*, std::vector<std::size_t>> how_many_times_each_vertex_was_removed_until_now;

                inline void remove_colour(const node &n, colour c) { how_many_times_each_vertex_was_removed_until_now[&n][c-1]++; }
                inline void restore_colour(const node &n, colour c) { how_many_times_each_vertex_was_removed_until_now[&n][c-1]--; }

            public:
                inline colour_removal_controller(const std::set<const node*> &nodes_){ for(auto i: nodes_) how_many_times_each_vertex_was_removed_until_now[i] = std::vector<std::size_t>(nodes_.size(),0); }

                inline void remove_colour_from_its_current_adjacents(const graph &G, const node &n, colour c) { for(auto i: n) if(G.contains(*i)) remove_colour(*i,c); }
                inline void restore_colour_on_its_current_adjacents(const graph &G, const node &n, colour c) { for(auto i: n) if(G.contains(*i)) restore_colour(*i,c); }

                inline bool some_vertex_is_not_coloureable_under_bound(colour bound){
                    bool res=false;
                    for(auto i=how_many_times_each_vertex_was_removed_until_now.begin(); i!=how_many_times_each_vertex_was_removed_until_now.end() && !res; ++i){
                        res=true;
                        for(colour c=1; c<bound && res; ++c) res = !is_available(*(i->first),c);
                    }
                    return res;
                }
                inline bool is_available(const node &n, colour c) const { return how_many_times_each_vertex_was_removed_until_now.at(&n)[c-1]==0; }
        };
        class graph_core: private std::vector<node>{
            private:
                mutable std::size_t chromatic_number_;
                mutable std::map<const graph::node*, colour> colours;
                mutable bool coloured;
                
                inline bool colour_is_valid(const graph::node &n, colour c) const{
                    for(const graph::node* adj : n)  if(colours[adj]==c) return false;
                    return true;
                }

                bool set_colours(std::list<const node*>::iterator current, std::list<const node*>::iterator end) const;
                void set_colours() const;

                inline bool correct_number_of_colours() const {
                    bool res = true;
                    for(auto i=colours.begin(); i!=colours.end() && res; ++i) res = 0<colours.at(i->first) && colours.at(i->first)<=chromatic_number_;
                    return res;
                }
                inline bool colouring_is_correct() const {
                    bool res = true;
                    for(auto i=colours.begin(); i!=colours.end() && res; ++i)
                        for(auto j=colours.begin(); j!=colours.end() && res; ++j)
                            res = !(i->first->is_adjacent_to(*(j->first))) || i->second!=j->second;
                        
                    return res;
                }

            public:
                inline graph_core(const adj_list &l): std::vector<graph::node>(l.n_nodes()), coloured(false){
                    for(auto i: l) at(i.first-1).make_adj(at(i.second-1));
                }

                inline colour colour_of(const node &n) const { return colours.at(&n); }
                inline std::size_t chromatic_number() const { return chromatic_number_; }

                inline std::size_t get_id(const node &n) const { return &*begin()-&n+1; }

                friend class graph;
        };
        
        const graph_core* structure_;
        std::set<const graph::node*> nodes_;
        bool is_root;

        graph(const graph &other): structure_(other.structure_), nodes_(other.nodes_), is_root(false) {}

        inline graph(const graph_core &G): structure_(&G), is_root(false) { for(auto i=G.begin(); i!=G.end(); ++i) nodes_.insert(&*i); }

        inline graph substract_node(const graph::node& n) const { graph G=*this; G.erase_node(n); return G; }
        inline void erase_node(const graph::node &n) { nodes_.erase(find(n)); }
        inline void restore_node(const graph::node &n) { nodes_.insert(&n); }
        
        inline std::set<const graph::node*>::iterator select_node() const{
            assert(!empty());

            std::set<const graph::node*>::iterator res = nodes_.begin();
            for(std::set<const graph::node*>::iterator i=nodes_.begin(); i!=nodes_.end(); ++i) if(degree(**res) < degree(**i)) res=i;
            return res;
        }
        
        std::size_t Yangzhao(std::size_t current_colour, std::size_t upper_bound, colour_removal_controller &table);

        std::size_t degree(const node &n) const { assert(contains(n)); std::size_t res=0; for(auto i: *this) if(are_adjacent(n,*i)) ++res; return res; }

        inline std::set<const node*> &colours_table() { return nodes_; }

        inline const graph_core &structure() const { return *structure_; }

        void erase_nodes_with_degree_under_bound(std::size_t bound);
        inline void erase_non_adjacents_nodes(const node &n);

        class CLIQUE_to_CNF_SAT_translator{
            private:
                std::map<std::pair<const graph::node*, std::size_t>, std::size_t> variables_table;

                void insert_P1_conditions(const graph &G, std::size_t i, CNF_formula &target) const;
                void insert_P2_conditions(const graph &G, std::size_t i, CNF_formula &target) const;
                void insert_P3_conditions(const graph &G, std::size_t i, std::size_t j, CNF_formula &target) const;

                void initialize_variables(const graph &G, std::size_t k_number, CNF_formula &target);
                void initialize_clauses(const graph &G, std::size_t k_number, CNF_formula &target) const;

                inline std::size_t variable_id(const graph::node *n, std::size_t i) const { return variables_table.at(std::make_pair(n,i)); }

                CNF_formula build_trivially_satisfiable_formula() const;

            public:
                CNF_formula build_from(const graph &G, std::size_t k_number);
        };
    
    public:
        class node: public std::set<node*>{
            private:
                inline void make_adj(node &n) { insert(&n); n.insert(this); }
                inline void remove_adj() { for(auto i: *this) if(i!=this) i->erase(this); }

            public:
                inline node() = default;
                inline node(const node &) = delete;

                inline ~node() { remove_adj(); }

                inline bool is_adjacent_to(const node &adj) const { return find(const_cast<node *>(&adj))!=end(); }

                inline std::size_t n_adj() const { return size(); }

                friend class graph_core;
        };

        inline graph(const std::string &s): graph(adj_list(s)) {}
        graph(const adj_list &);

        inline ~graph() { if(is_root) delete structure_; }

        const std::set<const node *> &nodes() const { return nodes_; }

        inline std::size_t size() const { return nodes_.size(); }
        inline bool empty() const { return nodes_.empty(); }
        inline bool contains(const node &n) const { return find(n)!=end(); }
        inline bool are_adjacent(const node &u, const node &v) const { assert(contains(u)); assert(contains(v)); return u.is_adjacent_to(v); }

        inline std::set<const graph::node*>::iterator find(const node &n) { return nodes_.find(&n); }
        inline std::set<const graph::node*>::iterator begin() { return nodes_.begin(); }
        inline std::set<const graph::node*>::iterator end() { return nodes_.end(); }
        inline std::set<const graph::node*>::iterator erase(std::set<const graph::node*>::iterator it) { return nodes_.erase(it); }

        inline std::set<const graph::node*>::const_iterator find(const node &n) const { return nodes_.find(&n); }
        inline std::set<const graph::node*>::const_iterator begin() const { return nodes_.begin(); }
        inline std::set<const graph::node*>::const_iterator end() const { return nodes_.end(); }

        inline void set_chromatic_colour_and_colouring(){
            colour_removal_controller colours_removal_table(nodes_);

            structure().chromatic_number_ = Yangzhao(0, size()+1, colours_removal_table);
            structure().set_colours();

            assert(structure().correct_number_of_colours());
            assert(structure().colouring_is_correct());
        }

        inline colour colour_of(const node &n) const { return structure().colour_of(n); }
        inline std::size_t chromatic_number() const { return structure().chromatic_number(); }

        bool contains_a_clique(std::size_t degree) const;
        graph get_a_clique(std::size_t degree) const;

        std::set<std::size_t>get_nodes_ids() const { std::set<std::size_t> res; for(auto i=structure().begin(); i!=structure().end(); ++i) if(contains(*i)) res.insert(structure().get_id(*i)); return res; }

        friend CNF_formula build_CNF_formula_from_graph(const graph &G, std::size_t k_number);
};

#endif
