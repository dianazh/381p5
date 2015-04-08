#ifndef CRUISE_SHIP
#define CRUISE_SHIP
#include "Ship.h"
#include <map>
#include <vector>
#include <memory>

class Cruise_ship : public Ship {
public:
  Cruise_ship(const std::string& name_, Point position_);
  // return true if status != not_cruising
  bool is_cruising() const; //NOTE: CAN BE PRIVATE: YAGNI?

  // This class overrides these Ship functions so that it can check if this Cruiser_ship is on a cruise. If so, stop the cruise.
  // if the destination is an island, go to that island and start the cruise. Otherwise, work as normal ships do.
  void set_destination_position_and_speed(Point destination, double speed) override;
  // stops the cruise trip if is cruising
  void set_course_and_speed(double course, double speed) override;
  // stops the cruise trip if is cruising
  void stop() override;
  // update cruise state according to current state
  void update() override;
  // output information about the current state
  void describe() const override;

private:
  enum class State {NOT_CRUISING, TO_NEXT_STOP, REFUEL, WAIT, SET_COURSE};
  // default values
  static const double FUEL_CAPACITY; // NOTE: NO CAPITAL
  static const double MAX_SPEED;
  static const double FUEL_CONSUMPTION;
  static const int RESISTANCE;

  std::shared_ptr<Island> cruise_destination; // the desitination of a cruise trip
  std::shared_ptr<Island> next_stop; // the next stop of the cruise
  double cruise_speed; // the speed of the cruise
  State cruise_state; // the cruise state of the ship
  std::map<std::string, std::shared_ptr<Island>> visited_islands; // the list of visted islands
  std::vector<std::shared_ptr<Island>> all_islands; //the list of all islands

  // helper
  // get the island ptr by a position, return null if no island matches
  std::shared_ptr<Island> get_island(Point point) const; 
  // get the next cruise island, return to original dest if all islands are visited
  std::shared_ptr<Island> get_next_stop() const;
  // cancel a cruise if in a cruise, print end msg
  void cancel_cruise();
  // cancel a cruise, reset all the values
  void cruise_end();
};
#endif
