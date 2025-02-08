#include "magnitude.hpp"

#include <iostream>

using namespace std;

magnitude magnitude::distance(DISTANCE, "distance", "d", "metter", "m");
magnitude magnitude::mass(MASS, "mass", "m", "kilogram", "Kg");
magnitude magnitude::time(TIME, "time", "t", "second", "s");
magnitude magnitude::current(CURRENT, "current", "I", "ampere", "A");
magnitude magnitude::temperature(TEMPERATURE, "temperature", "T", "kelvin", "K");
magnitude magnitude::substanceAmount(SUBSTANCE_AMOUNT, "amount of substance", "n", "mol", "mol");
magnitude magnitude::luminosity(LUMINOSITY, "luminosity", "I_V", "candela", "cd");
magnitude magnitude::rotation(ROTATION, "rotation", "theta", "pi*radian", "(pi*rads)");

magnitude magnitude::area(distance.square(), "area", "A");
magnitude magnitude::volume(distance.cube(), "volume", "V");
magnitude magnitude::frequency(time.inverse(), "frequency", "f", "herz", "Hz");
magnitude magnitude::velocity(distance/time, "velocity", "v");
magnitude magnitude::acceleration(velocity/time, "acceleration", "a");
magnitude magnitude::angularVelocity(rotation/time, "angular velocity", "omega");
magnitude magnitude::density(mass/volume, "density", "rho");
magnitude magnitude::force(mass*acceleration, "force", "F");
magnitude magnitude::pressure(force/area, "pressure", "P");
magnitude magnitude::energy(force*distance, "energy", "E");
magnitude magnitude::power(energy/time, "power", "P");
magnitude magnitude::momentum(mass*velocity, "momentum", "p");
magnitude magnitude::charge(current*time, "charge", "q");
magnitude magnitude::work(distance*force, "work", "W");
magnitude magnitude::heat(mass*velocity.square(), "heat", "Q");
magnitude magnitude::entropy(heat/temperature, "entropy", "S");
magnitude magnitude::voltage(energy/charge, "voltage", "V");
magnitude magnitude::resistance(voltage/current, "resistance", "R");
magnitude magnitude::impedance(voltage/current, "impedance", "Z");
magnitude magnitude::capacitance(charge/voltage, "capacitance", "C");

magnitude magnitude::electricField(force/charge, "electric field", "boldE");
magnitude magnitude::magneticField(electricField/velocity, "magnetic field", "boldB");
magnitude magnitude::electricFlux(electricField*area, "electric flux", "Phi_E");
magnitude magnitude::magneticFlux(magneticField*area, "magnetic flux", "Phi_B");

magnitude magnitude::inductance(magneticFlux/current, "inductance", "L");   // Defined after magneticFlux

magnitude magnitude::operator *(const magnitude &other) const{
    magnitude res(payload()*other.payload());
    transform(exponents.begin(), exponents.end(),  other.exponents.begin(), res.exponents.begin(), [](char a, char b) { return a+b; });
    return res;
}
magnitude magnitude::operator /(const magnitude &other) const{
    magnitude res(payload()/other.payload());
    transform(exponents.begin(), exponents.end(),  other.exponents.begin(), res.exponents.begin(), [](char a, char b) { return a-b; });
    return res;
}

magnitude magnitude::square() const{
    magnitude res(payload().square());
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](char n) { return n*2; });
    return res;
}
magnitude magnitude::cube() const{
    magnitude res(payload().cube());
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](char n) { return n*3; });
    return res;
}
magnitude magnitude::inverse() const{
    magnitude res(payload().inverse());
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](char n) { return -n; });
    return res;
}

opperation_defined_name_and_symbol user_defined_name_and_symbol::square() const{
    return map<user_defined_name_and_symbol, float>{std::pair(*this, 2)};
}
opperation_defined_name_and_symbol user_defined_name_and_symbol::cube() const{
    return map<user_defined_name_and_symbol, float>{std::pair(*this, 3)};
}
opperation_defined_name_and_symbol user_defined_name_and_symbol::inverse() const{
    return map<user_defined_name_and_symbol, float>{std::pair(*this, -1)};
}
opperation_defined_name_and_symbol opperation_defined_name_and_symbol::square() const{
    opperation_defined_name_and_symbol res(*this);
    for(auto &i: res.map) i.second*=2;
    return res;
}
opperation_defined_name_and_symbol opperation_defined_name_and_symbol::cube() const{
    opperation_defined_name_and_symbol res(*this);
    for(auto &i: res.map) i.second*=3;
    return res;
}
opperation_defined_name_and_symbol opperation_defined_name_and_symbol::inverse() const{
    opperation_defined_name_and_symbol res(*this);
    for(auto &i: res.map) i.second*=-1;
    return res;
}

opperation_defined_name_and_symbol name_and_symbol::operator *(const name_and_symbol &other) const{
    return opperation_defined_name_and_symbol(as_raw_map(), other.as_raw_map());
}
opperation_defined_name_and_symbol name_and_symbol::operator /(const name_and_symbol &other) const{
    return opperation_defined_name_and_symbol(as_raw_map(), other.inverse().as_raw_map());
}
opperation_defined_name_and_symbol::opperation_defined_name_and_symbol( const std::map<user_defined_name_and_symbol, float> &m1,  const std::map<user_defined_name_and_symbol, float> &m2){
    for(const auto &i: m1) map.emplace(i.first, i.second+ (m2.contains(i.first) ? m1.at(i.first) : 0) );
    for(const auto &i: m2) map.emplace(i.first, i.second+ (m1.contains(i.first) ? m2.at(i.first) : 0) );
}

inline string opperation_defined_name_and_symbol::name() const{
    string res;
    for(auto i=map.begin(); true;){
        res+=i->first.name();
        if(i->second!=1) res+="^(" + std::to_string(i->second) + ")";
        ++i;
        if(i==map.end()) break;
        res+="*";
    }
    return res;
}
inline string opperation_defined_name_and_symbol::symbol() const{
    string res;
    for(auto i=map.begin(); true;){
        res+=i->first.symbol();
        if(i->second!=1) res+="^(" + std::to_string(i->second) + ")";
        ++i;
        if(i==map.end()) break;
        res+="*";
    }
    return res;
}
