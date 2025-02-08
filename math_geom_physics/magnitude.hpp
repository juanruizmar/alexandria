#ifndef MAGNITUDE_250206
#define MAGNITUDE_250206

#include <algorithm>

#include <string>

#include <valarray>
#include <array>

#include <fstream>

class magnitude{    // A data class
    private:
        class magnitudeSymbols{
            public:
                enum basicComponents{DISTANCE, MASS, TIME, CURRENT, TEMPERATURE, SUBSTANCE_AMOUNT, LUMINOSITY, ROTATION, nBasicComponents};
                std::string basicMagnitudesSymbols[nBasicComponents];
                std::string basicUnitsSymbols[nBasicComponents];

                magnitudeSymbols();
        };

        static magnitudeSymbols symbols;

        typedef std::array<char, magnitudeSymbols::nBasicComponents> basicComponentExponents;

        basicComponentExponents exponents;

        inline magnitude(): exponents() {};
        inline magnitude(magnitudeSymbols::basicComponents basicIndex): exponents{0} { exponents[basicIndex]=1; };

        inline bool operator <(const magnitude &m) const { return exponents<m.exponents; }

    public:
        static magnitude distance, mass, time, current, temperature, substanceAmount, luminosity, rotation; // Basic ones
        static magnitude area, volume, density, velocity, acceleration, force, 
        pressure, energy, power, frequency, 
        charge, voltage, resistance, capacitance, 
        inductance, momentum, angularVelocity, 
        work, heat, entropy, impedance;

        static magnitude electricField, magneticField, electricFlux, magneticFlux;

        magnitude operator *(const magnitude &m) const;
        magnitude operator /(const magnitude &m) const;
        
        inline bool operator ==(const magnitude &m) const { return exponents==m.exponents; }

        magnitude square() const;
        magnitude cube() const;
        magnitude inverse() const;
        
        std::string units() const;
        std::string symbol() const;

        inline friend std::ostream &operator <<(std::ostream &os, const magnitude &m) { return os << m.symbol(); }

        friend class scalarVariable;
};

inline magnitude square(const magnitude &m) { return m.square(); }
inline magnitude cube(const magnitude &m) { return m.cube(); }
inline magnitude inverse(const magnitude &m) { return m.inverse(); }

class scalarVariable{
    private:
        const magnitude unit;
        double value;

    public:
        inline scalarVariable(double x, magnitude m): value(x), unit(m) {}

        inline friend std::ostream &operator <<(std::ostream &os, const scalarVariable &v) { return os << v.value << " " << v.unit.units(); }
};

template<unsigned int nDimensions> class vectorialVariable{
    private:
        const magnitude unit_;
        std::array<double, nDimensions> components;

        inline vectorialVariable(const magnitude &unit): unit_(unit) {}

    public:
        vectorialVariable operator +(const vectorialVariable &v) const;
        vectorialVariable operator -(const vectorialVariable &v) const;
        vectorialVariable operator *(const vectorialVariable &v) const;
        vectorialVariable operator /(const vectorialVariable &v) const;
        vectorialVariable operator -() const;

        friend vectorialVariable<3> crossProduct(const vectorialVariable<3> &v1, const vectorialVariable<3> &v2);

};

#endif
