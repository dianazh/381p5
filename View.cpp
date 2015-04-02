#include "View.h"

// virtual destructor
View::~View()
{}

// update functions
// If the name is already present,the new info replaces the previous one.
// update the object's location
void View::update_location(const std::string& name, Point location)
{}

// update ship's speed
void View::update_ship_speed(const std::string& name, double value)
{}

// update ship's course 
void View::update_ship_course(const std::string& name, double value)
{}

// update ship's fuel
void View::update_ship_fuel(const std::string& name, double value)
{}

// Remove the name and its location; no error if the name is not present.
void View::update_remove(const std::string& name)
{}
