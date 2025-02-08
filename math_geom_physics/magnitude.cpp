#include "magnitude.hpp"

using namespace std;

magnitude magnitude::distance(magnitudeSymbols::DISTANCE);
magnitude magnitude::mass(magnitudeSymbols::MASS);
magnitude magnitude::time(magnitudeSymbols::TIME);
magnitude magnitude::current(magnitudeSymbols::CURRENT);
magnitude magnitude::temperature(magnitudeSymbols::TEMPERATURE);
magnitude magnitude::substanceAmount(magnitudeSymbols::SUBSTANCE_AMOUNT);
magnitude magnitude::luminosity(magnitudeSymbols::LUMINOSITY);
magnitude magnitude::rotation(magnitudeSymbols::ROTATION);

magnitude::magnitudeSymbols::magnitudeSymbols(){
    basicMagnitudesSymbols[DISTANCE] = "l";
    basicMagnitudesSymbols[MASS] = "m";
    basicMagnitudesSymbols[TIME] = "t";
    basicMagnitudesSymbols[CURRENT] = "i";
    basicMagnitudesSymbols[TEMPERATURE] = "T";
    basicMagnitudesSymbols[SUBSTANCE_AMOUNT] = "n";
    basicMagnitudesSymbols[LUMINOSITY] = "I_V";
    basicMagnitudesSymbols[ROTATION] = "phi";
    
    basicUnitsSymbols[DISTANCE] = "m";
    basicUnitsSymbols[MASS] = "Kg";
    basicUnitsSymbols[TIME] = "s";
    basicUnitsSymbols[CURRENT] = "A";
    basicUnitsSymbols[TEMPERATURE] = "K";
    basicUnitsSymbols[SUBSTANCE_AMOUNT] = "mol";
    basicUnitsSymbols[LUMINOSITY] = "cd";
    basicUnitsSymbols[ROTATION] = "(pi*rads)";
}

magnitude::magnitudeSymbols magnitude::symbols;

magnitude magnitude::area(distance.square());
magnitude magnitude::volume(distance.cube());
magnitude magnitude::density(mass/volume);
magnitude magnitude::velocity(distance/time);
magnitude magnitude::acceleration(velocity/time);
magnitude magnitude::force(mass*acceleration);
magnitude magnitude::pressure(force/area);
magnitude magnitude::energy(force*distance);
magnitude magnitude::power(energy/time);
magnitude magnitude::frequency(time.inverse());
magnitude magnitude::charge(current*time);
magnitude magnitude::voltage(energy/charge);
magnitude magnitude::resistance(voltage/current);
magnitude magnitude::capacitance(charge/voltage);
magnitude magnitude::inductance(magneticFlux/current);
magnitude magnitude::momentum(mass*velocity);
magnitude magnitude::angularVelocity(rotation/time);
magnitude magnitude::work(distance*force);
magnitude magnitude::heat(mass*velocity.square());
magnitude magnitude::entropy(heat/temperature);
magnitude magnitude::impedance(voltage/current);

magnitude magnitude::electricField(force/charge);
magnitude magnitude::electricFlux(electricField*area);
magnitude magnitude::magneticField(electricField/velocity);
magnitude magnitude::magneticFlux(magneticField*area);

string magnitude::symbol() const{
    string res;
    for(int i=0; i<magnitudeSymbols::nBasicComponents; ++i) if(exponents[i]!=0){
        if(res.size()!=0) res += " * ";
        if(exponents[i]==1) res += symbols.basicMagnitudesSymbols[i];
        else res += symbols.basicMagnitudesSymbols[i] + "^" + to_string(exponents[i]);
    }
    return res;
}
string magnitude::units() const{
    string res;
    for(int i=0; i<magnitudeSymbols::nBasicComponents; ++i) if(exponents[i]!=0){
        if(res.size()!=0) res += " * ";
        if(exponents[i]==1) res += symbols.basicUnitsSymbols[i];
        else res += symbols.basicUnitsSymbols[i] + "^" + to_string(exponents[i]);
    }
    return res;
}

magnitude magnitude::operator *(const magnitude &m) const{
    magnitude res;
    transform(exponents.begin(), exponents.end(),  m.exponents.begin(), res.exponents.begin(), [](char a, char b) { return a+b; });
    return res;
}
magnitude magnitude::operator /(const magnitude &m) const{
    magnitude res;
    transform(exponents.begin(), exponents.end(),  m.exponents.begin(), res.exponents.begin(), [](char a, char b) { return a-b; });
    return res;
}

magnitude magnitude::square() const{
    magnitude res;
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](char n) { return n*2; });
    return res;
}
magnitude magnitude::cube() const{
    magnitude res;
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](char n) { return n*3; });
    return res;
}
magnitude magnitude::inverse() const{
    magnitude res;
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](char n) { return -n; });
    return res;
}

template<unsigned int N> vectorialVariable<N> vectorialVariable<N>::operator +(const vectorialVariable<N> &v) const{
    vectorialVariable<N> res;
    transform(components.begin(), components.end(), v.components.begin(), res.components.begin(), [](double a, double b) { return a+b; });
    return res;
}
template<unsigned int N> vectorialVariable<N> vectorialVariable<N>::operator -(const vectorialVariable<N> &v) const{
    vectorialVariable<N> res;
    transform(components.begin(), components.end(), v.components.begin(), res.components.begin(), [](double a, double b) { return a-b; });
    return res;
}
template<unsigned int N> vectorialVariable<N> vectorialVariable<N>::operator *(const vectorialVariable<N> &v) const{
    vectorialVariable<N> res;
    transform(components.begin(), components.end(), v.components.begin(), res.components.begin(), [](double a, double b) { return a*b; });
    return res;
}
template<unsigned int N> vectorialVariable<N> vectorialVariable<N>::operator /(const vectorialVariable<N> &v) const{
    vectorialVariable<N> res;
    transform(components.begin(), components.end(), v.components.begin(), res.components.begin(), [](double a, double b) { return a/b; });
    return res;
}
template<unsigned int N> vectorialVariable<N> vectorialVariable<N>::operator -() const{
    vectorialVariable<N> res;
    transform(components.begin(), components.end(), res.components.begin(), [](double n) { return -n; });
    return res;
}

vectorialVariable<3> crossProduct(const vectorialVariable<3> &v1, const vectorialVariable<3> &v2){
    vectorialVariable<3> res(v1.unit_*v2.unit_);
    for(int i=0; i<3; ++i) res.components[i%3] = v1.components[(i+1)%3] * v2.components[(i+2)%3] - v1.components[(i+2)%3] * v2.components[(i+1)%3];
    return res;
}
