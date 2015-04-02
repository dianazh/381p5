#include "Views.h"
#include "Navigation.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
using std::cout;
using std::endl;
using std::setw;
using std::string;
using std::vector;
using std::map;


const int MapView::DEFAULT_SIZE = 25;
const double MapView::DEFAULT_SCALE = 2.0;
const Point MapView::DEFAULT_ORIGIN = Point(-10, -10);
const string MapView::MULTIPLE = "* ";
const string MapView::EMPTY = ". ";

// default constructor sets the default size, scale, and origin, outputs constructor message
MapView::MapView()
  :size(DEFAULT_SIZE), scale(DEFAULT_SCALE), origin(DEFAULT_ORIGIN)
{}

// Save the supplied name and location for future use in a draw() call
// If the name is already present,the new location replaces the previous one.
void MapView::update_location(const std::string& name, Point location)
{
  auto pair = objects.find(name);
  if (pair == objects.end()) {
    objects.insert(std::pair<string, Point>(name, location));
  } else {
    pair->second = location;
  }
}

// Remove the name and its location; no error if the name is not present.
void MapView::update_remove(const std::string& name)
{
  objects.erase(name);
}

// prints out the current map
void MapView::draw()
{
  cout << "Display size: " << size << ", scale: " << scale << ", origin: " << origin << endl;
  bool has_outranger = false;
  vector<vector<string>> arr (size, vector<string>(size, EMPTY));
  // in alphabetical order
  for (auto obj : objects) {
    int ix, iy;
    if (get_subscripts(ix, iy, obj.second)) {
      if (arr[ix][iy] != EMPTY){
        arr[ix][iy] = MULTIPLE; 
      } else {
        arr[ix][iy] = obj.first.substr(0,2);
      }
    } else {
      if (has_outranger) {
        cout << ", ";
      }
      cout << obj.first;
      has_outranger = true;
    }
  }
  if (has_outranger) cout << " outside the map" << endl;

  // output map
  const char* no_grid = "     "; 
  int step = 3; // step for printing a grid number
  int precision = cout.precision();
  cout.precision(0);
  int y_grid = (size - 1)-((size - 1) % step);
  for (int y = size-1; y >= 0; --y) {
    // output the y-grid
    if (y == y_grid) {
      cout << setw(4) <<(y_grid * scale + origin.y)<< " ";
      y_grid -= step;
    } else {
      cout << no_grid;
    }
    // output the map for row y
    for (int x = 0; x < size; ++x) {
      cout << arr[x][y];
    }
    cout << endl;
  }
  // output the x-grid
  for (int x_grid=0; x_grid < size; x_grid += step) {
    cout << setw(6) << (x_grid * scale + origin.x);
  }
  cout << endl;
  //restore
  cout.precision(precision);
}

// Discard the saved information
void MapView::clear()
{
  objects.clear();
}

// modify the display parameters
void MapView::set_size(int size_)
{
  if (size_ > 30)
    throw Error("New map size is too big!");
  if (size_ <= 6)
    throw Error("New map size is too small!");
  size = size_;
}

// modify the display parameters
void MapView::set_scale(double scale_)
{
  if (scale_ <= 0.0)
    throw Error("New map scale must be positive!");
  scale = scale_;
}

// modify the display parameters
void MapView::set_origin(Point origin_)
{
  origin = origin_;
}

// set display parameters to map's default
void MapView::set_defaults()
{
  size = DEFAULT_SIZE;
  scale = DEFAULT_SCALE;
  origin = DEFAULT_ORIGIN;
}

// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool MapView::get_subscripts(int &ix, int &iy, Point location)
{
  // adjust with origin and scale
  Cartesian_vector subscripts = (location - origin) / scale;
  // truncate coordinates to integer after taking the floor
  // floor function will produce integer smaller than even for negative values, 
  // so - 0.05 => -1., which will be outside the array.
  ix = int(floor(subscripts.delta_x));
  iy = int(floor(subscripts.delta_y));
  // if out of range, return false
  if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
    return false;
    }
  else
    return true;
}

const int SailingDataView::WIDTH = 10;

// update ship's speed 
void SailingDataView::update_ship_speed(const std::string& name, double value)
{
  auto pair = objects.find(name);
  if (pair == objects.end()) {
    ShipInfo new_info;
    new_info.speed = value;
    objects.insert(std::pair<string, ShipInfo>(name, new_info));
  } else {
    pair->second.speed = value;
  }
}

// update ship's course 
void SailingDataView::update_ship_course(const std::string& name, double value)
{
  auto pair = objects.find(name);
  if (pair == objects.end()) {
    ShipInfo new_info;
    new_info.course = value;
    objects.insert(std::pair<string, ShipInfo>(name, new_info));
  } else {
    pair->second.course= value;
  }
}

// update ship's fuel
void SailingDataView::update_ship_fuel(const std::string& name, double value)
{
  auto pair = objects.find(name);
  if (pair == objects.end()) {
    ShipInfo new_info;
    new_info.fuel= value;
    objects.insert(std::pair<string, ShipInfo>(name, new_info));
  } else {
    pair->second.fuel= value;
  }
}


void SailingDataView::update_remove(const std::string& name)
{
  objects.erase(name);
}

void SailingDataView::draw()
{
  cout << "----- Sailing Data -----" << endl;
  cout << setw(WIDTH) << "Ship" << setw(WIDTH) << "Fuel" << setw(WIDTH) << "Course" << setw(WIDTH) << "Speed" << endl;
  for (auto it : objects) {
    cout << setw(WIDTH) << it.first << setw(WIDTH) << it.second.fuel
        << setw(WIDTH) << it.second.course
        << setw(WIDTH) << it.second.speed << endl;
  }
}

void SailingDataView::clear()
{
  objects.clear();
}

const double BridgeView::FULL_ANGLE = 360.0;
const double BridgeView::HALF_ANGLE = 180.0;
const int BridgeView::X_SIZE = 19;
const int BridgeView::Y_SIZE = 3;
const double BridgeView::DEFAULT_SCALE = 10;
const Point BridgeView::PLOT_ORIGIN = Point(-90, 0);
const std::string BridgeView::MULTIPLE = "**"; // for drawing
const std::string BridgeView::EMPTY = ". "; //for drawing
const std::string BridgeView::WATER = "w-"; //for drawing

BridgeView::BridgeView(std::string ownship_)
  :ownship(ownship_)
{}

// Save the supplied name and location for future use in a draw() call
// If the name is already present,the new location replaces the previous one.
void BridgeView::update_location(const std::string& name, Point location)
{
  auto pair = objects.find(name);
  if (pair == objects.end()) {
    objects.insert(std::pair<string, Point>(name, location));
  } else {
    pair->second = location;
  }
}

void BridgeView::update_ship_course(const std::string& name, double value)
{
  if (name == ownship) {
    ownship_course = value;
  }
}

void BridgeView::update_remove(const std::string& name)
{
  if (name == ownship) {
    ownship_sunk_point = objects.find(ownship)->second;
  }
  objects.erase(name);
}

void BridgeView::draw()
{
  vector<vector<string>> arr;
  auto ship = objects.find(ownship);
  if (ship == objects.end()) {
    cout << "Bridge view from " << ownship << " sunk at " << ownship_sunk_point << endl;
    arr = vector<vector<string>>(X_SIZE, vector<string>(Y_SIZE, WATER));
  } else {
    arr = vector<vector<string>>(X_SIZE, vector<string>(Y_SIZE, EMPTY));
    Point ownship_point = ship->second;
    cout << "Bridge view from " << ownship << " position " << ownship_point << " heading " << ownship_course << endl;
    for (auto it : objects) {
      if (it.first == ownship) continue;
      Compass_position cp(ownship_point, it.second);
      // only ships in [0.005, 20] range are shown
      if (cp.range >= 0.005 && cp.range <= 20) {
        int AoB = cp.bearing - ownship_course;
        if (AoB + HALF_ANGLE < 0) {
          AoB += FULL_ANGLE;
        } else if (AoB - HALF_ANGLE > 0) {
          AoB -= FULL_ANGLE;
        }
        
        int x, y;
        if (get_subscripts(x, y, Point(AoB, 0))){
          if (arr[x][y] != EMPTY) {
              arr[x][y] = MULTIPLE;
          } else {
            arr[x][y] = it.first.substr(0,2);
          }
        }
      }
    }
  }
  // output map
  int precision = cout.precision();
  cout.precision(0);
  string space = "     ";
  for (int i = Y_SIZE-1; i >= 0; --i) {
    cout << space;
    for (int j = 0; j < X_SIZE; ++j) {
      cout << arr[j][i];
    }
    cout << endl;
  }
  // output the x-grid
  for (int x_grid=-90; x_grid <= 90; x_grid += 30) {
    cout << setw(6) << x_grid;
  }
  cout << endl;
  //restore
  cout.precision(precision);
}

// Discard the saved information
void BridgeView::clear()
{
  objects.clear();
}

// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the default size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool BridgeView::get_subscripts(int &ix, int &iy, Point location)
{
  // adjust with origin and scale
  Cartesian_vector subscripts = (location - PLOT_ORIGIN) / DEFAULT_SCALE;
  // truncate coordinates to integer after taking the floor
  // floor function will produce integer smaller than even for negative values, 
  // so - 0.05 => -1., which will be outside the array.
  ix = int(floor(subscripts.delta_x));
  iy = int(floor(subscripts.delta_y));
  // if out of range, return false
  if ((ix < 0) || (ix >= X_SIZE) || (iy < 0) || (iy >= Y_SIZE)) {
    return false;
    }
  else
    return true;
}
