CC = g++
LD = g++

CFLAGS = -c -pedantic-errors -std=c++11 -Wall -fno-elide-constructors
LFLAGS = -pedantic -Wall

OBJS = p5_main.o Model.o Controller.o View.o Views.o Ship_factory.o Cruiser.o Warship.o Cruise_ship.o Tanker.o Ship.o Island.o Sim_object.o Utility.o Track_base.o Navigation.o Geometry.o
PROG = p5exe

default: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p5_main.o: p5_main.cpp Model.h Controller.h
	$(CC) $(CFLAGS) p5_main.cpp

Model.o: Model.cpp Ship_factory.h Utility.h Sim_object.h Island.h Ship.h View.h Geometry.h
	$(CC) $(CFLAGS) Model.cpp

Controller.o: Controller.cpp Ship_factory.h Utility.h Model.h View.h Ship.h Island.h Geometry.h Views.h
	$(CC) $(CFLAGS) Controller.cpp

Views.o: Views.cpp Views.h View.h Navigation.h
	$(CC) $(CFLAGS) Views.cpp

View.o: View.cpp View.h Geometry.h
	$(CC) $(CFLAGS) View.cpp

Ship_factory.o: Ship_factory.cpp Ship_factory.h Utility.h Tanker.h Cruiser.h
	$(CC) $(CFLAGS) Ship_factory.cpp

Cruiser.o: Cruiser.cpp Cruiser.h Warship.h
	$(CC) $(CFLAGS) Cruiser.cpp

Warship.o: Warship.cpp Warship.h Ship.h Utility.h
	$(CC) $(CFLAGS) Warship.cpp

Cruise_ship.o: Cruise_ship.cpp Cruise_ship.h Ship.h Model.h Island.h
	$(CC) $(CFLAGS) Cruise_ship.cpp

Tanker.o: Tanker.cpp Tanker.h Ship.h Island.h Utility.h
	$(CC) $(CFLAGS) Tanker.cpp

Ship.o: Ship.cpp Ship.h Sim_object.h Track_base.h Geometry.h Island.h Model.h Utility.h
	$(CC) $(CFLAGS) Ship.cpp

Island.o: Island.cpp Island.h Sim_object.h Geometry.h Model.h
	$(CC) $(CFLAGS) Island.cpp

Sim_object.o: Sim_object.cpp Sim_object.h 
	$(CC) $(CFLAGS) Sim_object.cpp

Utility.o: Utility.cpp Utility.h 
	$(CC) $(CFLAGS) Utility.cpp

Track_base.o: Track_base.cpp Track_base.h Geometry.h Navigation.h
	$(CC) $(CFLAGS) Track_base.cpp

Navigation.o: Navigation.cpp Navigation.h Geometry.h
	$(CC) $(CFLAGS) Navigation.cpp

Geometry.o: Geometry.cpp Geometry.h
	$(CC) $(CFLAGS) Geometry.cpp

clean:
	rm -f *.o

real_clean:
	rm -f *.o
	rm -f *exe

