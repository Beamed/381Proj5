/*
A Soldier is an Agent that has attack and defense behaviors. It can be commanded
to start attacking another Agent and will continue the attack as long as 
it is alive and the target is alive and in range. If attacked, the Soldier will
start attacking its attacker.
*/
#ifndef WARRIORS_H
#define WARRIORS_H

#include "Agent.h"

using std::shared_ptr;

class Warrior: public Agent {
public:
    //Constructor for Warrior takes in Agent info and warrior-specific
    //variables
    Warrior(const std::string& name_, Point location_, int strength_,
            double range_, const std::string &_msg);
    virtual ~Warrior() = 0;
    // Make this Warrior start attacking the target Agent.
	// Throws an exception if the target is the same as this Agent,
	// is out of range, or is not alive.
	void start_attacking(std::weak_ptr<Agent> target_ptr) override;
    
    //Returns true if the warrior is currently attacking
    //False otherwise
    bool is_attacking() const { return attacking; }
    
    // update implements a generic Warrior's behavior
    virtual void update();
    //Outputs information about the current state of the Warrior to stdout
    virtual void describe() const;
    
protected:
    //Sets the target to be the target, moves to state is_attacking
    //and announces the attack
    void attack_target(std::weak_ptr<Agent> target_ptr);
    //returns true if the target_ptr is in range
    //false otherwise
    bool in_range(std::shared_ptr<Agent> target_ptr) const;
    
private:
    int strength;
    double range;
    bool attacking;
    std::weak_ptr<Agent> target;
    std::string attack_msg;
    

};

class Soldier : public Warrior{

public:
	//Constructs a Soldier using the given Soldier defaults and the
    //name and location passed.
	Soldier(const std::string& name_, Point location_);
	
	// Overrides Agent's take_hit to counterattack when attacked.
	void take_hit(int attack_strength,
                  std::weak_ptr<Agent> attacker_ptr) override;
	
	// Overrides Agent's stop to print a message
	void stop() override;

	// output information about the current state
	void describe() const override;

};

class Archer: public Warrior {
public:
    //Constructs an Archer using the given Archer defaults
    //and the name and location passed
    Archer(const std::string& name_, Point location_);
    
    //Updates by calling the typical Warrior behavior,
    //but proceeds to pick a new target if the current is killed. 
    void update() override;
    //Overrides Agent's take_hit to run away when attacked
    void take_hit(int attack_strength,
                  std::weak_ptr<Agent> attacker_ptr) override;
    //Overrides describe to also output that the Agent is an archer
    void describe() const override;
};

#endif
