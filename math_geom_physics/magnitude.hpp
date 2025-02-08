#ifndef MAGNITUDE_250206
#define MAGNITUDE_250206

#include <iostream>

#include <algorithm>
#include <memory>

#include <string>

#include <valarray>
#include <array>

#include <fstream>
#include <map>
#include <set>

#include <cassert>

class magnitude;
class measure_unit;
class magnitudes_base;
class measure_units_system;

class opperation_defined_name_and_symbol;
class user_defined_name_and_symbol;
class named;

class name_and_symbol{
    private:
        virtual std::map<user_defined_name_and_symbol, float> as_raw_map() const = 0;

    public:
        virtual std::string name() const = 0;
        virtual std::string symbol() const = 0;

        virtual opperation_defined_name_and_symbol square() const = 0;
        virtual opperation_defined_name_and_symbol cube() const = 0;
        virtual opperation_defined_name_and_symbol inverse() const = 0;

        virtual opperation_defined_name_and_symbol operator *(const name_and_symbol &other) const;
        virtual opperation_defined_name_and_symbol operator /(const name_and_symbol &other) const;

        inline friend std::ostream &operator <<(std::ostream &os, const name_and_symbol &ns) { return os << ns.name() << ", " << ns.symbol(); }
};
class user_defined_name_and_symbol: public name_and_symbol{
    private:
        std::string name_;
        std::string symbol_;

        inline std::map<user_defined_name_and_symbol, float> as_raw_map() const { return {std::pair(*this, 1)}; }
    
    public:
        inline user_defined_name_and_symbol(const std::string &name, const std::string &symbol): name_(name), symbol_(symbol) {};

        inline user_defined_name_and_symbol(const user_defined_name_and_symbol &other) = default;

        inline std::string name() const { return name_; }
        inline std::string symbol() const { return symbol_; }

        opperation_defined_name_and_symbol square() const;
        opperation_defined_name_and_symbol cube() const;
        opperation_defined_name_and_symbol inverse() const;

        inline bool operator ==(const user_defined_name_and_symbol &other) const { return name_==other.name_ && symbol_==other.symbol_; }
        inline bool operator <(const user_defined_name_and_symbol &other) const { return name_==other.name_ ? name_<other.name_ : symbol_<other.symbol_; }
};
class opperation_defined_name_and_symbol: public name_and_symbol{
    private:
        std::map<user_defined_name_and_symbol, float> map;

        inline float get_exp(const user_defined_name_and_symbol &key) const { return map.contains(key) ? map.at(key) : 0 ; }
        inline std::map<user_defined_name_and_symbol, float> as_raw_map() const { return map; }

    public:
        opperation_defined_name_and_symbol(std::map<user_defined_name_and_symbol, float> &&m): map(m) { assert(map.size()==1); }
        opperation_defined_name_and_symbol(const std::map<user_defined_name_and_symbol, float> &m1, const std::map<user_defined_name_and_symbol, float> &m2);
        
        std::string name() const;
        std::string symbol() const;

        opperation_defined_name_and_symbol square() const;
        opperation_defined_name_and_symbol cube() const;
        opperation_defined_name_and_symbol inverse() const;

        friend class name_and_symbol;
        friend class user_defined_name_and_symbol;
        friend class named;
};

class named{
    private:
        const std::shared_ptr<name_and_symbol> ns;

    public:
        inline named(const std::string &name, const std::string &symbol): ns(new user_defined_name_and_symbol(name, symbol)) {}
        inline named(const opperation_defined_name_and_symbol &_ns): ns(new opperation_defined_name_and_symbol(_ns)) {}

        inline std::string name() const { return ns->name(); }
        inline std::string symbol() const { return ns->symbol(); }

        inline friend std::ostream &operator <<(std::ostream &os, const named &n) { return os << n.ns->name() << ", " << n.ns->symbol(); }

        inline const name_and_symbol &payload() const { return *ns; }
};

class measure_unit: public named{       // A data class
    private:
        const magnitude *measures_;

        double international_system_equivalence;

        measure_unit(double value, const magnitude *measures, const std::string &name, const std::string &symbol):
        named(name, symbol), measures_(measures), international_system_equivalence(value) {}

    public:
        const magnitude &measures() const { return *measures_; }

        inline bool operator <(const measure_unit &other) const{
            assert(measures_==other.measures_);
            return international_system_equivalence<other.international_system_equivalence;
        }
        
        friend class magnitude;
};
class magnitude: public named{          // A data class
    private:
        enum basicComponents{DISTANCE, MASS, TIME, CURRENT, TEMPERATURE, SUBSTANCE_AMOUNT, LUMINOSITY, ROTATION, nBasicComponents};

        std::array<char, nBasicComponents> exponents;
        std::set<measure_unit> units;

        inline void set_international_system_unit(const std::string &name, const std::string &symbol) { units.insert(std::move(measure_unit(1, this, name, symbol))); }

        inline magnitude(basicComponents basicIndex, const std::string &magn_name, const std::string &magn_symbol, const std::string &unit_name, const std::string &unit_symbol): 
        named(magn_name, magn_symbol), exponents{0} { exponents[basicIndex]=1; set_international_system_unit(unit_name, unit_symbol); }
        
        inline magnitude(const magnitude &m, const std::string &magn_name, const std::string &magn_symbol, const std::string &unit_name, const std::string &unit_symbol): 
        named(magn_name, magn_symbol), exponents{m.exponents} { set_international_system_unit(unit_name, unit_symbol); }
        
        inline magnitude(const opperation_defined_name_and_symbol &_ns): named(_ns), exponents{0} {};

        inline bool operator <(const magnitude &m) const { return exponents<m.exponents; }
        
    public:
        static magnitude distance, mass, time, current, temperature, substanceAmount, luminosity, rotation; // Basic ones
        static magnitude area, volume, velocity, frequency, angularVelocity, acceleration;                  // Distance and time
        static magnitude density, force, pressure, energy, momentum, work, power, heat, entropy;            // Mass and temperature
        static magnitude charge, voltage, resistance, inductance, capacitance, impedance;                   // Electricity
        static magnitude electricField, magneticField, electricFlux, magneticFlux;                          // Vectorial ones
    
        inline magnitude(const magnitude &m) = default;
        inline magnitude(const magnitude &m, const std::string &name, const std::string &symbol): named(name, symbol), exponents(m.exponents) {}

        magnitude operator *(const magnitude &m) const;
        magnitude operator /(const magnitude &m) const;
        
        inline bool operator ==(const magnitude &m) const { return exponents==m.exponents; }

        magnitude square() const;
        magnitude cube() const;
        magnitude inverse() const;
};

inline magnitude square(const magnitude &m) { return m.square(); }
inline magnitude cube(const magnitude &m) { return m.cube(); }
inline magnitude inverse(const magnitude &m) { return m.inverse(); }

#endif
