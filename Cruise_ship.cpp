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
{}

bool Cruise_ship::is_cruising() const
{
  return cruise_state != State::NOT_CRUISING;
}

void Cruise_ship::set_destination_position_and_speed(Point destination_position, double speed)
{
  cancel_cruise();
  Ship::set_destination_position_and_speed(destination_position, speed);
  cruise_destination = Model::get_Instance().get_island_ptr(destination_position);
  // can start a cruise
  if (cruise_destination) {
    next_stop = cruise_destination;
    cruise_speed = speed;
    cout << get_name() << " will visit " << next_stop->get_name() << endl;
    cout << get_name() << " cruise will start and end at " << cruise_destination->get_name() << endl;
    cruise_state = State::TO_NEXT_STOP;
  }
}

void Cruise_ship::set_course_and_speed(double course, double speed)
{
  cancel_cruise();
  Ship::set_course_and_speed(course, speed);
}

void Cruise_ship::stop()
{
  cancel_cruise();
  Ship::stop();
}

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
        next_stop = Model::get_Instance().get_next_island(get_docked_Island()->get_location(), visited_islands);
        if (!next_stop) { // return to beginning
          next_stop = cruise_destination;
        }
        Ship::set_destination_position_and_speed(next_stop->get_location(), cruise_speed);
        visited_islands.insert(std::pair<string, shared_ptr<Island>>(next_stop->get_name(), next_stop));
        cruise_state = State::TO_NEXT_STOP;
        cout << get_name() << " will visit " << next_stop->get_name() << endl;
        break;
      }
      default:
        break;
    }
  }
}

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

void Cruise_ship::cancel_cruise()
{
  if (is_cruising()) {
    cout << get_name() << " canceling current cruise" << endl;
    cruise_end();
  }
}

void Cruise_ship::cruise_end()
{
  cruise_destination = nullptr;
  next_stop = nullptr;
  cruise_speed = 0;
  visited_islands.clear();
  cruise_state = State::NOT_CRUISING;
}
