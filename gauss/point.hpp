#ifndef POINT_250217
#define POINT_250217

#include <initializer_list>
#include <cmath>

template<unsigned n_dimensions> class spacial_vector{
    private:
        double components[n_dimensions];

    protected:
        inline spacial_vector &operator +=(const spacial_vector &other) { for(unsigned i=0; i<n_dimensions; ++i) components[i]+=other.components[i]; return *this; }

        double module() const { double res=0; for(unsigned i=0; i<n_dimensions; ++i) res+=(pow(components[i], 2)); return std::sqrt(res); }

    public:
        inline spacial_vector() { for(unsigned int i=0; i<n_dimensions; ++i) components[i]=0; }
        
        inline spacial_vector(const std::initializer_list<double> &coords) { std::copy(coords.begin(), coords.end(), components); }

        inline double component(int i) const { return components[i]; }

        inline unsigned int range() const { return n_dimensions; }
};

template<unsigned int n_dimensions> class displacement: public spacial_vector<n_dimensions>{
    public:
        inline double module() const { return spacial_vector<n_dimensions>::module(); }
};
template<unsigned int n_dimensions> class point: public spacial_vector<n_dimensions>{
    public:    
        inline void displace(const displacement<n_dimensions> &velocity) { spacial_vector<n_dimensions>::operator +=(velocity); }
};

#endif
