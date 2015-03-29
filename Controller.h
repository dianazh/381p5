 /* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

//class Model; //pending
class View;
class Ship;
class Island;
class Point;

class Controller {
public:	
	// output constructor message
	Controller();
	// output destructor message
	~Controller();

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();

private:
  //helper
  // quit from the controller run
  void quit(View*);
  // get input point from user
  Point get_Point();
  // get input speed from user
  double get_speed();
  // get input island from user
  Island* get_island();
  // command handler
  // handle size command for view
  void view_size(View* view);
  // handle zoom command for view
  void view_zoom(View* view);
  // handle pan command for view
  void view_pan(View* view);
  // handle create command for model
  void model_create();
  // handle course command for ship
  void ship_course(Ship* ship);
  // handle position command for ship
  void ship_position(Ship* ship);
  // handle destination command for ship
  void ship_destination(Ship* ship);
  // handle load_at command for ship
  void ship_load_at(Ship* ship);
  // handle unload_at command for ship
  void ship_unload_at(Ship* ship);
  // handle dock_at command for ship
  void ship_dock_at(Ship* ship);
  // handle attack command for ship
  void ship_attack(Ship* ship);
  // handle refuel command for ship
  void ship_refuel(Ship* ship);
  // handle stop command for ship
  void ship_stop(Ship* ship);
  // handle stop_attack command for ship
  void ship_stop_attack(Ship* ship);
};
#endif
