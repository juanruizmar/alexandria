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

value value::pi(constant::pi/magnitude::angle);
value value::euler(constant::euler);
value value::phi(constant::phi);
value value::electronMass(constant::electronMass*magnitude::mass);
value value::protonMass(constant::protonMass*magnitude::mass);
value value::neutronMass(constant::neutronMass*magnitude::mass);
value value::gravitationConstant(constant::gravitation*magnitude::force*magnitude::area/magnitude::mass.square());
value value::lightVelocity(constant::lightVelocity*magnitude::velocity);
value value::elementaryCharge(constant::elementaryCharge*magnitude::charge);
value value::avogadroConstant(constant::avogadro/magnitude::substanceAmount);
value value::boltzmannConstant(constant::boltzmann*magnitude::energy/magnitude::temperature);
value value::molarGasConstant(boltzmannConstant*avogadroConstant);
value value::planckConstant(constant::planck*magnitude::action);
value value::planckBarConstant(planckConstant/(2*pi));
value value::stephanBoltzmannConstant((pi.square()/60)*pow(boltzmannConstant,4)/(pow(planckBarConstant,3)*pow(lightVelocity,2)));
value value::atomicMassConstant(constant::atomicMassConstant*magnitude::mass);
value value::vacuumPermeability(4*constant::pi*1e-7*magnitude::force/magnitude::current.square());
value value::vacuumPermitivity((vacuumPermeability*pow(lightVelocity,2)).inverse());
value value::coulombConstant((4*pi*vacuumPermitivity*magnitude::angle).inverse());
value value::bohrMagneton(elementaryCharge*planckBarConstant/(2*electronMass));
value value::nuclearMagneton(elementaryCharge*planckBarConstant/(2*protonMass));
value value::magneticFluxQuantum(planckConstant/(2*elementaryCharge));
value value::rydbergConstant(constant::rydberg*magnitude::distance.inverse());
value value::josephsonConstant(magneticFluxQuantum.inverse());
value value::comptonWaveLength(planckConstant/(electronMass*lightVelocity));

value value::metter(1*value::magnitude::distance);
value value::kilogram(1*value::magnitude::mass);
value value::second(1*value::magnitude::time);
value value::ampere(1*value::magnitude::current);
value value::kelvin(1*value::magnitude::temperature);
value value::mole(1*value::magnitude::substanceAmount);
value value::candela(1*value::magnitude::luminosity);
value value::radian(1*value::magnitude::angle);

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
