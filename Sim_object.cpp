#include "Sim_object.h"
#include <iostream>
using std::cout;
using std::endl;

// output the constructor message with object's name
Sim_object::Sim_object(const std::string& name_)
  :name(name_)
{}

Sim_object::~Sim_object()
{}
