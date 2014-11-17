/*
A Soldier is an Agent that has attack and defense behaviors. It can be commanded
to start attacking another Agent and will continue the attack as long as 
it is alive and the target is alive and in range. If attacked, the Soldier will
start attacking its attacker.
*/
#ifndef SOLDIER_H
#define SOLDIER_H

#include "Agent.h"

class Soldier : public Agent {

public:
	
	Soldier(const std::string& name_, Point location_);
	~Soldier();
	
	// update implements Soldier behavior
	void update() override;
	
	// Make this Soldier start attacking the target Agent.
	// Throws an exception if the target is the same as this Agent,
	// is out of range, or is not alive.
	void start_attacking(Agent* target_ptr) override;
	
	// Overrides Agent's take_hit to counterattack when attacked.
	void take_hit(int attack_strength, Agent* attacker_ptr) override;
	
	// Overrides Agent's stop to print a message
	void stop() override;

	// output information about the current state
	void describe() const override;
    
private:
    int strength;
    double range;
    bool is_attacking;
    Agent* target;
    
    //returns true if the target_ptr is in range
    //false otherwise
    bool in_range(Agent* target_ptr) const;
    //Sets variables to relevant values if an attack is stopped
    void stop_attacking();
    //Sets the target to be the target, moves to state is_attacking
    //and announces the attack
    void attack_target(Agent* target_ptr);
};

#endif
