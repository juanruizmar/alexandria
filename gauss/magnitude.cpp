#include "magnitude.hpp"

using namespace std;

magnitude magnitude::distance(DISTANCE, "distance", "d");
magnitude magnitude::mass(MASS, "mass", "m");
magnitude magnitude::time(TIME, "time", "t");
magnitude magnitude::current(CURRENT, "current", "I");
magnitude magnitude::temperature(TEMPERATURE, "temperature", "T");
magnitude magnitude::substanceAmount(SUBSTANCE_AMOUNT, "amount of substance", "n");
magnitude magnitude::luminosity(LUMINOSITY, "luminosity", "I_V");
magnitude magnitude::angle(ANGLE, "rotation", "theta");

magnitude magnitude::area(distance.square(), "area", "A");
magnitude magnitude::volume(distance.cube(), "volume", "V");
magnitude magnitude::frequency(time.inverse(), "frequency", "f");
magnitude magnitude::velocity(distance/time, "velocity", "v");
magnitude magnitude::acceleration(velocity/time, "acceleration", "a");
magnitude magnitude::solidAngle(angle.square(), "rotation", "theta");
magnitude magnitude::angularVelocity(angle/time, "angular velocity", "omega");
magnitude magnitude::density(mass/volume, "density", "rho");
magnitude magnitude::force(mass*acceleration, "force", "F");
magnitude magnitude::pressure(force/area, "pressure", "P");
magnitude magnitude::energy(force*distance, "energy", "E");
magnitude magnitude::power(energy/time, "power", "P");
magnitude magnitude::momentum(mass*velocity, "momentum", "p");
magnitude magnitude::charge(current*time, "charge", "q");
magnitude magnitude::entropy(energy/temperature, "entropy", "S");
magnitude magnitude::voltage(energy/charge, "voltage", "V");
magnitude magnitude::resistance(voltage/current, "resistance", "R");
magnitude magnitude::conductance(resistance.inverse(), "conductance", "G");
magnitude magnitude::capacitance(charge/voltage, "capacitance", "C");
magnitude magnitude::impedance(resistance, "impedance", "Z");
magnitude magnitude::illuminance(luminosity/angularVelocity.square(), "illuminance", "E_V");
magnitude magnitude::dose(energy/mass, "dose", "D");
magnitude magnitude::catalyticActivity(substanceAmount/time, "catalytic activity", "cat. act.");

magnitude magnitude::electricField(force/charge, "electric field", "boldE");
magnitude magnitude::magneticField(electricField/velocity, "magnetic field", "boldB");
magnitude magnitude::electricFlux(electricField*area, "electric flux", "Phi_E");
magnitude magnitude::magneticFlux(magneticField*area, "magnetic flux", "Phi_B");
magnitude magnitude::luminousFlux(luminosity/solidAngle, "lluminous flux", "Phi_V");

magnitude magnitude::inductance(magneticFlux/current, "inductance", "L");   // Defined after magneticFlux

measuring_unit measuring_unit::metter(magnitude::distance, "metter", "m");
measuring_unit measuring_unit::kilogram(magnitude::mass, "kilogram", "Kg");

measuring_unit measuring_unit::second(magnitude::time, "second", "s");
measuring_unit measuring_unit::ampere(magnitude::current, "ampere", "A");
measuring_unit measuring_unit::kelvin(magnitude::temperature, "kelvin", "K");
measuring_unit measuring_unit::mol(magnitude::substanceAmount, "mol", "mol");
measuring_unit measuring_unit::candela(magnitude::luminosity, "candela", "cd");
measuring_unit measuring_unit::radian(magnitude::angle, "radian", "rad");

measuring_unit measuring_unit::steradian(magnitude::solidAngle, "steradian", "sr");
measuring_unit measuring_unit::herz(magnitude::frequency, "herz", "Hz"); 
measuring_unit measuring_unit::newton(magnitude::force, "newton", "N");
measuring_unit measuring_unit::pascal(magnitude::pressure, "pascal", "Pa");
measuring_unit measuring_unit::joule(magnitude::energy, "joule", "J");
measuring_unit measuring_unit::watt(magnitude::power, "watt", "W");
//measuring_unit measuring_unit::celsius(magnitude::temperature);   // Difficult conversion (not implemented yet)
measuring_unit measuring_unit::becquerel(magnitude::frequency, "becquerel", "Bq");
measuring_unit measuring_unit::coulomb(magnitude::charge, "coulomb", "C"); 
measuring_unit measuring_unit::volt(magnitude::voltage, "volt", "V"); 
measuring_unit measuring_unit::ohm(magnitude::resistance, "ohm", "Omega"); 
measuring_unit measuring_unit::siemens(magnitude::conductance, "siemens", "S"); 
measuring_unit measuring_unit::henry(magnitude::inductance, "henry", "H"); 
measuring_unit measuring_unit::farad(magnitude::capacitance, "farad", "F");
measuring_unit measuring_unit::gray(magnitude::dose, "gray", "Gy"); 
measuring_unit measuring_unit::katal(magnitude::catalyticActivity, "katal", "kat"); 
measuring_unit measuring_unit::lumen(magnitude::luminousFlux, "lumen", "lm"); 
measuring_unit measuring_unit::lux(magnitude::illuminance, "lux", "lx");
measuring_unit measuring_unit::tesla(magnitude::magneticFlux, "tesla", "T"); 
measuring_unit measuring_unit::weber(magnitude::magneticField, "weber", "Wb");

units_system units_system::international_system({
    measuring_unit::metter,
    measuring_unit::kilogram,
    measuring_unit::second,
    measuring_unit::ampere,
    measuring_unit::kelvin,
    measuring_unit::mol,
    measuring_unit::candela,
    measuring_unit::radian
});

void measuring_unit::record() { measures_->record_unit(this); }
void measuring_unit::unrecord() { measures_->unrecord_unit(this); }

magnitude magnitude::operator *(const magnitude &other) const{
    magnitude res(payload()*other.payload());
    transform(exponents.begin(), exponents.end(),  other.exponents.begin(), res.exponents.begin(), [](rational a, rational b) { return a+b; });
    return res;
}
magnitude magnitude::operator /(const magnitude &other) const{
    magnitude res(payload()/other.payload());
    transform(exponents.begin(), exponents.end(),  other.exponents.begin(), res.exponents.begin(), [](rational a, rational b) { return a-b; });
    return res;
}

magnitude magnitude::square() const{
    magnitude res(payload().square());
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](rational n) { return n*2; });
    return res;
}
magnitude magnitude::cube() const{
    magnitude res(payload().cube());
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](rational n) { return n*3; });
    return res;
}
magnitude magnitude::inverse() const{
    magnitude res(payload().inverse());
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](rational n) { return -n; });
    return res;
}

named::opperation_defined_name_and_symbol named::user_defined_name_and_symbol::square() const{
    return map<user_defined_name_and_symbol, rational>{std::pair(*this, 2)};
}
named::opperation_defined_name_and_symbol named::user_defined_name_and_symbol::cube() const{
    return map<user_defined_name_and_symbol, rational>{std::pair(*this, 3)};
}
named::opperation_defined_name_and_symbol named::user_defined_name_and_symbol::inverse() const{
    return map<user_defined_name_and_symbol, rational>{std::pair(*this, -1)};
}

named::opperation_defined_name_and_symbol named::opperation_defined_name_and_symbol::square() const{
    opperation_defined_name_and_symbol res(*this);
    for(auto &i: res.map) i.second*=2;
    return res;
}
named::opperation_defined_name_and_symbol named::opperation_defined_name_and_symbol::cube() const{
    opperation_defined_name_and_symbol res(*this);
    for(auto &i: res.map) i.second*=3;
    return res;
}
named::opperation_defined_name_and_symbol named::opperation_defined_name_and_symbol::inverse() const{
    opperation_defined_name_and_symbol res(*this);
    for(auto &i: res.map) i.second*=-1;
    return res;
}

named::opperation_defined_name_and_symbol named::name_and_symbol::operator *(const name_and_symbol &other) const{
    return opperation_defined_name_and_symbol(as_raw_map(), other.as_raw_map());
}
named::opperation_defined_name_and_symbol named::name_and_symbol::operator /(const name_and_symbol &other) const{
    return opperation_defined_name_and_symbol(as_raw_map(), other.inverse().as_raw_map());
}
named::opperation_defined_name_and_symbol::opperation_defined_name_and_symbol( const std::map<user_defined_name_and_symbol, rational> &m1,  const std::map<user_defined_name_and_symbol, rational> &m2){
    for(const auto &i: m1) map.emplace(i.first, i.second + (m2.contains(i.first) ? m1.at(i.first) : 0) );
    for(const auto &i: m2) map.emplace(i.first, i.second + (m1.contains(i.first) ? m2.at(i.first) : 0) );
}

string named::opperation_defined_name_and_symbol::name() const{
    string res;
    for(auto i=map.begin(); true;){
        res+=i->first.name();
        if(i->second!=1) res+="^(" + to_string(i->second) + ")";
        ++i;
        if(i==map.end()) break;
        res+="*";
    }
    return res;
}
string named::opperation_defined_name_and_symbol::symbol() const{
    string res;
    for(auto i=map.begin(); true;){
        res+=i->first.symbol();
        if(i->second!=1) res+="^(" + to_string(i->second) + ")";
        ++i;
        if(i==map.end()) break;
        res+="*";
    }
    return res;
}
