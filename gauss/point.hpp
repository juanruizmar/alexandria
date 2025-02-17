#ifndef POINT_250217
#define POINT_250217

#include <array>
#include <cmath>

template<unsigned int n_dimensions> class spacial_vector{
    private:
        std::array<double, n_dimensions> components;

    protected:
        inline spacial_vector &operator +=(const spacial_vector &other) { for(unsigned int i=0; i<n_dimensions; ++i) components[i]+=other.components[i]; return *this; }

        double module() const { double res=0; for(unsigned int i=0; i<n_dimensions; ++i) res+=(pow(components[i], 2)); return std::sqrt(res); }

    public:
        inline spacial_vector() { components.fill(0); }
        
        inline spacial_vector(const std::array<double, n_dimensions> &coords): components(coords) {}
        inline spacial_vector(std::array<double, n_dimensions> &&coords): components(coords) {}

        inline double component(int i) const { return components[i]; }

        inline unsigned int range() const { return n_dimensions; }
};

template<unsigned int n_dimensions> class displacement: public spacial_vector<n_dimensions>{
    public:
        inline displacement(const std::array<double, n_dimensions> &coords): spacial_vector<n_dimensions>(coords) {}
        inline displacement(std::array<double, n_dimensions> &&coords): spacial_vector<n_dimensions>(coords) {}
    
        inline double module() const { return spacial_vector<n_dimensions>::module(); }
};
template<unsigned int n_dimensions> class point: public spacial_vector<n_dimensions>{
    public:
        inline point(const std::array<double, n_dimensions> &coords): spacial_vector<n_dimensions>(coords) {}
        inline point(std::array<double, n_dimensions> &&coords): spacial_vector<n_dimensions>(coords) {}
    
        inline void displace(const displacement<n_dimensions> &velocity) { spacial_vector<n_dimensions>::operator +=(velocity); }
};

#endif
