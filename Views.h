#ifndef VIEWS_H
#define VIEWS_H
#include "View.h"
#include <map>

class PositionView : public View {
public:
  void update_location(const std::string& name, Point location) override;
  void update_remove(const std::string& name) override;
  void clear() override;
  // Calculate the cell subscripts corresponding to the location parameter, using the 
  // current size, scale, and origin of the display. 
  // Return true if the location is within the map, false if not
  bool get_subscripts(int &ix, int &iy, Point location); 
  
protected:
  PositionView(int size_, double scale_, Point origin_); //prevent client from initializing
  int size;      // current size of the display
  double scale;    // distance per cell of the display
  Point origin;    // coordinates of the lower-left-hand corner
  std::map<std::string, Point> objects; //ordered list of objects remembered
};

class MapView : public PositionView {
public:
  MapView();
  void draw() override;
  
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
  static const int DEFAULT_SIZE;
  static const double DEFAULT_SCALE;
  static const Point DEFAULT_ORIGIN;
  static const std::string MULTIPLE; // for drawing
  static const std::string EMPTY; //for drawing
};

class SailingDataView : public View {
public:
  void update_info(const std::string& name, const std::string& info_name, double info_value) override;
  void update_remove(const std::string& name) override;
  void draw() override;
  void clear() override;

private:
  static const int WIDTH;
  std::map<std::string, std::map<std::string, double>> objects; //ordered list of ship objects remembered
};

class BridgeView : public PositionView {
public:
  // initialize with ownship's name
  BridgeView(std::string ownship_);
  // get the course of ownship when given ship info
  void update_info(const std::string& name, const std::string& info_name, double info_value) override;
  // if the removed ship is ownship, memorize the sunk_point before removing it
  void update_remove(const std::string& name) override;
  void draw() override;

private:
  static const double FULL_ANGLE, HALF_ANGLE;
  static const int X_SIZE;
  static const int Y_SIZE;
  static const double DEFAULT_SCALE;
  static const Point PLOT_ORIGIN;
  static const std::string MULTIPLE; // for drawing
  static const std::string EMPTY; //for drawing
  static const std::string WATER; //for drawing

  std::string ownship;
  double ownship_course;
  Point ownship_sunk_point;
};

#endif
