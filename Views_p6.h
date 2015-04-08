#include "View_p5.h"
#include "Geometry.h"

class GridView : public View{
public:
  // get the subscript for the given location in (x,y)
  // return false if location is not in the current range
  bool get_subscripts(int& x, int& y, Point location);

private:
  double scale;
  int x_size, y_size; // boundary of the grid on x, y direction
  Point origin;
};
