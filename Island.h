/***** Island Class *****/
/* Islands are a kind of Sim_object; they have an amount of fuel and a an amount by which it increases
every update (default is zero). The can also provide or accept fuel, and update their amount
accordingly.
*/
#ifndef ISLAND_H 
#define ISLAND_H
#include "Sim_object.h"
#include "Geometry.h"

class Island : public Sim_object
{
public:
  // initialize then output constructor message
  Island (const std::string& name_, Point position_, double fuel_ = 0., double production_rate_ = 0.);

  // Return whichever is less, the request or the amount left,
  // update the amount on hand accordingly, and output the amount supplied.
  double provide_fuel(double request);
  // Add the amount to the amount on hand, and output the total as the amount the Island now has.
  void accept_fuel(double amount);
  
  Point get_location() const override
    {return position;}

  // if production_rate > 0, compute production_rate * unit time, and add to amount, and print an update message
  void update() override;

  // output information about the current state
  void describe() const override;

  // ask model to notify views of current state
  void broadcast_current_state() override;

  // forbid  copy/move, construction/assignment
  Island(const Island&) = delete;
  Island(const Island&&) = delete; //NOTE: REMOVE const
  Island& operator= (const Island&) = delete;
  Island& operator= (const Island&&) = delete;//NOTE: REMOVE const

private:
  Point position;        // Location of this island
  double fuel;    // amount of fuels initially for this island
  double production_rate;   // rate of fuel production for this island

  static const int UNIT_TIME; //time unit // NOTE: DON'T USE ALL CAPITALIZE FOR CONST VARS, QUESTION: NECESSARY?
};
#endif
