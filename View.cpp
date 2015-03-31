#include "View.h"

View::~View()
{}

// Save the supplied name and location for future use in a draw() call
// If the name is already present,the new location replaces the previous one.
void View::update_location(const std::string& name, Point location)
{}

// update the ship's information, like in update_location
// fat interface for SailingDataView
void View::update_ship_info(const std::string& name, double fuel, double course, double speed)
{}

// Remove the name and its location; no error if the name is not present.
void View::update_remove(const std::string& name)
{}
