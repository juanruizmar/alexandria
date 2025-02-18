#ifndef CONSTANTS_250218
#define CONSTANTS_250218

class constant{
    private:
        double value;

        inline constant() = delete;
        inline constant(double v): value(v) {}

    public:
        static constant pi, euler, phi;
        static constant gravitation, light_velocity, elemental_charge;
        static constant avogadro, boltzmann, planck, rydberg;
        static constant electron_mass, proton_mass, neutron_mass;

        inline constant(const constant &other) = default;

        inline constant &operator =(const constant &other) = default;

        inline operator double() const { return value; }
};

#endif
