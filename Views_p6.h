#include "View_p5.h"
#include "Geometry.h"

class GridView : public View{
public:
  GridView(int x_bound, int y_bound, double scale, Point origin); // provent from being initialized
  // update the location of name in objects
  void update_location(const std::string& name, Point location);
  // remove the name from objects
  void update_remove(const std::string& name) override;
  // draw grid view
    // init emtpy arr
    // call draw_header initialize empty arr
    // fill arr: call get_draw_position, if true, call get_subscripts & fill the arr
    // save the precision
    // draw the grid: form the loop, call draw_y_grid(), call draw_x_grid()
    // restore the precision
  void draw() override;

protected:

  // fat interface for draw()
  // if ownship is sunk in bridgeView, fill with w-; list of outsider in MapView
    // call get_draw_point(Point in_)
    // call get_subscripts, init ix, iy
  virtual void draw_header_init_arr(vector<vector<std::string>>& arr) = 0;
  // calculate the point to draw
  // return false if point should not be drawn
  virtual bool get_draw_location(Point& out, Point in) = 0;
  // return the string indicating multiple
  virtual std::string get_multiple_str() const = 0;
  virtual void draw_y_grid(int curr_y) = 0;
  virtual void draw_x_grid() = 0;

  // interface for MapView to set the parameters 
  void set_x_bound(int x_);
  void set_y_bound(int y_);
  void set_origin(Point origin_);
  void set_scale(double scale);

private:
  double scale;
  int x_bound, y_bound; // boundary of the grid on x, y direction
  Point origin;
  std::map<std::string, Point> objects;

  
  // get the subscript for the given location in (x,y)
  // return false if location is not in the current range
  // calls get_draw_location
  bool get_subscripts(int& x, int& y, Point location);
};

class MapView : public GridView{
public:
  MapView();
  // init to empty
  void draw_header_init_arr(vector<vector<std::string>>& arr) override;
  // return the original point to out, return true all the time
  bool get_draw_location(Point& out, Point in) override;
  std::string get_multiple_str() const override;
  void draw_y_grid(int curr_y) override;
  void draw_x_grid() override;
  
  void set_size(int size_);
  void set_scale(double scale);
  void set_origin(Point origin_);
  void set_defaults();
  
private:
  static const int default_size_c;
  static const double default_scale_c;
  static const Point default_origin_c;
  static const char* empty_c; // filling string for empty grid
  static const char* multiple_c; // filling string for multiple grid
};

class SailingView : public View {
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
  
private:
  static const int WIDTH; // default width for view entries
  struct ShipInfo {
    double fuel, speed, course;
  };
  std::map<std::string, ShipInfo> objects; //ordered list of ship objects remembered
};

class BridgeView : public GridView {
public:
  BridgeView(const std::string& ownship_);
  void update_ship_course(const std::string& name, double value) override;
  // mark the ownship sunk
  void update_remove(const std::string& name) override;
  // init to empty or water
  void draw_header_init_arr(vector<vector<std::string>>& arr) override;
  // return the point after calculating AoB, return false if ship is not in range [0.005, 20] or if ownship sunk
  bool get_draw_location(Point& out, Point in) override;
  std::string get_multiple_str() const override;
  void draw_y_grid(int curr_y) override;
  void draw_x_grid() override;
  
private:
  std::string ownship;
  bool is_ownship_sunk;
  Point ownship_position;
  
  static const int x_bound_c;
  static const int y_bound_c;
  static const double default_scale_c;
  static const Point plot_origin_c;
  static const char* empty_c; // filling string for empty grid
  static const char* multiple_c; // filling string for multiple grid
  static const char* water_c; // filling string for water grid
};
