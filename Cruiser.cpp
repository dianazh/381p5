#include "Cruiser.h"
#include <iostream>
using std::cout;
using std::endl;

const double Cruiser::FUEL_CAPACITY = 1000.0;
const double Cruiser::MAX_SPEED = 20.0;
const double Cruiser::FUEL_CONSUMPTION = 10.0;
const int Cruiser::RESISTANCE = 6;
const int Cruiser::FIREPOWER = 3;
const double Cruiser::MAX_ATTACKING_RANGE = 15.0;

// initialize, then output constructor message
Cruiser::Cruiser(const std::string& name_, Point position_)
  :Warship(name_, position_, FUEL_CAPACITY, MAX_SPEED, FUEL_CONSUMPTION, 
      RESISTANCE, FIREPOWER, MAX_ATTACKING_RANGE)
{
  cout << "Cruiser " << get_name() << " constructed" << endl;
}

// output destructor message
Cruiser::~Cruiser()
{
  cout << "Cruiser " << get_name() << " destructed" << endl;
}

// update the Cruiser: fire when in range; stop when target out of range
void Cruiser::update()
{
  Warship::update();
  if (is_attacking()) {
    if (target_in_range()) {
      fire_at_target();
    } else {
      cout << get_name() << " target is out of range" << endl;
      stop_attack();
    }
  }
}

// describe the Cruiser
void Cruiser::describe() const
{
  cout << "\nCruiser ";
  Warship::describe();
}

// Cruiser shall attack back when hit
void Cruiser::receive_hit(int hit_force, Ship* attacker_ptr)
{
  Ship::receive_hit(hit_force, attacker_ptr);
  if (!is_attacking()) {
    attack(attacker_ptr);
  }
}
