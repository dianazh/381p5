/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <memory>
#include <map>
#include <vector>

//class Model; //pending
class View;
class MapView;
class SailingDataView;
class BridgeView;
class Ship;
class Island;
class Point;

class Controller {
public:  
  // create View object, run the program by acccepting user commands, then destroy View object
  void run();

private:
  std::shared_ptr<MapView> map_view; //ptr to the only map view
  std::shared_ptr<SailingDataView> sailing_view; //ptr to the only sailing view
  // map of ownship_name, bridge_view_ptr of the ownship
  std::map<std::string, std::shared_ptr<BridgeView>> bridge_views;
  // list of views in constructing order 
  std::vector<std::shared_ptr<View>> views;

  //helper
  // add & remove view from controller and model's list
  void add_view(std::shared_ptr<View> view);
  void remove_view(std::shared_ptr<View> view);
  // get input point from user
  Point get_Point();
  // get input speed from user
  double get_speed();
  // get input island from user
  std::shared_ptr<Island> get_island();

  // command handler
  // handle status command for model
  void status();
  // handle go command for model
  void go();
  // handle create command for model
  void create();
  // handle show command for model
  void show();
  // handle open_map_view command for model
  void open_map_view();
  // handle close_map_view command for model
  void close_map_view();
  // handle open_sailing_view command for model
  void open_sailing_view();
  // handle close_sailing_view command for model
  void close_sailing_view();
  // handle open_bridge_view command for model
  void open_bridge_view();
  // handle close_bridge_view command for model
  void close_bridge_view();
  // quit from the controller run
  void quit();
  // handle default command for view
  void view_default();
  // handle size command for view
  void view_size();
  // handle zoom command for view
  void view_zoom();
  // handle pan command for view
  void view_pan();
  // handle course command for ship
  void ship_course(std::shared_ptr<Ship> ship);
  // handle position command for ship
  void ship_position(std::shared_ptr<Ship> ship);
  // handle destination command for ship
  void ship_destination(std::shared_ptr<Ship> ship);
  // handle load_at command for ship
  void ship_load_at(std::shared_ptr<Ship> ship);
  // handle unload_at command for ship
  void ship_unload_at(std::shared_ptr<Ship> ship);
  // handle dock_at command for ship
  void ship_dock_at(std::shared_ptr<Ship> ship);
  // handle attack command for ship
  void ship_attack(std::shared_ptr<Ship> ship);
  // handle refuel command for ship
  void ship_refuel(std::shared_ptr<Ship> ship);
  // handle stop command for ship
  void ship_stop(std::shared_ptr<Ship> ship);
  // handle stop_attack command for ship
  void ship_stop_attack(std::shared_ptr<Ship> ship);
};
#endif
