/*
Model is part of a simplified Model-View-Controller pattern.
Model keeps track of the Sim_objects in our little world. It is the only
component that knows how many Islands and Ships there are, but it does not
know about any of their derived classes, nor which Ships are of what kind of Ship. 
It has facilities for looking up objects by name, and removing Ships.  When
created, it creates an initial group of Islands and Ships using the Ship_factory.
Finally, it keeps the system's time.

Controller tells Model what to do; Model in turn tells the objects what do, and
when asked to do so by an object, tells all the Views whenever anything changes that might be relevant.
Model also provides facilities for looking up objects given their name.
*/
#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <map>
#include <vector>
#include <memory>
struct Point;
class Sim_object;
class Island;
class Ship;
class View;

// Declare the global model pointer
class Model;

class Model {
public:
  // get the instance
  static Model& get_Instance();

  // return the current time
  int get_time() {return time;}

  // is name already in use for either ship or island?
  // either the identical name, or identical in first two characters counts as in-use
  bool is_name_in_use(const std::string& name) const;

  // is there such an island?
  bool is_island_present(const std::string& name) const;
  // will throw Error("Island not found!") if no island of that name
  std::shared_ptr<Island> get_island_ptr(const std::string& name) const;
  // will return nullptr if no island of that position 
  std::shared_ptr<Island> get_island_ptr(const Point& point) const;
  // will return nullptr if all islands are visited
  std::shared_ptr<Island> get_next_island(const Point& point, 
    const std::map<std::string, std::shared_ptr<Island>>& visited) const;

  // is there such an ship?
  bool is_ship_present(const std::string& name) const;
  // add a new ship to the list, and update the view
  void add_ship(std::shared_ptr<Ship>);
  // remove the Ship from the containers
  void remove_ship(std::shared_ptr<Ship> ship_ptr);
  // will throw Error("Ship not found!") if no ship of that name
  std::shared_ptr<Ship> get_ship_ptr(const std::string& name) const;
  
  // tell all objects to describe themselves
  void describe() const;
  // increment the time, and tell all objects to update themselves
  void update();  
  
  /* Note: In Project 4 there is only one View. There will be multiple View objects
  later. So implement the View services so that multiple Views are possible by
  using a container of View pointers.  You should delete this comment. */
    
  /* View services */
  // Attaching a View adds it to the container and causes it to be updated
  // with all current objects'location (or other state information.
  void attach(std::shared_ptr<View>);
  // Detach the View by discarding the supplied pointer from the container of Views
  // - no updates sent to it thereafter.
  void detach(std::shared_ptr<View>);
  
  // notify the views about an object's location
  void notify_location(const std::string& name, Point location);
  // notify the views that an object is now gone
  void notify_gone(const std::string& name);

  // disallow copy/move construction or assignment
  Model(const Model&) = delete;
  Model(Model&&) = delete;
  Model& operator= (const Model&) = delete;
  Model& operator= (Model&&) = delete;

private:
  int time;    // the simulated time
  
  // ordered container for sim_objects
  std::map<std::string, std::shared_ptr<Sim_object>> sim_objects;
  // ordered container for islands 
  std::map<std::string, std::shared_ptr<Island>> islands;
  // ordered container for ships 
  std::map<std::string, std::shared_ptr<Ship>> ships;
  // container for views 
  std::vector<std::shared_ptr<View>> views;

  // create the initial objects, output constructor message
  Model();

  //helper
  // insert an island to its containers
  void insert_island(std::shared_ptr<Island> island);
  // insert a ship to its containers
  void insert_ship(std::shared_ptr<Ship> ship);
  
  // destroy all objects, output destructor message
  ~Model();
};

#endif
