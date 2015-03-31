#ifndef CRUISE_SHIP
#define CRUISE_SHIP
#include "Ship.h"
#include <map>
#include <memory>

class Cruise_ship : public Ship {
public:
  Cruise_ship(const std::string& name_, Point position_);
  bool is_cruising() const;

  // This class overrides these Ship functions so that it can check if this Cruiser_ship is on a cruise. If so, stop the cruise.
  // if the destination is an island, go to that island and start the cruise. Otherwise, work as normal ships do.
  void set_destination_position_and_speed(Point destination, double speed) override;
  // stops the cruise trip if is cruising
  void set_course_and_speed(double course, double speed) override;
  // stops the cruise trip if is cruising
  void stop() override;
  void update() override;
  void describe() const override;

private:
  enum class State {NOT_CRUISING, TO_NEXT_STOP, REFUEL, WAIT, SET_COURSE};
  static const double FUEL_CAPACITY;
  static const double MAX_SPEED;
  static const double FUEL_CONSUMPTION;
  static const int RESISTANCE;

  std::shared_ptr<Island> cruise_destination;
  std::shared_ptr<Island> next_stop;
  double cruise_speed;
  State cruise_state;
  std::map<std::string, std::shared_ptr<Island>> visited_islands;

  // helper
  void cancel_cruise();
  void cruise_end();
};
#endif
