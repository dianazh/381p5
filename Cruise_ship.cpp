#include "Cruise_ship.h"
#include "Model.h"
#include "Island.h"
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;

const double Cruise_ship::FUEL_CAPACITY = 500.0;
const double Cruise_ship::MAX_SPEED = 15.0;
const double Cruise_ship::FUEL_CONSUMPTION = 2.0;
const int Cruise_ship::RESISTANCE = 0;

Cruise_ship::Cruise_ship(const std::string& name_, Point position_)
  :Ship(name_, position_, FUEL_CAPACITY, MAX_SPEED, FUEL_CONSUMPTION, RESISTANCE), 
    cruise_destination(nullptr), cruise_state(State::NOT_CRUISING)
{
  all_islands =  Model::get_Instance().get_islands();
}

// return true if status != not_cruising
bool Cruise_ship::is_cruising() const
{
  return cruise_state != State::NOT_CRUISING;
}

// if the destination is an island, go to that island and start the cruise. Otherwise, work as
// normal ships do.
void Cruise_ship::set_destination_position_and_speed(Point destination_position, double speed)
{
  cancel_cruise();
  Ship::set_destination_position_and_speed(destination_position, speed);
  cruise_destination = get_island(destination_position);
  // can start a cruise
  if (cruise_destination) {
    next_stop = cruise_destination;
    cruise_speed = speed;
    cout << get_name() << " will visit " << next_stop->get_name() << endl;
    cout << get_name() << " cruise will start and end at " << cruise_destination->get_name() << endl;
    cruise_state = State::TO_NEXT_STOP;
  }
}

// stops the cruise trip if is cruising
void Cruise_ship::set_course_and_speed(double course, double speed)
{
  cancel_cruise();
  Ship::set_course_and_speed(course, speed);
}

// stops the cruise trip if is cruising
void Cruise_ship::stop()
{
  cancel_cruise();
  Ship::stop();
}

// update cruise state according to current state
void Cruise_ship::update()
{
  Ship::update();
  if (!can_move()) {
    cancel_cruise(); 
  } else {
    switch (cruise_state) {
      case State::TO_NEXT_STOP:
        if (!is_moving() && can_dock(next_stop)) {
          dock(next_stop);
          if (next_stop == cruise_destination && !visited_islands.empty()){
            cout << get_name() << " cruise is over at " << cruise_destination->get_name() << endl;
            cruise_end();
          } else {
            cruise_state = State::REFUEL;
            visited_islands.insert(std::pair<string, shared_ptr<Island>>(next_stop->get_name(), next_stop));
          }
        }
        break;
      case State::REFUEL:
      {
        refuel();
        cruise_state = State::WAIT;
        break;
      }
      case State::WAIT:
      {
        cruise_state = State::SET_COURSE;
        break;
      }
      case State::SET_COURSE:
      {
        next_stop = get_next_stop();
        if (!next_stop) { // return to beginning
          next_stop = cruise_destination;
        }
        Ship::set_destination_position_and_speed(next_stop->get_location(), cruise_speed);
        //visited_islands.insert(std::pair<string, shared_ptr<Island>>(next_stop->get_name(), next_stop));
        cruise_state = State::TO_NEXT_STOP;
        cout << get_name() << " will visit " << next_stop->get_name() << endl;
        break;
      }
      default:
        break;
    }
  }
}

// output information about the current state
void Cruise_ship::describe() const
{
  cout << "\nCruise_ship ";
  Ship::describe();
  if (is_cruising()) {
    if (cruise_state == State::TO_NEXT_STOP){
        cout << "On cruise to " << next_stop->get_name() << endl;
    } else {
        cout << "Waiting during cruise at " << next_stop->get_name() << endl;
    }
  }
}

// will return nullptr if no island is at the point
shared_ptr<Island> Cruise_ship::get_island(Point point) const
{
  for (auto it : all_islands) {
    if (it->get_location() == point) {
      return it;
    }
  }
  return nullptr;
}

// will return nullptr if no island is not visited
shared_ptr<Island> Cruise_ship::get_next_stop() const
{
  Point point = get_docked_Island()->get_location();
  double shortest = -1;
  shared_ptr<Island> next_stop;
  for (auto it : all_islands) {
    if (visited_islands.find(it->get_name()) == visited_islands.end()) {
      double distance = cartesian_distance(it->get_location(), point);
      if (distance < shortest || shortest < 0) {
         shortest = distance;
         next_stop = it;
       } 
     } 
   } 
   return next_stop;
}

// cancel a cruise if in a cruise, print end msg
void Cruise_ship::cancel_cruise()
{
  if (is_cruising()) {
    cout << get_name() << " canceling current cruise" << endl;
    cruise_end();
  }
}

// cancel a cruise, reset all the values
void Cruise_ship::cruise_end()
{
  cruise_destination = nullptr;
  next_stop = nullptr;
  cruise_speed = 0;
  visited_islands.clear();
  cruise_state = State::NOT_CRUISING;
}
