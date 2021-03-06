/* *** View class ***
The View class encapsulates the data and functions needed to generate the map
display, and control its properties. It has a "memory" for the names and locations
of the to-be-plotted objects.

Usage: 
1. Call the update_location function with the name and position of each object
to be plotted. If the object is not already in the View's memory, it will be added
along with its location. If it is already present, its location will be set to the 
supplied location. If a single object changes location, its location can be separately
updated with a call to update_location. 

2. Call the update_remove function with the name of any object that should
no longer be plotted. This must be done *after* any call to update_location that
has the same object name since update_location will add any object name supplied.

3. Call the draw function to print out the map.

4. As needed, change the origin, scale, or displayed size of the map
with the appropriate functions. Since the view "remembers" the previously updated
information, the draw function will print out a map showing the previous objects
using the new settings.
*/

#ifndef VIEW_H
#define VIEW_H
#include <string>

struct Point;

class View {
public:
  virtual ~View();

  // update functions
  // If the name is already present,the new info replaces the previous one.
  // update the object's location 
  virtual void update_location(const std::string& name, Point location);
  // update ship's speed
  virtual void update_ship_speed(const std::string& name, double value);
  // update ship's course 
  virtual void update_ship_course(const std::string& name, double value);
  // update ship's fuel
  virtual void update_ship_fuel(const std::string& name, double value);

  // Remove the name and its location; no error if the name is not present.
  virtual void update_remove(const std::string& name) = 0;
  
  // prints out the current map
  virtual void draw() = 0;
};

#endif
