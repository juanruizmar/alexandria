#include "constant.hpp"

#include <numbers>

constant constant::pi(std::numbers::pi);
constant constant::euler(std::numbers::e);
constant constant::phi(std::numbers::phi);

constant constant::gravitation(6.67430e-11);
constant constant::lightVelocity(299792458);
constant constant::elementaryCharge(1.602176634e-19);

constant constant::avogadro(6.02214076e23);
constant constant::boltzmann(1.380649e-23);
constant constant::planck(6.62607015e-34);
constant constant::rydberg(10973731.568157);

constant constant::electronMass(9.1093837139e-31);
constant constant::protonMass(1.67262192595e-27);
constant constant::neutronMass(1.67492750056e-27);
