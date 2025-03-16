#ifndef NAMED_250218
#define NAMED_250218

#include <string>
#include <fstream>
#include <memory>

#include <map>

#include "rational.hpp"

class name_and_symbol{    // Easy to reuse. Something with a name and a symbol, useful for physics magnitudes or measuring units
    private:
        class opperation_defined_name_and_symbol;
        class user_defined_name_and_symbol;

        class name_and_symbol_wrapee_interface{
            private:
                virtual std::map<user_defined_name_and_symbol, rational> as_raw_map() const = 0;
        
            public:
                virtual std::string name() const = 0;
                virtual std::string symbol() const = 0;
        
                virtual opperation_defined_name_and_symbol square() const = 0;
                virtual opperation_defined_name_and_symbol sq_root() const = 0;
                virtual opperation_defined_name_and_symbol cube() const = 0;
                virtual opperation_defined_name_and_symbol inverse() const = 0;

                virtual opperation_defined_name_and_symbol pow(const rational&) const = 0;
        
                virtual opperation_defined_name_and_symbol operator *(const name_and_symbol_wrapee_interface &other) const;
                virtual opperation_defined_name_and_symbol operator /(const name_and_symbol_wrapee_interface &other) const;
        
                inline friend std::ostream &operator <<(std::ostream &os, const name_and_symbol_wrapee_interface &ns) { return os << ns.name() << ", " << ns.symbol(); }
        };
        class user_defined_name_and_symbol: public name_and_symbol_wrapee_interface{
            private:
                std::string name_;
                std::string symbol_;
        
                inline std::map<user_defined_name_and_symbol, rational> as_raw_map() const { return {std::pair(*this, 1)}; }

            public:
                inline user_defined_name_and_symbol(const std::string &name, const std::string &symbol): name_(name), symbol_(symbol) {};
        
                inline user_defined_name_and_symbol(const user_defined_name_and_symbol &other) = default;
        
                inline std::string name() const { return name_; }
                inline std::string symbol() const { return symbol_; }
        
                opperation_defined_name_and_symbol square() const;
                opperation_defined_name_and_symbol sq_root() const;
                opperation_defined_name_and_symbol cube() const;
                opperation_defined_name_and_symbol inverse() const;

                opperation_defined_name_and_symbol pow(const rational&) const;
        
                inline bool operator ==(const user_defined_name_and_symbol &other) const { return name_==other.name_ && symbol_==other.symbol_; }
                inline bool operator <(const user_defined_name_and_symbol &other) const { return name_==other.name_ ? name_<other.name_ : symbol_<other.symbol_; }
        };
        class opperation_defined_name_and_symbol: public name_and_symbol_wrapee_interface{
            private:
                std::map<user_defined_name_and_symbol, rational> map;
        
                inline rational get_exp(const user_defined_name_and_symbol &key) const { return map.contains(key) ? map.at(key) : 0; }
                inline std::map<user_defined_name_and_symbol, rational> as_raw_map() const { return map; }
        
            public:
                opperation_defined_name_and_symbol(std::map<user_defined_name_and_symbol, rational> &&m): map(m) { assert(map.size()==1); }
                opperation_defined_name_and_symbol(const std::map<user_defined_name_and_symbol, rational> &m1, const std::map<user_defined_name_and_symbol, rational> &m2);

                std::string name() const;
                std::string symbol() const;
        
                opperation_defined_name_and_symbol square() const;
                opperation_defined_name_and_symbol sq_root() const;
                opperation_defined_name_and_symbol cube() const;
                opperation_defined_name_and_symbol inverse() const;

                opperation_defined_name_and_symbol pow(const rational&) const;
        
                friend class name_and_symbol_wrapee_interface;
                friend class user_defined_name_and_symbol;
                friend class name_and_symbol;
        };
        
        const std::shared_ptr<name_and_symbol_wrapee_interface> ns;

        inline name_and_symbol(const opperation_defined_name_and_symbol &_ns): ns(new opperation_defined_name_and_symbol(_ns)) {}
        inline const name_and_symbol_wrapee_interface &payload() const { return *ns; }

    public:
        inline name_and_symbol(const name_and_symbol &other): ns(other.ns) {}

        inline name_and_symbol(const std::string &name, const std::string &symbol): ns(new user_defined_name_and_symbol(name, symbol)) {}

        inline std::string name() const { return ns->name(); }
        inline std::string symbol() const { return ns->symbol(); }

        inline friend std::ostream &operator <<(std::ostream &os, const name_and_symbol &n) { return os << n.ns->name() << ", " << n.ns->symbol(); }

};

#endif
