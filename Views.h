#ifndef VIEWS_H
#define VIEWS_H
#include "View.h"
#include <map>

class MapView : public View {
public:
  MapView();
  // get all objects' location
  void update_location(const std::string& name, Point location) override;
  // remove an object
  void update_remove(const std::string& name) override;
  void draw() override;
  void clear() override;
  
  // modify the display parameters
  // if the size is out of bounds will throw Error("New map size is too big!")
  // or Error("New map size is too small!")
  void set_size(int size_);
  // If scale is not postive, will throw Error("New map scale must be positive!");
  void set_scale(double scale_);
  // any values are legal for the origin
  void set_origin(Point origin_);
  // set the parameters to the default values
  void set_defaults();

private:
  // Calculate the cell subscripts corresponding to the location parameter, using the 
  // current size, scale, and origin of the display. 
  // Return true if the location is within the map, false if not
  bool get_subscripts(int &ix, int &iy, Point location);

  static const int DEFAULT_SIZE;
  static const double DEFAULT_SCALE;
  static const Point DEFAULT_ORIGIN;
  static const std::string MULTIPLE; // for drawing
  static const std::string EMPTY; //for drawing

  std::map<std::string, Point> objects; //ordered list of objects remembered
  int size;      // current size of the display
  double scale;    // distance per cell of the display
  Point origin;    // coordinates of the lower-left-hand corner
};

class SailingDataView : public View {
public:
  // update ship's speed 
  void update_ship_speed(const std::string& name, double value);
  // update ship's course 
  void update_ship_course(const std::string& name, double value);
  // update ship's fuel
  void update_ship_fuel(const std::string& name, double value);
  // remove an object
  void update_remove(const std::string& name) override;
  void draw() override;
  void clear() override;
  
private:
  static const int WIDTH; // default width for view entries
  struct ShipInfo {
    double fuel, speed, course;
  };
  std::map<std::string, ShipInfo> objects; //ordered list of ship objects remembered
};

class BridgeView : public View {
public:
  // initialize with ownship's name
  BridgeView(std::string ownship_);
  // get all objects' location
  void update_location(const std::string& name, Point location) override;
  // get the course of ownship 
  void update_ship_course(const std::string& name, double value) override;
  // if the removed ship is ownship, memorize the sunk_point before removing it
  void update_remove(const std::string& name) override;
  void draw() override;
  void clear() override;

private:
  // Calculate the cell subscripts corresponding to the location parameter, using the 
  // default size, scale, and origin of the display. 
  // Return true if the location is within the map, false if not
  bool get_subscripts(int &ix, int &iy, Point location);

  static const double FULL_ANGLE, HALF_ANGLE;
  static const int X_SIZE;
  static const int Y_SIZE;
  static const double DEFAULT_SCALE;
  static const Point PLOT_ORIGIN;
  static const std::string MULTIPLE; // for drawing
  static const std::string EMPTY; //for drawing
  static const std::string WATER; //for drawing

  std::map<std::string, Point> objects; //ordered list of objects remembered
  std::string ownship;
  double ownship_course;
  Point ownship_sunk_point;
};

#endif
