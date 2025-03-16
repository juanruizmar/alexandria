#include "magnitude.hpp"

#include <numbers>

using namespace std;

value::magnitude value::magnitude::units;

value::magnitude value::magnitude::distance(DISTANCE);
value::magnitude value::magnitude::mass(MASS);
value::magnitude value::magnitude::time(TIME);
value::magnitude value::magnitude::current(CURRENT);
value::magnitude value::magnitude::temperature(TEMPERATURE);
value::magnitude value::magnitude::substanceAmount(SUBSTANCE_AMOUNT);
value::magnitude value::magnitude::luminosity(LUMINOSITY);
value::magnitude value::magnitude::angle(ANGLE);

value::magnitude value::magnitude::area(distance.square());
value::magnitude value::magnitude::volume(distance.cube());
value::magnitude value::magnitude::frequency(time.inverse());
value::magnitude value::magnitude::velocity(distance/time);
value::magnitude value::magnitude::acceleration(velocity/time);
value::magnitude value::magnitude::solidAngle(angle.square());
value::magnitude value::magnitude::angularVelocity(angle/time);
value::magnitude value::magnitude::density(mass/volume);
value::magnitude value::magnitude::force(mass*acceleration);
value::magnitude value::magnitude::pressure(force/area);
value::magnitude value::magnitude::energy(force*distance);
value::magnitude value::magnitude::power(energy/time);
value::magnitude value::magnitude::action(energy*time);
value::magnitude value::magnitude::momentum(mass*velocity);
value::magnitude value::magnitude::charge(current*time);
value::magnitude value::magnitude::entropy(energy/temperature);
value::magnitude value::magnitude::voltage(energy/charge);
value::magnitude value::magnitude::resistance(voltage/current);
value::magnitude value::magnitude::conductance(resistance.inverse());
value::magnitude value::magnitude::capacitance(charge/voltage);
value::magnitude value::magnitude::impedance(resistance);
value::magnitude value::magnitude::illuminance(luminosity/angularVelocity.square());
value::magnitude value::magnitude::dose(energy/mass);
value::magnitude value::magnitude::catalyticActivity(substanceAmount/time);
value::magnitude value::magnitude::permitivity(capacitance/distance);
value::magnitude value::magnitude::permeability(force/current.square());
value::magnitude value::magnitude::electricField(force/charge);
value::magnitude value::magnitude::magneticField(electricField/velocity);
value::magnitude value::magnitude::electricFlux(electricField*area);
value::magnitude value::magnitude::magneticFlux(magneticField*area);
value::magnitude value::magnitude::luminousFlux(luminosity/solidAngle);
value::magnitude value::magnitude::inductance(magneticFlux/current);

value::constant value::constant::pi(std::numbers::pi);
value::constant value::constant::euler(std::numbers::e);
value::constant value::constant::phi(std::numbers::phi);

value::constant value::constant::gravitation(6.67430e-11);
value::constant value::constant::lightVelocity(299792458);
value::constant value::constant::elementaryCharge(1.602176634e-19);

value::constant value::constant::avogadro(6.02214076e23);
value::constant value::constant::boltzmann(1.380649e-23);
value::constant value::constant::planck(6.62607015e-34);
value::constant value::constant::rydberg(10973731.568157);

value::constant value::constant::electronMass(9.1093837139e-31);
value::constant value::constant::protonMass(1.67262192595e-27);
value::constant value::constant::neutronMass(1.67492750056e-27);
value::constant value::constant::atomicMassConstant(1.66053906892e-27);

value value::pi(constant::pi);
value value::euler(constant::euler);
value value::phi(constant::phi);

value value::metter(magnitude::distance);
value value::kilogram(magnitude::mass);
value value::second(magnitude::time);
value value::ampere(magnitude::current);
value value::kelvin(magnitude::temperature);
value value::mole(magnitude::substanceAmount);
value value::candela(magnitude::luminosity);
value value::radian(magnitude::angle);

value value::newton(kilogram*metter/second.square());
value value::joule(newton*metter);
value value::coulomb(ampere*second);

value value::electronMass(constant::electronMass*kilogram);
value value::protonMass(constant::protonMass*kilogram);
value value::neutronMass(constant::neutronMass*kilogram);
value value::gravitationConstant(constant::gravitation*newton*metter.square()/kilogram.square());
value value::lightVelocity(constant::lightVelocity*metter/second);
value value::elementaryCharge(constant::elementaryCharge*coulomb);
value value::avogadroConstant(constant::avogadro/mole);
value value::boltzmannConstant(constant::boltzmann*joule/kelvin);
value value::molarGasConstant(boltzmannConstant*avogadroConstant);
value value::planckConstant(constant::planck*joule*second);
value value::planckBarConstant(planckConstant/(2*pi));
value value::stephanBoltzmannConstant((pi.square()/60)*pow(boltzmannConstant,4)/(pow(planckBarConstant,3)*pow(lightVelocity,2)));
value value::atomicMassConstant(constant::atomicMassConstant*kilogram);
value value::vacuumPermeability(4*constant::pi*1e-7*coulomb.square()/(newton*metter.square()));
value value::vacuumPermitivity((vacuumPermeability*pow(lightVelocity,2)).inverse());
value value::coulombConstant((4*pi*vacuumPermitivity).inverse());
value value::bohrMagneton(elementaryCharge*planckBarConstant/(2*electronMass));
value value::nuclearMagneton(elementaryCharge*planckBarConstant/(2*protonMass));
value value::magneticFluxQuantum(planckConstant/(2*elementaryCharge));
value value::rydbergConstant(constant::rydberg*metter.inverse());
value value::josephsonConstant(magneticFluxQuantum.inverse());
value value::comptonWaveLength(planckConstant/(electronMass*lightVelocity));

value::magnitude value::magnitude::operator *(const value::magnitude &other) const{
    value::magnitude res;
    transform(exponents.begin(), exponents.end(),  other.exponents.begin(), res.exponents.begin(), [](rational a, rational b) { return a+b; });
    return res;
}
value::magnitude value::magnitude::operator /(const value::magnitude &other) const{
    value::magnitude res;
    transform(exponents.begin(), exponents.end(),  other.exponents.begin(), res.exponents.begin(), [](rational a, rational b) { return a-b; });
    return res;
}

value value::operator +(const value &other) const{
    assert(measures==other.measures); 
    double real_value = scalar, other_real_value = other.scalar;
    for(auto &i: constants) real_value*=pow((double)i.first, i.second);
    for(auto &i: other.constants) other_real_value*=pow((double)i.first, i.second);
    return value(real_value+other_real_value, measures);
}

value::magnitude value::magnitude::square() const{
    value::magnitude res;
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](rational n) { return n*2; });
    return res;
}
value::magnitude value::magnitude::sq_root() const{
    value::magnitude res;
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](rational n) { return n/2; });
    return res;
}
value::magnitude value::magnitude::cube() const{
    value::magnitude res;
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](rational n) { return n*3; });
    return res;
}
value::magnitude value::magnitude::inverse() const{
    value::magnitude res;
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [](rational n) { return -n; });
    return res;
}

value::magnitude value::magnitude::pow(rational exp) const{
    value::magnitude res=*this; 
    transform(exponents.begin(), exponents.end(), res.exponents.begin(), [exp](rational n) { return n*exp; });
    return res;
}

void value::magnitude::display(ostream &os) const{
    bool any = false;
    static std::string basicComponents[] = {"m", "kg", "s", "A", "K", "mol", "cd", "rad"};

    os << " ";

    for(size_t i=0; i<nBasicComponents; ++i){
        if(exponents[i]!=0){
            any = true;
            os << basicComponents[i];
            if(exponents[i].is_integer()){
                if(exponents[i]!=1) os << "^" << exponents[i];
            }
            else os << "^(" << exponents[i] << ")";
            os << " ";
        }
    }
    if(!any) os << "units";
}
void value::display(std::ostream &os) const{
    double real_value = scalar;
    for(auto &i: constants) real_value*=pow((double)i.first, i.second);
    
    os << real_value;
    measures.display(os);
}
