#ifndef VALUE_250206
#define VALUE_250206

#include <cmath>

#include <string>

#include <iostream>
#include <fstream>

#include <map>
#include <set>

#include <cassert>

#include "magnitude.hpp"

class value{
    private:
        class constant{             // A data class
            private:
                const double value;
        
                inline constant() = delete;
                
            public:
                static constant pi, euler, phi;
                static constant gravitation, lightVelocity, elementaryCharge;
                static constant avogadro, boltzmann, planck, rydberg;
                static constant electronMass, protonMass, neutronMass, atomicMassConstant;
                
                inline explicit constant(double v): value(v) {}
                inline constant(const constant &other) = default;
        
                inline constant &operator =(const constant &other) = default;
        
                inline double operator *(double k) const { return value*k; }
                inline double operator /(double k) const { return value/k; }
        
                inline double operator *(int k) const { return value*k; }
                inline double operator /(int k) const { return value/k; }
        
                inline constant operator +(const constant &k) const { return constant(value+k.value); }
                inline constant operator -(const constant &k) const { return constant(value-k.value); }
        
                inline friend double operator *(double k1, const constant &k2) { return k1*k2.value; }
                inline friend double operator /(double k1, const constant &k2) { return k1/k2.value; }
        
                inline friend double operator *(int k1, const constant &k2) { return k1*k2.value; }
                inline friend double operator /(int k1, const constant &k2) { return k1/k2.value; }
        
                inline operator double() const { return value; }
        };

        double scalar;
        const magnitude measures;
        std::map<constant, rational> constants;

        inline value &add_constants(const std::map<constant, rational> &kk) { for(const auto &i: kk) constants[i.first]+=i.second; return *this; }
        inline value &sub_constants(const std::map<constant, rational> &kk) { for(const auto &i: kk) constants[i.first]-=i.second; return *this; }

        inline value &operator *=(const constant &k) { constants[k]+=1; return *this; }
        inline value &operator /=(const constant &k) { constants[k]/=1; return *this; }

        inline value(const magnitude &m): value(1, m) {}
        inline value(const constant &k): value(1, magnitude::units, {{k,1}}) {}
        inline value(double v, const magnitude &m): scalar(v), measures(m) {}
        inline value(double v, const magnitude &m, const std::map<constant, rational> &kk): scalar(v), measures(m), constants(kk) {}
        inline value(double v, const magnitude &m, const std::map<constant, rational> &kk, rational exp): value(pow(v,(double)exp), m.pow(exp), kk) { for(auto &i: constants) i.second*=exp; }
        
        inline friend value operator *(double k, const magnitude &m) { return value(k, m, {}); }
        inline friend value operator /(double k, const magnitude &m) { return value(k, m.inverse(), {}); }

        inline friend value operator *(const constant &k, const magnitude &m) { return value(1, m, {{k,1}}); }
        inline friend value operator /(const constant &k, const magnitude &m) { return value(1, m.inverse(), {{k,1}}); }

        inline value operator *(const magnitude &m) { return value(scalar, measures*m, constants); }
        inline value operator /(const magnitude &m) { return value(scalar, measures/m, constants); }

        void display(std::ostream &os) const;

        friend class metric_system;

    public:
        static value pi, euler, phi;
        
        static value metter, kilogram, second, ampere, kelvin, mole, candela, radian;
        static value newton, joule, coulomb;

        static value gravitationConstant, lightVelocity, elementaryCharge;
        static value avogadroConstant, boltzmannConstant;
        static value molarGasConstant, stephanBoltzmannConstant, atomicMassConstant;
        static value vacuumPermitivity, vacuumPermeability, coulombConstant;
        static value planckConstant, planckBarConstant; 
        static value electronMass, protonMass, neutronMass, bohrMagneton, nuclearMagneton;
        static value magneticFluxQuantum, rydbergConstant, josephsonConstant, comptonWaveLength;

        inline value operator *(const value &other) { return value(scalar*other.scalar, measures*other.measures, constants).add_constants(other.constants); }
        inline value operator /(const value &other) { return value(scalar/other.scalar, measures/other.measures, constants).sub_constants(other.constants); }

        inline value operator *(double k) const { return value(scalar*k, measures, constants); }
        inline value operator /(double k) const { return value(scalar/k, measures, constants); }

        inline friend value operator *(double k, const value &v) { return v.operator*(k); }
        inline friend value operator /(double k, const value &v) { return v.inverse().operator*(k); }

        value operator +(const value &other) const;
        
        inline value square() const { return pow(*this, 2); }
        inline value sq_root() const { return pow(*this, rational(1,2)); }
        inline value cube() const { return pow(*this, 3); }
        inline value inverse() const { return pow(*this, -1); }

        inline friend value pow(const value &base, rational exp) { return value(base.scalar, base.measures, base.constants, exp); }

        double get_numeric_value() const;

        inline friend std::ostream &operator <<(std::ostream &os, const value &v) { v.display(os); return os; }
};

class metric_system{
    private:
        matrix<rational> basis_change_matrix_;
        std::array<std::string, magnitude::nBasicComponents> symbols_;

        std::set <std::ostream*> owned;
        static std::map<std::ostream*, metric_system*> configuration;

        inline void internal_take(std::ostream *os){
            assert(!configuration.contains(os)); 
            assert(!owns(*os)); 
            configuration[os]=this; 
            owned.insert(os); 
            assert(owns(*os)); 
        }
        inline void internal_release(std::ostream *os){
            assert(configuration.contains(os)); 
            assert(configuration[os]==this); 
            assert(owned.contains(os)); 
            configuration.erase(os);
            owned.erase(os); 
        }
        
    public:
        static metric_system international_system;
        
        inline metric_system(const matrix<rational> &basis_change_matrix, const std::array<std::string, magnitude::nBasicComponents> &symbols): 
        basis_change_matrix_(basis_change_matrix), symbols_(symbols) {}

        inline metric_system(const metric_system&) = delete;
        inline metric_system operator=(const metric_system&) = delete;
        
        inline ~metric_system() { for(auto const &os: owned) configuration.erase(os); owned.clear(); }
        
        std::string translate(const value&) const;
        
        inline bool owns(std::ostream &os) const { return owned.contains(&os); }
        inline void take(std::ostream &os){
            if(configuration.contains(&os)) configuration.at(&os)->internal_release(&os);
            internal_take(&os);
        }
        inline void release(std::ostream &os) { 
            if(!configuration.contains(&os)) throw std::logic_error("Attempt to release a metric system that is not associated with the given stream.");
            internal_release(&os); 
        }

        inline double scale_factor() const { return 1; }
        std::string get_units(const value&) const;

        inline friend const metric_system &get_metric_system(std::ostream &os);
};

inline const metric_system &get_metric_system(std::ostream &os) { return metric_system::configuration.contains(&os) ? *metric_system::configuration[&os] : metric_system::international_system; }

#endif
