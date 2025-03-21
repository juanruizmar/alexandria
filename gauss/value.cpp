#include "value.hpp"
#include "magnitude.hpp"

#include <alexandria/matmaths/io.hpp>

#include <numbers>

using namespace std;

magnitude magnitude::units;

magnitude magnitude::distance(DISTANCE);
magnitude magnitude::mass(MASS);
magnitude magnitude::time(TIME);
magnitude magnitude::current(CURRENT);
magnitude magnitude::temperature(TEMPERATURE);
magnitude magnitude::substanceAmount(SUBSTANCE_AMOUNT);
magnitude magnitude::luminosity(LUMINOSITY);
magnitude magnitude::angle(ANGLE);

magnitude magnitude::area(distance.square());
magnitude magnitude::volume(distance.cube());
magnitude magnitude::frequency(time.inverse());
magnitude magnitude::velocity(distance/time);
magnitude magnitude::acceleration(velocity/time);
magnitude magnitude::solidAngle(angle.square());
magnitude magnitude::angularVelocity(angle/time);
magnitude magnitude::density(mass/volume);
magnitude magnitude::force(mass*acceleration);
magnitude magnitude::pressure(force/area);
magnitude magnitude::energy(force*distance);
magnitude magnitude::power(energy/time);
magnitude magnitude::action(energy*time);
magnitude magnitude::momentum(mass*velocity);
magnitude magnitude::charge(current*time);
magnitude magnitude::entropy(energy/temperature);
magnitude magnitude::voltage(energy/charge);
magnitude magnitude::resistance(voltage/current);
magnitude magnitude::conductance(resistance.inverse());
magnitude magnitude::capacitance(charge/voltage);
magnitude magnitude::impedance(resistance);
magnitude magnitude::illuminance(luminosity/angularVelocity.square());
magnitude magnitude::dose(energy/mass);
magnitude magnitude::catalyticActivity(substanceAmount/time);
magnitude magnitude::permitivity(capacitance/distance);
magnitude magnitude::permeability(force/current.square());
magnitude magnitude::electricField(force/charge);
magnitude magnitude::magneticField(electricField/velocity);
magnitude magnitude::electricFlux(electricField*area);
magnitude magnitude::magneticFlux(magneticField*area);
magnitude magnitude::luminousFlux(luminosity/solidAngle);
magnitude magnitude::inductance(magneticFlux/current);

value::constant value::constant::pi(numbers::pi);
value::constant value::constant::euler(numbers::e);
value::constant value::constant::phi(numbers::phi);

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

metric_system metric_system::international_system(matrix<rational>::identity(magnitude::nBasicComponents), {"m", "kg", "s", "A", "K", "mol", "cd", "rad"});

std::map<std::ostream*, metric_system*> metric_system::configuration;

value value::operator +(const value &other) const{
    assert(measures==other.measures); 
    double real_value = scalar, other_real_value = other.scalar;
    for(auto &i: constants) real_value*=pow((double)i.first, i.second);
    for(auto &i: other.constants) other_real_value*=pow((double)i.first, i.second);
    return value(real_value+other_real_value, measures);
}

double value::get_numeric_value() const{
    double res = scalar;
    for(auto &i: constants) res*=pow((double)i.first, i.second);
    return res;
}

string metric_system::get_units(const value &v) const{
    bool any = false;
    string res;

    for(size_t i=0; i<magnitude::nBasicComponents; ++i){
        if(v.measures.exponents[i]!=0){
            any = true;
            res += symbols_[i];
            if(v.measures.exponents[i].is_integer()){
                if(v.measures.exponents[i]!=1) res += "^" + to_string(v.measures.exponents[i]);
            }
            else res += "^(" + to_string(v.measures.exponents[i]) + ")";
            res += " ";
        }
    }
    if(!any) res = "units";

    return res;
}
std::string metric_system::translate(const value &v) const{
    double numeric_value = v.get_numeric_value() * scale_factor();
    return scientific_double_to_str(numeric_value) + " " + get_units(v);
}

void value::display(std::ostream &os) const{
    os << get_metric_system(os).translate(*this);
}
