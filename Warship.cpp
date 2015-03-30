#include "Warship.h"
#include "Utility.h"
#include <iostream>
using std::cout;
using std::endl;
using std::shared_ptr;

// initialize, then output constructor message
Warship::Warship(const std::string& name_, Point position_, double fuel_capacity_, 
    double maximum_speed_, double fuel_consumption_, int resistance_,
    int firepower_, double maximum_range_)
  :Ship(name_, position_, fuel_capacity_, maximum_speed_, fuel_consumption_, resistance_),
  firepower(firepower_), maximum_range(maximum_range_), warship_state(State::NOT_ATTACKING)
{}

// perform warship-specific behavior
void Warship::update()
{
  Ship::update();
  if (warship_state == State::ATTACKING) {
    if (!is_afloat() || target.expired() || !target.lock()->is_afloat()) {
      stop_attack();
    } else {
      cout << get_name() << " is attacking" << endl;
    }
  }
}

// attack on the ship supplied, throw error if can't attack
// or made attacking the wrong target
void Warship::attack(shared_ptr<Ship> target_ptr_)
{
  if (!is_afloat()) 
    throw Error("Cannot attack!");
  else {
    if (target_ptr_ == shared_from_this()) {
      throw Error("Warship may not attack itself!");
    } else if (target_ptr_ == target.lock()){
      throw Error ("Already attacking this target!");
    } else {
      target = target_ptr_;
      warship_state = State::ATTACKING;
      cout << get_name() << " will attack " << target.lock()->get_name() << endl;
    }
  }
}

// stop attacking
void Warship::stop_attack()
{
  if (warship_state == State::NOT_ATTACKING) 
    throw Error("Was not attacking!");
  warship_state = State::NOT_ATTACKING;
  target.reset();
  cout << get_name() << " stopping attack" << endl;
}

// describe a warship's target if attacking
void Warship::describe() const
{
  Ship::describe();
  if (warship_state == State::ATTACKING) {
    if (target.expired() || !target.lock()->is_afloat()) {
      cout << "Attacking absent ship" << endl;
    } else {
      cout << "Attacking " << target.lock()->get_name() << endl;
    }
  }
}

// protected member functions

// return true if this Warship is in the attacking state
bool Warship::is_attacking() const
{
  return warship_state == State::ATTACKING;
}

// fire at the current target
void Warship::fire_at_target()
{
  cout << get_name() << " fires" << endl;
  target.lock()->receive_hit(firepower, shared_from_this());
}

// is the current target in range?
bool Warship::target_in_range() const
{
  return (cartesian_distance(get_location(), target.lock()->get_location()) <= maximum_range);
}

// get the target
shared_ptr<Ship> Warship::get_target() const
{
  return target.lock();
}
