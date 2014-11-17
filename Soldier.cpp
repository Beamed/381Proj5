#include "Soldier.h"
#include "Utility.h"
#include "Geometry.h"
#include <iostream>//cout, endl

const int default_strength_c = 2;
const double default_range_c = 2.0;

using std::string;
using std::cout;
using std::endl;

//Constructs a soldier by calling the Agent base ctor,
//and then adding the defaults to new member vars
Soldier::Soldier(const string& name_, Point location) :
Agent(name_, location), strength(default_strength_c), range(default_range_c),
is_attacking(false), target(nullptr)
{
    cout << "Soldier " << name_ << " constructed" << endl;
}
//Destructs by signalling destruction
Soldier::~Soldier()
{
    cout << "Soldier " << get_name() << " destructed" << endl;
}
//Begins attacking given target if it is a valid target
//Throws an error if not
void Soldier::start_attacking(Agent *target_ptr)
{
    if(target_ptr == this) {
        throw Error{get_name() + + ": I cannot attack myself!"};
    }
    if(!target_ptr->is_alive()) {
        throw Error{get_name() + ": Target is not alive!"};
    }
    if(!in_range(target_ptr)) {
        throw Error{get_name() + ": Target is out of range!"};
    }
    attack_target(target_ptr);
}
//assumes the target is valid and sets it to be attacked
void Soldier::attack_target(Agent *target_ptr)
{
    cout << get_name() << ": I'm attacking!" << endl;
    is_attacking = true;
    target = target_ptr;
}
//Returns true if the target is within range
//False otherwise
bool Soldier::in_range(Agent *target_ptr) const
{
    return cartesian_distance(get_location(),
                    target_ptr->get_location()) <= range;
}
//Updates the soldier by analyzing current target; if no target,
//does nothing. If target, moves to strike if valid target.
//Discards target if invalid.
void Soldier::update()
{
    Agent::update();
    if(!is_alive() || !is_attacking) {
        return;
    }//do nothing further if dead or not attacking
    //if this far, means alive & attacking
    if(!target->is_alive()) {
        cout << get_name() << ": Target is dead" << endl;
        is_attacking = false;
        target = nullptr;
        return;
    }
    if(!in_range(target)) {
        cout << get_name() << ": Target is now out of range" << endl;
        stop_attacking();
        return;
    }//else we can strike:
    cout << get_name() << ": Clang!" << endl;//soldiers attack w/a sword
    target->take_hit(strength, this);
    if(!target->is_alive()) {
        //if we killed the target, celebrate!
        cout << get_name() << ": I triumph!" << endl;
        stop_attacking();
    }
}

//Sets variables to relevant values if an attack is stopped
//for any reason
void Soldier::stop_attacking()
{
    is_attacking = false;
    target = nullptr;
}

//Take_hit receives the damage of the enemy,
//but also launches a counter-attack against them!
void Soldier::take_hit(int attack_strength, Agent *attacker_ptr)
{
    Agent::take_hit(attack_strength, attacker_ptr);
    if(is_attacking && !is_alive()) {//if we died
        stop_attacking();
        return;
    }
    if(!is_attacking) {
        attack_target(attacker_ptr);
    }
}

//Outputs a message that a soldier doesn't stop.
void Soldier::stop()
{
    cout << get_name() << ": Don't bother me" << endl;
}

//Outputs the Soldier and Agent information, mainly to include
//type of agent and whether or not they're attacking
void Soldier::describe() const
{
    cout << "Soldier ";
    Agent::describe();
    if(is_attacking)
        cout << "   Attacking " << target->get_name() << endl;
    else
        cout << "   Not attacking" << endl;
}
