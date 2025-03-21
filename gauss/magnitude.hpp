#ifndef magnitude_250206
#define magnitude_250206

#include <algorithm>

#include <fstream>

#include <array>

#include "matrix.hpp"
#include "rational.hpp"
#include "name_and_symbol.hpp"

class magnitude{            // A data class
    private:
        enum basicComponents{DISTANCE, MASS, TIME, CURRENT, TEMPERATURE, SUBSTANCE_AMOUNT, LUMINOSITY, ANGLE, nBasicComponents};

        std::array<rational, nBasicComponents> exponents;

        inline magnitude(): exponents{0} {}
        inline magnitude(basicComponents basicIndex): exponents{0} { exponents[basicIndex]=1; }

        friend class metric_system;

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

        inline bool operator ==(const magnitude &m) const { return exponents==m.exponents; }

        inline static unsigned n_basic_components() { return nBasicComponents; }
};

#endif
