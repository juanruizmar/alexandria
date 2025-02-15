#ifndef MAGNITUDE_250206
#define MAGNITUDE_250206

#include <algorithm>
#include <memory>

#include <string>

#include <valarray>
#include <array>

#include <fstream>
#include <map>
#include <set>

#include <cassert>

#include "rational.hpp"

class magnitude;
class measuring_unit;
class magnitudes_base;
class measure_units_system;

class named{    // Easy to reuse
    private:
        class opperation_defined_name_and_symbol;
        class user_defined_name_and_symbol;

        class name_and_symbol{
            private:
                virtual std::map<user_defined_name_and_symbol, rational> as_raw_map() const = 0;
        
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
        
                inline std::map<user_defined_name_and_symbol, rational> as_raw_map() const { return {std::pair(*this, 1)}; }

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
                std::map<user_defined_name_and_symbol, rational> map;
        
                inline rational get_exp(const user_defined_name_and_symbol &key) const { return map.contains(key) ? map.at(key) : 0; }
                inline std::map<user_defined_name_and_symbol, rational> as_raw_map() const { return map; }
        
            public:
                opperation_defined_name_and_symbol(std::map<user_defined_name_and_symbol, rational> &&m): map(m) { assert(map.size()==1); }
                opperation_defined_name_and_symbol(const std::map<user_defined_name_and_symbol, rational> &m1, const std::map<user_defined_name_and_symbol, rational> &m2);

                std::string name() const;
                std::string symbol() const;
        
                opperation_defined_name_and_symbol square() const;
                opperation_defined_name_and_symbol cube() const;
                opperation_defined_name_and_symbol inverse() const;
        
                friend class name_and_symbol;
                friend class user_defined_name_and_symbol;
                friend class named;
        };
        
        const std::shared_ptr<name_and_symbol> ns;

    public:
        inline named(const named &other): ns(other.ns) {}

        inline named(const std::string &name, const std::string &symbol): ns(new user_defined_name_and_symbol(name, symbol)) {}
        inline named(const opperation_defined_name_and_symbol &_ns): ns(new opperation_defined_name_and_symbol(_ns)) {}

        inline std::string name() const { return ns->name(); }
        inline std::string symbol() const { return ns->symbol(); }

        inline friend std::ostream &operator <<(std::ostream &os, const named &n) { return os << n.ns->name() << ", " << n.ns->symbol(); }

        inline const name_and_symbol &payload() const { return *ns; }
};

class measuring_unit: public named{       // A data class
    private:
        magnitude* measures_;
        double international_system_equivalence;

        inline measuring_unit(magnitude &m, const std::string &name, const std::string &symbol): 
        named(name, symbol), measures_(&m), international_system_equivalence(1) { record(); }

        void record();
        void unrecord();

    public:
        static measuring_unit metter, kilogram, second, ampere, kelvin, mol, candela, radian;       // Basic ones
        static measuring_unit herz, newton, pascal, joule, watt, celsius, becquerel, steradian;     // Not so basic ones
        static measuring_unit coulomb, volt, ohm, siemens, henry, farad;                            // Electricity
        static measuring_unit gray, katal, lumen, lux;                                              // Others
        static measuring_unit tesla, weber;                                                         // Vectorial ones

        inline measuring_unit(double value, const measuring_unit &ref, const std::string &name, const std::string &symbol): 
        named(name, symbol), measures_(ref.measures_), international_system_equivalence(ref.international_system_equivalence*value) { record(); }

        inline measuring_unit() = delete;
        inline ~measuring_unit() { unrecord(); }

        inline measuring_unit &operator =(const measuring_unit &) = delete;

        const magnitude &measures() const { return *measures_; }

        inline bool operator <(const measuring_unit &other) const{
            assert(measures_==other.measures_);
            return international_system_equivalence<other.international_system_equivalence;
        }
        
        friend class magnitude;
        friend class units_system;
};
class magnitude: public named{          // A data class
    private:
        enum basicComponents{DISTANCE, MASS, TIME, CURRENT, TEMPERATURE, SUBSTANCE_AMOUNT, LUMINOSITY, ANGLE, nBasicComponents};

        std::array<rational, nBasicComponents> exponents;
        std::set<measuring_unit*> units;

        inline void record_unit(measuring_unit *unit) { units.emplace(unit); }
        inline void unrecord_unit(measuring_unit *unit) { units.erase(unit); }
        
        inline magnitude(const magnitude &m) = default;

        inline magnitude(basicComponents basicIndex, const std::string &name, const std::string &symbol): 
        named(name, symbol), exponents{0} { exponents[basicIndex]=1; }
        
        inline magnitude(const named &_ns): named(_ns), exponents{0} {};

        inline bool operator <(const magnitude &m) const { return exponents<m.exponents; }
        
    public:
        static magnitude distance, mass, time, current, temperature, substanceAmount, luminosity, angle;    // Basic ones
        static magnitude area, volume, velocity, frequency, angularVelocity, acceleration, solidAngle;      // Distance and time
        static magnitude density, force, pressure, energy, momentum, power, entropy;                        // Mass and temperature
        static magnitude charge, voltage, resistance, conductance, inductance, capacitance, impedance;      // Electricity
        static magnitude luminousFlux, illuminance, dose, catalyticActivity;                                // Others
        static magnitude electricField, magneticField, electricFlux, magneticFlux;                          // Vectorial ones
    
        inline magnitude(const magnitude &m, const std::string &name, const std::string &symbol): named(name, symbol), exponents(m.exponents) {}

        inline ~magnitude() { for(auto i: units) delete i; }

        magnitude operator *(const magnitude &m) const;
        magnitude operator /(const magnitude &m) const;
        
        inline bool operator ==(const magnitude &m) const { return exponents==m.exponents; }

        magnitude square() const;
        magnitude cube() const;
        magnitude inverse() const;

        friend class measuring_unit;
};
class variable{
    private:
        double value;
        const measuring_unit *unit;

    public:
        inline variable(double v, const measuring_unit &u): value(v), unit(&u) {}

        inline friend std::ostream &operator <<(std::ostream &os, const variable &v) { return os << v.value << " " << v.unit->symbol(); }

        friend class units_system;
};

class units_system{
    private:
        std::map<const magnitude *, const measuring_unit *> default_units;

        units_system() = default;

    public:
        static units_system international_system;

        inline units_system(std::initializer_list<std::reference_wrapper<measuring_unit>> units) { for(const auto &i: units) set_as_default(&i.get()); }

        inline void set_as_default(const measuring_unit *u) { default_units[u->measures_] = u; }

        std::string get(const variable &v){
            // At this moment, it only accepts basic units (metter, second, etc.)
            return std::to_string(v.unit->international_system_equivalence*v.value) + " " + default_units[&v.unit->measures()]->symbol();
        }

        inline const measuring_unit &default_unit(const magnitude &m) const { return *default_units.at(&m); }
};

inline magnitude square(const magnitude &m) { return m.square(); }
inline magnitude cube(const magnitude &m) { return m.cube(); }
inline magnitude inverse(const magnitude &m) { return m.inverse(); }

#endif
