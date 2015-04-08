#include "Sim_object.h"

// output the constructor message with object's name
Sim_object::Sim_object(const std::string& name_)
  :name(name_)
{}

Sim_object::~Sim_object()//NOTE: BETTER DEFINE DIRECLY IN THE .H FILE
{}
