#include "Warriors.h"
#include "Utility.h"
#include "Geometry.h"
#include "Model.h"
#include "Structure.h"
#include <iostream>//cout, endl
#include <cassert>

const int default_soldier_strength_c = 2;
const double default_soldier_range_c = 2.0;
const char* const default_soldier_msg_c = "Clang!";
const int default_archer_strength_c = 1;
const double default_archer_range_c = 6.0;
const char* const default_archer_msg_c = "Twang!";

using std::string;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::weak_ptr;

//Constructs a Warrior with the given parameters
Warrior::Warrior(const string& name_, Point location_, int strength_,
                 double range_, const std::string &_msg):
Agent(name_, location_),
strength(strength_), range(range_), attacking(false),
attack_msg(_msg)
{
}

//Empty destructor

Warrior::~Warrior()
{
}
//Begins attacking given target if it is a valid target
//Throws an error if not
void Warrior::start_attacking(weak_ptr<Agent> target_ptr)
{
    assert(!target_ptr.expired());
    shared_ptr<Agent> new_target = target_ptr.lock();
    if(new_target == shared_from_this()) {
        //check if agents are the same
        throw Error{get_name() + + ": I cannot attack myself!"};
    }
    if(!new_target->is_alive()) {
        throw Error{get_name() + ": Target is not alive!"};
    }
    if(!in_range(new_target)) {
        throw Error{get_name() + ": Target is out of range!"};
    }
    attack_target(target_ptr);
}
//assumes the target is valid and sets it to be attacked
void Warrior::attack_target(weak_ptr<Agent> target_ptr)
{
    cout << get_name() << ": I'm attacking!" << endl;
    attacking = true;
    target = target_ptr;
}
//Returns true if the target is within range
//False otherwise
bool Warrior::in_range(shared_ptr<Agent> target_ptr) const
{
    return cartesian_distance(get_location(),
                    target_ptr->get_location()) <= range;
}

//Updates the soldier by analyzing current target; if no target,
//does nothing. If target, moves to strike if valid target.
//Discards target if invalid.
void Warrior::update()
{
    Agent::update();
    if(!is_alive() || !is_attacking()) {
        return;
    }//do nothing further if dead or not attacking
    //if this far, means alive & attacking
    shared_ptr<Agent> cur_target = target.lock();
    if(!cur_target || !cur_target->is_alive()) {
        cout << get_name() << ": Target is dead" << endl;
        attacking = false;
        return;
    }
    if(!in_range(cur_target)) {
        cout << get_name() << ": Target is now out of range" << endl;
        attacking = false;
        return;
    }//else we can strike:
    cout << get_name() << ": " << attack_msg << endl;
    cur_target->take_hit(strength, shared_from_this());
    if(!cur_target->is_alive()) {
        //if we killed the target, celebrate!
        cout << get_name() << ": I triumph!" << endl;
        attacking = false;
    }
}

//Outputs the Warrior and Agent information, mainly to include
//type of agent and whether or not they're attacking
void Warrior::describe() const
{
    Agent::describe();
    if(!is_alive()) {
        cout << "   Is dead" << endl;//shouldn't appear in this proj
    }
    if(is_attacking()){
        if(target.expired() || !target.lock()->is_alive())
            cout << "   Attacking dead target" << endl;
        else
            cout << "   Attacking " << target.lock()->get_name() << endl;
    }
    else
        cout << "   Not attacking" << endl;
}
//Outputs a message that a True Warrior doesn't stop.
void Warrior::stop()
{
    cout << get_name() << ": Don't bother me" << endl;
}

//Constructs a soldier by calling the Warrior base ctor,
//mainly by providing the default values
Soldier::Soldier(const string& name_, Point location_) :
Warrior(name_, location_, default_soldier_strength_c,default_soldier_range_c,
        default_soldier_msg_c)
{
}

//Take_hit receives the damage of the enemy,
//but also launches a counter-attack against them!
void Soldier::take_hit(int attack_strength,
                       weak_ptr<Agent> attacker_ptr)
{
    Agent::take_hit(attack_strength, attacker_ptr);
    if(!is_alive()) {//if we died, do nothing
        return;
    }
    if(!is_attacking()) {
        attack_target(attacker_ptr);
    }
}



//Outputs that the Agent is a soldier before proceeding with Warrior describe
void Soldier::describe() const
{
    cout << "Soldier ";
    Warrior::describe();
}
//Constructs an Archer by providing Warrior with the given defaults
//as well as the given name and location
Archer::Archer(const string& name_, Point location_) :
Warrior(name_, location_, default_archer_strength_c, default_archer_range_c,
        default_archer_msg_c)
{//no extra work required
}
//calls Warrior::update; if it is not in an attack state, find a new target
void Archer::update()
{
    Warrior::update();
    if(!is_attacking()) {
        shared_ptr<Agent> closest =
            Model::get_instance().get_closest_agent(shared_from_this());
        if(!in_range(closest)) {
            return;//if closest not in range, do nothing
        }
        attack_target(closest);
    }
}

//Overrides Agent's take_hit to run away when attacked
void Archer::take_hit(int attack_strength, std::weak_ptr<Agent> attacker_ptr)
{
    Agent::lose_health(attack_strength);
    if(!is_alive()) {
        return;//welp. nothing else we can do
    }
    shared_ptr<Structure> closest =
        Model::get_instance().get_closest_structure(shared_from_this());
    cout << get_name() << ": I'm going to run away to " <<
        closest->get_name() << endl;
    move_to(closest->get_location());
}
//Outputs that the Agent is an Archer before proceeding with Warrior describe
void Archer::describe() const
{
    cout << "Archer ";
    Warrior::describe();
}
