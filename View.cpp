#include "View.h"

View::~View()
{}

// update the object's location
void View::update_location(const std::string& name, Point location)
{}

// update the ship's information
// If the name is already present,the new info replaces the previous one.
void View::update_info(const std::string& name, const std::string& info_name, double info_value)
{}

// Remove the name and its location; no error if the name is not present.
void View::update_remove(const std::string& name)
{}
