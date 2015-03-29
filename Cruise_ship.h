#ifndef CRUISE_SHIP
#define CRUISE_SHIP
#include "Ship.h"

class Cruise_ship : public Ship {
public:
  Cruise_ship();
  ~Cruise_ship();
  void update() override;
  void describe() const override;
  
  virtual void set_destination_position_and_speed(Point destination_position, double speed);

  void set_cruise_destination(shared_ptr<Island> island, double speed);
  
private:
  enum class State {CRUISING, NOT_CRUISING};
  static const double Cruise_ship::FUEL_CAPACITY;
  static const double Cruise_ship::MAX_SPEED;
  static const double Cruise_ship::FUEL_CONSUMPTION;
  static const int Cruise_ship::RESISTANCE;

  std::shared_ptr<Island> destination;
  State cruise_state;
  vector<std::string> visited_islands;
};
#endif
