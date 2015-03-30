#include "View.h"
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

const int View::DEFAULT_SIZE = 25;
const double View::DEFAULT_SCALE = 2.0;
const Point View::DEFAULT_ORIGIN = Point(-10, -10);
const string View::MULTIPLE = "* ";
const string View::EMPTY = ". ";

// default constructor sets the default size, scale, and origin, outputs constructor message
View::View()
  :size(DEFAULT_SIZE), scale(DEFAULT_SCALE), origin(DEFAULT_ORIGIN)
{}

// Save the supplied name and location for future use in a draw() call
// If the name is already present,the new location replaces the previous one.
void View::update_location(const std::string& name, Point location)
{
  auto pair = objects.find(name);
  if (pair == objects.end()) {
    objects.insert(std::pair<string, Point>(name, location));
  } else {
    pair->second = location;
  }
}

// Remove the name and its location; no error if the name is not present.
void View::update_remove(const std::string& name)
{
  objects.erase(name);
}

// prints out the current map
void View::draw()
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
void View::clear()
{
  objects.clear();
}

// modify the display parameters
void View::set_size(int size_)
{
  if (size_ > 30)
    throw Error("New map size is too big!");
  if (size_ <= 6)
    throw Error("New map size is too small!");
  size = size_;
}

// modify the display parameters
void View::set_scale(double scale_)
{
  if (scale_ <= 0.0)
    throw Error("New map scale must be positive!");
  scale = scale_;
}

// modify the display parameters
void View::set_origin(Point origin_)
{
  origin = origin_;
}

// modify the display parameters
void View::set_defaults()
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
bool View::get_subscripts(int &ix, int &iy, Point location)
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

