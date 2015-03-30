#include "Model.h"
#include "Sim_object.h"
#include "Island.h"
#include "Ship.h"
#include "View.h"
#include "Geometry.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <algorithm>
#include <functional>
#include <iostream>
using std::map;
using std::vector;
using std::string;
using std::for_each;
using std::find;
using std::mem_fn;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using namespace std::placeholders;

Model& Model::get_Instance()
{
  static Model model;
  return model;
}

Model::~Model()
{}

// create the initial objects, output constructor message
Model::Model()
  :time(0)
{
  insert_island(shared_ptr<Island>(new Island ("Exxon", Point(10, 10), 1000, 200)));
  insert_island(shared_ptr<Island>(new Island ("Shell", Point(0, 30), 1000, 200)));
  insert_island(shared_ptr<Island>(new Island ("Bermuda", Point(20, 20))));
  insert_island(shared_ptr<Island>(new Island ("Treasure_Island", Point(50, 5), 100, 5)));
 
  insert_ship(create_ship("Ajax", "Cruiser", Point (15, 15)));
  insert_ship(create_ship("Xerxes", "Cruiser", Point (25, 25)));
  insert_ship(create_ship("Valdez", "Tanker", Point (30, 30)));
}

// is name already in use for either ship or island?
// either the identical name, or identical in first two characters counts as in-use
bool Model::is_name_in_use(const string& name) const
{
  string starting_two = name.substr(0, 2); //should have no problem since Controller checked size() >= 2
  for (auto ptr : sim_objects) {
    if (!ptr.second->get_name().compare(0, 2, starting_two)) {
      return true;
    }
  }
  return false;
}

// is there such an island?
bool Model::is_island_present(const string& name) const
{
  if (islands.find(name) != islands.end()) {
      return true;
  }
  return false;
}

// will throw Error("Island not found!") if no island of that name
shared_ptr<Island> Model::get_island_ptr(const string& name) const
{
  auto it = islands.find(name);
  if (it != islands.end()) {
    return it->second; 
  }
  throw Error("Island not found!");
}

// get the island of that position 
std::shared_ptr<Island> Model::get_island_ptr(const Point& point) const
{
  for (auto it : islands) {
    if (it.second->get_location() == point) {
      return it.second; 
    }
  }
  return nullptr;
}

std::shared_ptr<Island> Model::get_next_island(const Point& point, const std::map<std::string, std::shared_ptr<Island>>& visited) const 
{
  double shortest = -1;
  shared_ptr<Island> next_stop;
  for (auto it : islands) {
    if (visited.find(it.first) == visited.end()) {
      double distance = cartesian_distance(it.second->get_location(), point);
      if (distance < shortest || shortest < 0) {
        shortest = distance;
        next_stop = it.second;
      }
    }
  }
  return next_stop;
}

// is there such an ship?
bool Model::is_ship_present(const string& name) const
{
  if (ships.find(name) != ships.end()) {
      return true;
  }
  return false;
}

// add a new ship to the list, and update the view
void Model::add_ship(shared_ptr<Ship> new_ship)
{
  insert_ship(new_ship);
  // update the view
  new_ship->broadcast_current_state();
}

// remove the Ship from the containers
void Model::remove_ship(shared_ptr<Ship> ship_ptr)
{
  sim_objects.erase(ship_ptr->get_name());
  ships.erase(ship_ptr->get_name());
}

// will throw Error("Ship not found!") if no ship of that name
shared_ptr<Ship> Model::get_ship_ptr(const string& name) const
{
  auto it = ships.find(name);
  if (it != ships.end()) {
    return it->second;  
  }
  throw Error("Ship not found!");
}

// tell all objects to describe themselves
void Model::describe() const
{
  for_each(sim_objects.begin(), sim_objects.end(),
      bind(&Sim_object::describe, 
          bind(&map<string, shared_ptr<Sim_object>>::value_type::second, _1)));
}

// increment the time, and tell all objects to update themselves
void Model::update()
{
  ++time;
  for_each(sim_objects.begin(), sim_objects.end(),
      bind(&Sim_object::update, 
          bind(&map<string, shared_ptr<Sim_object>>::value_type::second, _1)));
}

// Attaching a View adds it to the container and causes it to be updated
// with all current objects'location (or other state information.
void Model::attach(shared_ptr<View> new_view)
{
  views.push_back(new_view);
  for_each(sim_objects.begin(), sim_objects.end(), 
      bind(&Sim_object::broadcast_current_state, 
          bind(&map<string, shared_ptr<Sim_object>>::value_type::second, _1)));
}

// Detach the View by discarding the supplied pointer from the container of Views
// - no updates sent to it thereafter.
void Model::detach(shared_ptr<View> view_ptr)
{
  views.erase(find(views.begin(), views.end(), view_ptr)); //no need to delete the obj
}

// notify the views about an object's location
void Model::notify_location(const string& name, Point location)
{
  for_each(views.begin(), views.end(), bind(&View::update_location, _1, std::ref(name), std::ref(location)));
}

void Model::notify_ship_info(const std::string& name, double fuel, double course, double speed)
{
  for_each(views.begin(), views.end(), bind(&View::update_ship_info, _1, std::ref(name), fuel, course, speed));
}

// notify the views that an object is now gone
void Model::notify_gone(const string& name)
{
  for_each(views.begin(), views.end(), bind(&View::update_remove, _1, std::ref(name)));
}

// insert an island to its containers
void Model::insert_island(shared_ptr<Island> island_ptr) 
{
  sim_objects.insert(std::pair<string, shared_ptr<Sim_object>>(island_ptr->get_name(), island_ptr));
  islands.insert(std::pair<string, shared_ptr<Island>>(island_ptr->get_name(), island_ptr));
}

// insert a ship to its containers
void Model::insert_ship(shared_ptr<Ship> ship_ptr) 
{
  sim_objects.insert(std::pair<string, shared_ptr<Sim_object>>(ship_ptr->get_name(), ship_ptr));
  ships.insert(std::pair<string, shared_ptr<Ship>>(ship_ptr->get_name(), ship_ptr));
}
