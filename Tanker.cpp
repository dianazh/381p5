#include "Tanker.h"
#include "Island.h"
#include "Utility.h"
#include <iostream>
using std::cout;
using std::endl;
using std::shared_ptr;

const double Tanker::FUEL_CAPACITY = 100.0;
const double Tanker::MAX_SPEED = 10.0;
const double Tanker::FUEL_CONSUMPTION = 2.0;
const int Tanker::RESISTANCE = 0;
const double Tanker::CARGO_CAPACITY = 1000.0;
const double Tanker::INIT_CARGO = 0.0;

// initialize, the output constructor message
Tanker::Tanker(const std::string& name_, Point position_)
  :Ship(name_, position_, FUEL_CAPACITY, MAX_SPEED, FUEL_CONSUMPTION, RESISTANCE),
  cargo(INIT_CARGO), cargo_capacity(CARGO_CAPACITY),
  tanker_state(State::NO_CARGO_DESTINATIONS),
  load_destination(nullptr), unload_destination(nullptr) //NOTE: EXPLICIT INIT NOT NECESSARY
{}

// check if this Tanker has assigned cargo destinations, if yes, throw error
void Tanker::set_destination_position_and_speed(Point destination, double speed)
{
  if (tanker_state == State::NO_CARGO_DESTINATIONS) {
    Ship::set_destination_position_and_speed(destination, speed);
  } else {
    throw Error("Tanker has cargo destinations!"); //NOTE: BETTER DEFINE AS A CONST, BUT NO NEED TO DEFINE A FUNCTION
  }
}

// check if this Tanker has assigned cargo destinations, if yes, throw error
void Tanker::set_course_and_speed(double course, double speed)
{
  if (tanker_state == State::NO_CARGO_DESTINATIONS) {
    Ship::set_course_and_speed(course, speed);
  } else {
    throw Error("Tanker has cargo destinations!");
  }
}

// Set the loading and unloading Island destinations
// if both cargo destination are already set, throw Error("Tanker has cargo destinations!").
// if they are the same, leave at the set values, and throw Error("Load and unload cargo destinations are the same!")
// if both destinations are now set, start the cargo cycle
void Tanker::set_load_destination(shared_ptr<Island> island_ptr)
{
  if (tanker_state == State::NO_CARGO_DESTINATIONS) {
    load_destination = island_ptr;
    if (load_destination == unload_destination) {
      throw Error("Load and unload cargo destinations are the same!");
    }
    cout << get_name() << " will load at " << island_ptr->get_name() << endl;
    start_cycle_if_appropriate();
  } else {
    throw Error("Tanker has cargo destinations!");
  }
}

// see as load_destination
void Tanker::set_unload_destination(shared_ptr<Island> island_ptr)
{
  if (tanker_state == State::NO_CARGO_DESTINATIONS) {
    unload_destination = island_ptr;
    if (unload_destination == load_destination) {
      throw Error("Load and unload cargo destinations are the same!");
    }
    cout << get_name() << " will unload at " << island_ptr->get_name() << endl;
    start_cycle_if_appropriate();
  } else {
    throw Error("Tanker has cargo destinations!");
  }
}

// when told to stop, clear the cargo destinations and stop
void Tanker::stop()
{
  Ship::stop();
  tanker_stop();
}

// update a Tanker
void Tanker::update()
{
  Ship::update();
  if (!can_move()) {
    tanker_stop(); //NOTE: USE TANKER::STOP() IS ENOUGH
  } else {
    switch (tanker_state) {
      case State::MOVING_TO_LOADING:
        if (!is_moving() && can_dock(load_destination)) {
          dock(load_destination);
          tanker_state = State::LOADING;
        }
        break;
      case State::MOVING_TO_UNLOADING:
        if (!is_moving() && can_dock(unload_destination)) {
          dock(unload_destination);
          tanker_state = State::UNLOADING;
        }
        break;
      case State::LOADING:
      {
        refuel();
        double need = cargo_capacity - cargo;
        if (need < 0.005) {
          cargo = cargo_capacity;
          Ship::set_destination_position_and_speed(unload_destination->get_location(), MAX_SPEED);
          tanker_state = State::MOVING_TO_UNLOADING;
        } else {
          cargo += load_destination->provide_fuel(need);
          cout << get_name() << " now has " << cargo << " of cargo" << endl;
        }
        break;
      }
      case State::UNLOADING:
      {
        if (cargo == 0.0) {
          Ship::set_destination_position_and_speed(load_destination->get_location(), MAX_SPEED);
          tanker_state = State::MOVING_TO_LOADING;
        } else {
          unload_destination->accept_fuel(cargo);
          cargo = 0.0;
        }
        break;
      }
      default:
        break;
    }
  }
}

// describe a Tanker
void Tanker::describe() const
{
  cout << "\nTanker ";
  Ship::describe();
  cout << "Cargo: " << cargo << " tons";
  switch (tanker_state) {
    case State::NO_CARGO_DESTINATIONS:
      cout << ", no cargo destinations" << endl;
      break;
    case State::LOADING:
      cout << ", loading" << endl;
      break;
    case State::UNLOADING:
      cout << ", unloading" << endl;
      break;
    case State::MOVING_TO_LOADING:
      cout << ", moving to loading destination" << endl;
      break;
    case State::MOVING_TO_UNLOADING:
      cout << ", moving to unloading destination" << endl;
      break;
    default:
      break;
  } 
}

//see if both destinations for a cycle is set
//if yes, start the cycle by finding the appropriate state
void Tanker::start_cycle_if_appropriate() // NOTE: NEED FLAT OUT, AVOID NESTED IF
{
  if (unload_destination && load_destination) {
    if (is_docked()) {
      if (get_docked_Island() == load_destination) {
        tanker_state = State::LOADING;
        return;
      } else if (get_docked_Island() == unload_destination){
        tanker_state = State::UNLOADING;
        return;
      }
    } 
    if (!is_moving()) {
      if (cargo == 0 && can_dock(load_destination)) {
        dock(load_destination);
        tanker_state = State::LOADING;
        return;
      } else if (cargo > 0 && can_dock(unload_destination)) {
        dock(unload_destination);
        tanker_state = State::UNLOADING;
        return;
      }
    }
    if (cargo == 0) {
      tanker_state = State::MOVING_TO_LOADING;
      Ship::set_destination_position_and_speed(load_destination->get_location(), MAX_SPEED);
      return;
    } else if (cargo > 0){
      tanker_state = State::MOVING_TO_UNLOADING;
      Ship::set_destination_position_and_speed(unload_destination->get_location(), MAX_SPEED);
      return;
    }
  } 
}

// stop the tanker, forget both destinations
void Tanker::tanker_stop()
{
  load_destination = nullptr;
  unload_destination = nullptr;
  tanker_state = State::NO_CARGO_DESTINATIONS;
  cout << get_name() << " now has no cargo destinations" << endl;
}
