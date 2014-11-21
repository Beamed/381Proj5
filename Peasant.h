/* 
A Peasant is an Agent that can move food between Structures. It can be commanded to
start_working, whereupon it moves to the source, picks up food, returns to destination,
deposits the food, returns to source.  If picks up zero food at the source, it waits there
and tries again on the next update. 
If commanded to move_to somewhere, it stops working, and goes there.
*/

#ifndef PEASANT_H
#define PEASANT_H

#include "Agent.h"

class Structure;

class Peasant : public Agent {

public:
    
	Peasant(const std::string& name_, Point location_);

	~Peasant();

	// implement Peasant behavior
	void update() override;
	
	// overridden to suspend working behavior
    void move_to(Point dest) override;
	
	// stop moving and working
	void stop() override;

	// starts the working process
	// Throws an exception if the source is the same as the destination.
	void start_working(std::shared_ptr<Structure> source_,
                       std::shared_ptr<Structure> destination_) override;

	// output information about the current state
	void describe() const override;
    //notify Model about the amount carried 
    void broadcast_current_state() override;
    
private:
    enum class Peasant_state_e {
        NOT_WORKING,
        DEPOSITING,
        INBOUND,
        COLLECTING,
        OUTBOUND
    };
    Peasant_state_e working_state;
    double food;
    std::shared_ptr<Structure> food_src;
    std::shared_ptr<Structure> food_dest;
    //Checks if current state is working;
    //if it is, outputs "I'm stopping work",
    //and calls end_work
    void stop_work();
    //sets food src/dest to nullptr, and sets state to not working.
    void end_work();
};

#endif
