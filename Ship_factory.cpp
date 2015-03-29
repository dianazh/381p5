#include "Ship_factory.h"
#include "Tanker.h"
#include "Cruiser.h"
#include "Utility.h"

/* This is a very simple form of factory, a function; you supply the information, it creates
the specified kind of object and returns a pointer to it. The Ship is allocated
with new, so some other component is resposible for deleting it.
*/
std::shared_ptr<Ship> create_ship(const std::string& name, const std::string& type, Point initial_position)
{
  if (type == "Tanker") {
    return std::make_shared<Tanker>(name, initial_position);
  } else if (type == "Cruiser") {
    return std::make_shared<Cruiser>(name, initial_position);
  } else {
    throw Error("Trying to create ship of unknown type!");
  }
}
