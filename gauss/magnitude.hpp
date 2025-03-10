#ifndef MAGNITUDE_250206
#define MAGNITUDE_250206

#include <algorithm>
#include <cmath>

#include <iostream>

#include <valarray>
#include <array>

#include <string>

#include <fstream>
#include <map>
#include <set>

#include <cassert>

#include "rational.hpp"

class value{
    private:
        class magnitude{            // A data class
            private:
                enum basicComponents{DISTANCE, MASS, TIME, CURRENT, TEMPERATURE, SUBSTANCE_AMOUNT, LUMINOSITY, ANGLE, nBasicComponents};
        
                std::array<rational, nBasicComponents> exponents;
        
                inline magnitude(): exponents{0} {}
                inline magnitude(basicComponents basicIndex): exponents{0} { exponents[basicIndex]=1; }

            public:
                static magnitude units;                                                                             // Adimensional one
                static magnitude distance, mass, time, current, temperature, substanceAmount, luminosity, angle;    // Basic ones
                static magnitude area, volume, velocity, frequency, angularVelocity, acceleration, solidAngle;      // Distance and time
                static magnitude density, force, pressure, energy, momentum, power, action, entropy;                // Mass and temperature
                static magnitude charge, voltage, resistance, conductance, inductance, capacitance, impedance;      // Electricity
                static magnitude luminousFlux, illuminance, dose, catalyticActivity;                                // Others
                static magnitude electricField, magneticField, electricFlux, magneticFlux;                          // Vectorial ones
                static magnitude permitivity, permeability;

                inline magnitude(const magnitude &m) = default;
        
                magnitude operator *(const magnitude &m) const;
                magnitude operator /(const magnitude &m) const;
        
                magnitude square() const;
                magnitude sq_root() const;
                magnitude cube() const;
                magnitude inverse() const;

                magnitude pow(rational exp) const;

                inline void display(std::ostream &os) const{
                    os << "[ ";
                    for(size_t i=0; i<nBasicComponents; ++i) os << exponents[i] << " ";
                    os << "]";
                }

                inline bool operator ==(const magnitude &m) const { return exponents==m.exponents; }
        };
        class constant{             // A data class
            private:
                double value;
        
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

        inline void display(std::ostream &os) const{
            double real_value = scalar;
            for(auto &i: constants) real_value*=pow((double)i.first, i.second);
            
            os << real_value << " ";
            measures.display(os);
        }

    public:
        static value pi, euler, phi;
        static value gravitationConstant, lightVelocity, elementaryCharge;
        static value avogadroConstant, boltzmannConstant;
        static value molarGasConstant, stephanBoltzmannConstant, atomicMassConstant;
        static value vacuumPermitivity, vacuumPermeability, coulombConstant;
        static value planckConstant, planckBarConstant; 
        static value electronMass, protonMass, neutronMass, bohrMagneton, nuclearMagneton;
        static value magneticFluxQuantum, rydbergConstant, josephsonConstant, comptonWaveLength;

        static value metter, kilogram, second, ampere, kelvin, mole, candela, radian;

        inline value operator *(const value &other) { return value(scalar*other.scalar, measures*other.measures, constants).add_constants(other.constants); }
        inline value operator /(const value &other) { return value(scalar/other.scalar, measures/other.measures, constants).sub_constants(other.constants); }

        inline value operator *(double k) const { return value(scalar*k, measures, constants); }
        inline value operator /(double k) const { return value(scalar/k, measures, constants); }

        inline friend value operator *(double k, const value &v) { return v.operator*(k); }
        inline friend value operator /(double k, const value &v) { return v.inverse().operator*(k); }

        inline value operator +(const value &other) const{
            assert(measures==other.measures); 
            double real_value = scalar, other_real_value = other.scalar;
            for(auto &i: constants) real_value*=pow((double)i.first, i.second);
            for(auto &i: other.constants) other_real_value*=pow((double)i.first, i.second);
            return value(real_value+other_real_value, measures);
        }
        
        inline value square() const { return pow(*this, 2); }
        inline value sq_root() const { return pow(*this, rational(1,2)); }
        inline value cube() const { return pow(*this, 3); }
        inline value inverse() const { return pow(*this, -1); }

        inline friend value pow(const value &base, rational exp) { return value(base.scalar, base.measures, base.constants, exp); }

        inline friend std::ostream &operator <<(std::ostream &os, const value &v) { v.display(os); return os; }
};

#endif
