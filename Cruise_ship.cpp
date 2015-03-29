#include "Cruise_ship.h"

const double Cruise_ship::FUEL_CAPACITY = 500.0;
const double Cruise_ship::MAX_SPEED = 15.0;
const double Cruise_ship::FUEL_CONSUMPTION = 2.0;
const int Cruise_ship::RESISTANCE = 0;

Cruise_ship::Cruise_ship(const std::string& name_, Point position_)
  :Ship(name_, position_, FUEL_CAPACITY, MAX_SPEED, FUEL_CONSUMPTION, RESISTANCE), destination(nullptr), cruise_state(State::NOT_CRUISING)
{}

void Cruise_ship::set_destination_position_and_speed(Point destination_position, double speed)
{
  if (cruise_state == State::NOT_CRUISING) {
    
  }
  Ship::set_destination_position_and_speed(destination_postion, speed);
}

void Cruise_ship::set_cruise_destination(shared_ptr<Island> island, double speed)
{
  Ship::set_destination_position_and_speed(island->get_location(), speed);
  cout << get_name() << " will visit " << island->get_name() << endl;
  cout << get_name() << " cruise will start and end at " << island->get_name() << endl;
}

void Cruise_ship::describe()
{
  cout << "\nCruise_ship ";
  Ship::describe();
  cout << 
}
