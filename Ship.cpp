#include "Ship.h"
#include "Island.h"
#include "Utility.h"
#include "Model.h"
#include <iostream>
using std::cout;
using std::endl;
using std::shared_ptr;

// initialize, then output constructor message
Ship::Ship(const std::string& name_, Point position_, double fuel_capacity_,
  double maximum_speed_, double fuel_consumption_, int resistance_)
  :Sim_object(name_), fuel(fuel_capacity_),
  fuel_consumption(fuel_consumption_), fuel_capacity(fuel_capacity_), 
  maximum_speed(maximum_speed_), resistance(resistance_), ship_state(State::STOPPED),
  docked_Island(nullptr)
{
  track_base.set_position(position_);
}
  
// Return true if ship can move (it is not dead in the water or in the process or sinking);
bool Ship::can_move() const
{
  return !(ship_state == State::SUNK || ship_state == State::DEAD_IN_THE_WATER);
}

// Return true if ship is moving;
bool Ship::is_moving() const
{
  return (ship_state == State::MOVING_TO_POSITION || ship_state == State::MOVING_ON_COURSE);
}

// Return true if ship is docked;
bool Ship::is_docked() const
{
  return (ship_state == State::DOCKED);
}

// Return true if ship is afloat (not in process of sinking), false if not
bool Ship::is_afloat() const
{
  return !(ship_state == State::SUNK);
}

// Return true if the ship is Stopped and the distance to the supplied island
// is less than or equal to 0.1 nm
bool Ship::can_dock(shared_ptr<Island> island_ptr) const
{
  return (ship_state == State::STOPPED && 
    cartesian_distance(get_location(), island_ptr->get_location()) <= 0.1);
}

// Update the state of the Ship
void Ship::update()
{
  cout << get_name();
  if (is_moving()) {
    calculate_movement();
    cout << " now at " << get_location();
  } else {
    switch (ship_state) {
      case State::STOPPED:
        cout << " stopped at " << get_location();
        break;
      case State::DOCKED:
        cout << " docked at " << docked_Island->get_name();
        break;
      case State::DEAD_IN_THE_WATER:
        cout << " dead in the water at " << get_location();
        break;
      case State::SUNK:
        cout << " sunk" << endl;
        break;
      default:
        break;
    }
  }
  cout << endl;
  broadcast_current_state();
}

// output a description of current state to cout
void Ship::describe() const
{
  // called by subclass
  cout << get_name() << " at " << get_location();
  if (ship_state == State::SUNK) {
    cout << " sunk" << endl;
  } else {
    cout << ", fuel: " << fuel << " tons, resistance: " << resistance << endl;
    switch (ship_state) {
      case State::MOVING_TO_POSITION:
        cout << "Moving to " << destination << " on " << track_base.get_course_speed() << endl;
        break;
      case State::MOVING_ON_COURSE:
        cout << "Moving on " << track_base.get_course_speed() << endl;
        break;
      case State::DOCKED:
        cout << "Docked at " << docked_Island->get_name() << endl;
        break;
      case State::STOPPED:
        cout << "Stopped" << endl;
        break;
      case State::DEAD_IN_THE_WATER:
        cout << "Dead in the water" << endl;
        break;
      default:
        break;
    }
  }
}

// let model notify all views of the ship's current state
void Ship::broadcast_current_state()
{
  Model::get_Instance().notify_location(get_name(), get_location());
  Model::get_Instance().notify_info(get_name(), "fuel", fuel);
  Model::get_Instance().notify_info(get_name(), "course", track_base.get_course());
  Model::get_Instance().notify_info(get_name(), "speed", track_base.get_speed());
}

// Start moving to a destination position at a speed
void Ship::set_destination_position_and_speed(Point destination_position, double speed)
{
  if (can_move()) {
    if (speed <= maximum_speed) {
      destination = destination_position;
      Compass_vector cv(get_location(), destination);
      track_base.set_course(cv.direction);
      track_base.set_speed(speed);
      if (ship_state == State::DOCKED) {
        docked_Island = nullptr;
      }
      ship_state = State::MOVING_TO_POSITION;
      cout << get_name() << " will sail on " << track_base.get_course_speed() 
        << " to " << destination << endl;
      broadcast_current_state();
    } else {
      throw Error("Ship cannot go that fast!");
    }
  } else {
    throw Error("Ship cannot move!");
  } 
}

// Start moving on a course and speed
void Ship::set_course_and_speed(double course, double speed)
{
  if (can_move()) {
    if (speed <= maximum_speed) {
      track_base.set_course(course);
      track_base.set_speed(speed);
      if (ship_state == State::DOCKED) {
        docked_Island = nullptr;
      }
      ship_state = State::MOVING_ON_COURSE; 
      cout << get_name() << " will sail on " << track_base.get_course_speed() << endl;
      broadcast_current_state();
    } else {
      throw Error("Ship cannot go that fast!");
    }
  } else {
    throw Error("Ship cannot move!");
  } 
}

// Stop moving
void Ship::stop()
{
  if (can_move()) {
    track_base.set_speed(0.);
    ship_state = State::STOPPED;
    cout << get_name() << " stopping at " << get_location() << endl; 
    broadcast_current_state();
  } else {
    throw Error("Ship cannot move!");
  }
}

// dock at an Island - set our position = Island's position, go into Docked state
void Ship::dock(shared_ptr<Island> island_ptr)
{
  if (can_dock(island_ptr)) {
    track_base.set_position(island_ptr->get_location());
    ship_state = State::DOCKED;
    cout << get_name() << " docked at " << island_ptr-> get_name() << endl;
    docked_Island = island_ptr;
    broadcast_current_state();
  } else {
    throw Error("Can't dock!");
  }
}

// Refuel - must already be docked at an island; fill takes as much as possible
void Ship::refuel()
{
  if (is_docked()) {
    double need = fuel_capacity - fuel;
    if (need < 0.005) fuel = fuel_capacity; 
    else {
      fuel += docked_Island->provide_fuel(need);
      cout << get_name() << " now has " << fuel << " tons of fuel" << endl;
    }
    broadcast_current_state();
  } else {
    throw Error("Must be docked!");
  }
}

// Fat interface command function - will throw error
void Ship::set_load_destination(shared_ptr<Island>)
{
  throw Error("Cannot load at a destination!");
}

// Fat interface command function - will throw error
void Ship::set_unload_destination(shared_ptr<Island>)
{
  throw Error("Cannot unload at a destination!");
}

// Fat interface command function - will throw error
void Ship::attack(shared_ptr<Ship> in_target_ptr)
{
  throw Error("Cannot attack!");
}

// Fat interface command function - will throw error
void Ship::stop_attack()
{
  throw Error("Cannot attack!");
}

// receive a hit from an attacker
// why attacker_ptr here?
// this is a partial fat interface for warship, so that warship can attack back
void Ship::receive_hit(int hit_force, shared_ptr<Ship> attacker_ptr) 
{
  resistance -= hit_force;
  cout << get_name() << " hit with " << hit_force << ", resistance now " << resistance << endl;
  if (resistance < 0) {
    cout << get_name() << " sunk" << endl;
    ship_state = State::SUNK;
    track_base.set_speed(0.);
    broadcast_current_state();
    Model::get_Instance().notify_gone(get_name());
    Model::get_Instance().remove_ship(shared_from_this());
  }
}

// protected member function
double Ship:: get_maximum_speed() const
{
  return maximum_speed;
}

// protected member function
shared_ptr<Island> Ship::get_docked_Island() const
{
  if (ship_state == State::DOCKED) return docked_Island;
  else return nullptr;
}

/* Private Function Definitions */

/*
Calculate the new position of a ship based on how it is moving, its speed, and
fuel state. This function should be called only if the state is 
MOVING_TO_POSITION or MOVING_ON_COURSE.

Track_base has an update_position(double time) function that computes the new position
of an object after the specified time has elapsed. If the Ship is going to move
for a full time unit (one hour), then it will get go the "full step" distance,
so update_position would be called with time = 1.0. If we can move less than that,
e.g. due to not enough fuel, update position  will be called with the corresponding
time less than 1.0.

For clarity in specifying the computation, this code assumes the specified private variable names, 
but you may change the variable names or enum class names, or state names if you wish (e.g. movement_state).
*/
void Ship:: calculate_movement()
{
  // Compute values for how much we need to move, and how much we can, and how long we can,
  // given the fuel state, then decide what to do.
  double time = 1.0;  // "full step" time
  // get the distance to destination
  double destination_distance = cartesian_distance(get_location(), destination);
  // get full step distance we can move on this time step
  double full_distance = track_base.get_speed() * time;
  // get fuel required for full step distance
  double full_fuel_required = full_distance * fuel_consumption;  // tons = nm * tons/nm
  // how far and how long can we sail in this time period based on the fuel state?
  double distance_possible, time_possible;
  if(full_fuel_required <= fuel) {
    distance_possible = full_distance;
    time_possible = time;
  } else {
    distance_possible = fuel / fuel_consumption;  // nm = tons / tons/nm
    time_possible = (distance_possible / full_distance) * time;
  }
  
  // are we are moving to a destination, and is the destination within the distance possible?
  if(ship_state == State::MOVING_TO_POSITION && destination_distance <= distance_possible) {
    // yes, make our new position the destination
    track_base.set_position(destination);
    // we travel the destination distance, using that much fuel
    double fuel_required = destination_distance * fuel_consumption;
    fuel -= fuel_required;
    track_base.set_speed(0.);
    ship_state = State::STOPPED;
  } else {
    // go as far as we can, stay in the same movement state
    // simply move for the amount of time possible
    track_base.update_position(time_possible);
    // have we used up our fuel?
    if(full_fuel_required >= fuel) {
      fuel = 0.0;
      track_base.set_speed(0.);
      ship_state = State::DEAD_IN_THE_WATER;
    } else {
      fuel -= full_fuel_required;
    }
  }
}
