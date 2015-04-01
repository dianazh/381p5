#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Views.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::map;
using std::vector;
using std::shared_ptr;

// create View object, run the program by acccepting user commands, then destroy View object
void Controller::run()
{
  // general commands for view and model
  map<string, void(Controller::*)()> commands = {
    {"status", &Controller::status}, 
    {"go", &Controller::go}, 
    {"create", &Controller::create}, 
    {"show", &Controller::show},
    {"open_map_view", &Controller::open_map_view}, 
    {"close_map_view", &Controller::close_map_view}, 
    {"open_sailing_view", &Controller::open_sailing_view}, 
    {"close_sailing_view", &Controller::close_sailing_view},
    {"open_bridge_view", &Controller::open_bridge_view}, 
    {"close_bridge_view", &Controller::close_bridge_view},
    {"default", &Controller::view_default}, 
    {"size", &Controller::view_size}, 
    {"zoom", &Controller::view_zoom}, 
    {"pan", &Controller::view_pan}
  };
  // ship commands
  map<string, void(Controller::*)(shared_ptr<Ship>)> ship_commands = {
    {"course", &Controller::ship_course}, {"position", &Controller::ship_position}, {"destination", &Controller::ship_destination},
    {"load_at", &Controller::ship_load_at}, {"unload_at", &Controller::ship_unload_at}, {"dock_at", &Controller::ship_dock_at},
    {"attack", &Controller::ship_attack}, {"refuel", &Controller::ship_refuel}, {"stop", &Controller::ship_stop}, 
    {"stop_attack", &Controller::ship_stop_attack}
  };
  string word;
  while (true) {
    cout << "\nTime " << Model::get_Instance().get_time() << ": Enter command: ";
    cin >> word;
    try {
      if (word == "quit") {
        quit();
        return;
      } else if (Model::get_Instance().is_ship_present(word)) {
        shared_ptr<Ship> ship = Model::get_Instance().get_ship_ptr(word);
        // expect ship command
        string instr;
        cin >> instr;
        auto fn = ship_commands.find(instr);
        if (fn != ship_commands.end()) {
          (this->*(fn->second))(ship);
        } else {
          throw Error("Unrecognized command!");
        }
      } else {
        // expect command for model or view
        auto fn = commands.find(word);
        if (fn != commands.end()) {
          (this->*(fn->second))();
        } else {
          throw Error("Unrecognized command!");
        }
      }
    } catch (Error& e) {
      cout << e.what() << endl;
      if (cin.fail()) cin.clear();
      while (cin.get() != '\n');
    } catch (std::exception& e2) {
      cout << e2.what() << endl;
      quit();
      return;
    }
  }
}

// handle status command for model
void Controller::status()
{
  Model::get_Instance().describe();
}

// handle go command for model
void Controller::go()
{
  Model::get_Instance().update();
}

// handle create command for model
void Controller::create()
{
  string ship_name;
  cin >> ship_name;
  if (ship_name.size() < 2) {
    throw Error("Name is too short!");
  }
  if (Model::get_Instance().is_name_in_use(ship_name)) {
    throw Error("Name is already in use!");
  }
  string type;
  cin >> type;
  Point point = get_Point();
  Model::get_Instance().add_ship(create_ship(ship_name, type, point));
}

// handle show command for model
void Controller::show()
{
  std::for_each(views.begin(), views.end(), std::mem_fn(&View::draw));
}

// handle open_map_view command for model
void Controller::open_map_view()
{
  if (map_view) throw Error("Map view is already open!");
  map_view.reset(new MapView());
  add_view(map_view);
}

// handle close_map_view command for model
void Controller::close_map_view()
{
  if (!map_view) throw Error("Map view is not open!");
  remove_view(map_view);
  map_view = nullptr;
}

// handle open_sailing_view command for model
void Controller::open_sailing_view()
{
  if (sailing_view) throw Error("Sailing data view is already open!");
  sailing_view.reset(new SailingDataView());
  add_view(sailing_view);
}

// handle close_sailing_view command for model
void Controller::close_sailing_view()
{
  if (!sailing_view) throw Error("Sailing data view is not open!");
  remove_view(sailing_view);
  sailing_view = nullptr;
}

// handle open_bridge_view command for model
void Controller::open_bridge_view()
{
  string ownship;
  cin >> ownship;
  shared_ptr<Ship> ship = Model::get_Instance().get_ship_ptr(ownship);
  if (bridge_views.find(ownship) != bridge_views.end())
    throw Error("Bridge view is already open for that ship!");
  shared_ptr<BridgeView> new_view(new BridgeView(ownship));
  bridge_views.insert(std::pair<string, shared_ptr<BridgeView>>(ownship, new_view));
  add_view(new_view);
}

// handle close_bridge_view command for model
void Controller::close_bridge_view()
{
  string ownship;
  cin >> ownship;
  auto target = bridge_views.find(ownship);
  if (target == bridge_views.end())
    throw Error("Bridge view for that ship is not open!");
  remove_view(target->second);
  bridge_views.erase(ownship);
}

// quit from the controller run
void Controller::quit()
{
  cout << "Done" << endl;
}

// helpers
void Controller::add_view(shared_ptr<View> view)
{
  views.push_back(view);
  Model::get_Instance().attach(view);
}

void Controller::remove_view(shared_ptr<View> view)
{
  Model::get_Instance().detach(view);
  views.erase(find(views.begin(), views.end(), view));
}

// get input point from user
Point Controller::get_Point()
{
  double x, y;
  cin >> x;
  if (!cin) throw Error("Expected a double!");
  cin >> y;
  if (!cin) throw Error("Expected a double!");
  return Point(x,y);
}

// get input speed from user
double Controller::get_speed()
{
  double speed;
  cin >> speed;
  if (!cin) throw Error("Expected a double!");
  if (speed < 0.0) {
    throw Error("Negative speed entered!");
  }
  return speed;
}

// get input island from user
shared_ptr<Island> Controller::get_island()
{
  string island_name;
  cin >> island_name;
  return Model::get_Instance().get_island_ptr(island_name);
}

// handle default command for view
void Controller::view_default()
{
  if (!map_view) throw Error("Map view is not open!");
  map_view->set_defaults(); 
}
 
// handle size command for view
void Controller::view_size()
{
  if (!map_view) throw Error("Map view is not open!");
  int size;
  cin >> size;
  if (!cin) throw Error("Expected an integer!");
  map_view->set_size(size);
}

// handle zoom command for view
void Controller::view_zoom()
{
  if (!map_view) throw Error("Map view is not open!");
  double scale;
  cin >> scale;
  if (!cin) throw Error("Expected a double!");
  map_view->set_scale(scale);
}

// handle pan command for view
void Controller::view_pan()
{
  if (!map_view) throw Error("Map view is not open!");
  Point point = get_Point();
  map_view->set_origin(point);
}

// handle course command for ship
void Controller::ship_course(shared_ptr<Ship> ship)
{
  double heading;
  cin >> heading;
  if (!cin) throw Error("Expected a double!");
  if (heading < 0.0 || heading >= 360.0) {
    throw Error("Invalid heading entered!");
  }
  double speed = get_speed();
  ship->set_course_and_speed(heading, speed);
}

// handle position command for ship
void Controller::ship_position(shared_ptr<Ship> ship)
{
  Point point = get_Point();
  double speed = get_speed();
  ship->set_destination_position_and_speed(point, speed);
}

// handle destination command for ship
void Controller::ship_destination(shared_ptr<Ship> ship)
{
  shared_ptr<Island> island = get_island();
  double speed = get_speed();
  ship->set_destination_position_and_speed(island->get_location(), speed);
}

// handle load_at command for ship
void Controller::ship_load_at(shared_ptr<Ship> ship)
{
  ship->set_load_destination(get_island());
}

// handle unload_at command for ship
void Controller::ship_unload_at(shared_ptr<Ship> ship)
{
  ship->set_unload_destination(get_island());
}

// handle dock_at command for ship
void Controller::ship_dock_at(shared_ptr<Ship> ship)
{
  ship->dock(get_island());
}

// handle attack command for ship
void Controller::ship_attack(shared_ptr<Ship> ship)
{
  string target_name;
  cin >> target_name;
  ship->attack(Model::get_Instance().get_ship_ptr(target_name));
}

// handle refuel command for ship
void Controller::ship_refuel(shared_ptr<Ship> ship)
{
  ship->refuel();
}

// handle stop command for ship
void Controller::ship_stop(shared_ptr<Ship> ship)
{
  ship->stop();
}

// handle stop_attack command for ship
void Controller::ship_stop_attack(shared_ptr<Ship> ship)
{
  ship->stop_attack();
}

