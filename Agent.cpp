#include "Agent.h"
#include "Model.h"
#include "Utility.h"
#include <iostream>//cout, endl
#include <iomanip>//changing output settings(precision)
#include <cassert>//assert

using std::cout;
using std::endl;
using std::string;
using std::ios;
using std::shared_ptr;
using std::weak_ptr;

const int default_health_c = 5;
const double default_speed_c = 5.;
const int default_precision_c = 2;

//Constructs the Agent by calling agent and moving_object,
//and then setting the default values for its private variables
Agent::Agent(const std::string& name_, Point location_):
Sim_object(name_), moving_obj(location_, default_speed_c),
health(default_health_c), speed(default_speed_c),
alive(true)
{
}
//Returns current location by calling get_current_location
Point Agent::get_location() const
{
    return moving_obj.get_current_location();
}
//Returns is_currently_moving; if dead, has already been set to not move
bool Agent::is_moving() const
{
    return moving_obj.is_currently_moving();
}//if is currently dead, is no longer moving.
//Orders the agent to begin moving to the given destination
void Agent::move_to(Point destination_)
{
    if(destination_ == moving_obj.get_current_location()) {
        cout << get_name() << ": I'm already there" << endl;
        return;
    }
    cout << get_name() <<  ": I'm on the way" << endl;
    moving_obj.start_moving(destination_);
}
//Stops moving and announces they've stopped moving
void Agent::stop()
{
    if(moving_obj.is_currently_moving())  {
        cout << get_name() << ": I'm stopped" << endl;
        moving_obj.stop_moving();
    }
}
//Decrements the health lost by the attack by calling lose_health
void Agent::take_hit(int attack_strength,
                     weak_ptr<Agent> attacker_ptr)
{
    lose_health(attack_strength);
}
//Loses health; if it dies, shouts "Arrggh!"
//Otherwise shouts "Ouch!" and continues its business
void Agent::lose_health(int attack_strength)
{
    health -= attack_strength;
    if(health <= 0) {
        alive = false;
        moving_obj.stop_moving();
        Model::get_instance().notify_gone(get_name());
        cout << get_name() << ": Arrggh!" << endl;
        Model::get_instance().remove_agent(shared_from_this());
        return;
    }
    Model::get_instance().notify_health(get_name(), health);
    cout << get_name() << ": Ouch!" << endl;
}
//Has the agent update its movement if alive
void Agent::update()
{
    if(alive) {
        update_Movement();
    }
}
//Calls update location; if there, announces such.
//If not, announces it's taken another step.
//Either way, notifies model.
void Agent::update_Movement()
{
    if(moving_obj.update_location()) {
        cout << get_name() << ": I'm there!" << endl;
    }
    else if(moving_obj.is_currently_moving()) {
        cout << get_name() << ": step..." << endl;
    }
    Model::get_instance().notify_location(get_name(),
                                 moving_obj.get_current_location());
}
//Outputs all information on the current state of the agent.
void Agent::describe() const
{
    cout << get_name() << " at " <<
        moving_obj.get_current_location() << endl;
    if(is_alive()) {
        cout << "   Health is " << health << endl;
        if(is_moving()) {
            auto old_settings = cout.precision();
            cout << std::fixed << std::setprecision(default_precision_c);
            cout << "   Moving at speed " << speed << " to " <<
                moving_obj.get_current_destination() << endl;
            cout.precision(old_settings);//save and restore old settings
        }
        else  {
            cout << "   Stopped" << endl;
        }
        return;//no need to check dying state if is alive
    }
    else
        cout << "   Is dead" << endl;
}
//Tells Model about the current location of the Agent.
void Agent::broadcast_current_state()
{
    Model::get_instance().notify_location(get_name(), get_location());
    Model::get_instance().notify_health(get_name(), health);
}
//Throws error that it can't work
void Agent::start_working(shared_ptr<Structure>,
                          shared_ptr<Structure>)
{
    throw Error{get_name() + ": Sorry, I can't work!"};
}
//Throws error that it can't attack
void Agent::start_attacking(weak_ptr<Agent>)
{
    throw Error{get_name() +  + ": Sorry, I can't attack!"};
}
