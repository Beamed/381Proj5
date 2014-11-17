#include "Peasant.h"
#include "Structure.h"
#include "Utility.h"
#include <iostream>//cout, endl
#include <cassert>//assert

const double default_food_c = 0.0;
const double max_food_c = 35.0;

using std::string;
using std::cout;
using std::endl;

//constructs & announces construction of a Peasant
Peasant::Peasant(const string& name_, Point location_) :
Agent(name_, location_), working_state(Peasant_state_e::NOT_WORKING),
food(default_food_c), food_src(nullptr), food_dest(nullptr)
{
    cout << "Peasant " << name_ << " constructed" << endl;
}
//announces destruction of Peasant
Peasant::~Peasant()
{
    cout << "Peasant " << get_name() << " destructed" << endl;
}
//Updates the states of the Peasant;
//Processing the depositing or withdrawal of food as specified
//And moving if in state of, well, moving.
//Announces if food is collected, deposited,
//or waiting for food.
void Peasant::update()
{
    Agent::update();
    if(!is_alive() || working_state == Peasant_state_e::NOT_WORKING) {
        return;
    }
    if(working_state == Peasant_state_e::INBOUND &&
       !is_moving() &&
       food_src->get_location() == get_location()) {
        working_state = Peasant_state_e::COLLECTING;
        return;
    }
    if(working_state == Peasant_state_e::COLLECTING) {
        double received_amount = food_src->withdraw(max_food_c - food);
        food += received_amount;
        if(received_amount > 0.0) {//if it is positive
            cout << get_name() << ": Collected " <<
                received_amount << endl;
            working_state = Peasant_state_e::OUTBOUND;
            Agent::move_to(food_dest->get_location());
            return;
        }
        cout << get_name() << ": Waiting " << endl;
        return;
    }
    if(working_state == Peasant_state_e::OUTBOUND &&
       !is_moving() &&
       food_dest->get_location() == get_location()) {
        working_state = Peasant_state_e::DEPOSITING;
        return;
    }
    if(working_state == Peasant_state_e::DEPOSITING) {
        food_dest->deposit(food);
        cout << get_name() << ": Deposited " << food << endl;
        food = default_food_c;
        working_state = Peasant_state_e::INBOUND;
        Agent::move_to(food_src->get_location());
        //call it as Agent::move_to since move_to is overloaded
        //such that an order to move is different from an order to work
    }
}
//Moves to the specified destination, stopping work along the way
void Peasant::move_to(Point dest)
{
    Agent::move_to(dest);
    stop_work();
}
//Checks if the current state is working;
//if so, calls end_work, and announces it's no longer working.
void Peasant::stop_work()
{
    if(working_state != Peasant_state_e::NOT_WORKING) {
        end_work();
        cout << get_name() << ": I'm stopping work" << endl;
    }
}
//Sets food src and dest to null, ensures the peasant isn't working
void Peasant::end_work()
{
    food_src = nullptr;
    food_dest = nullptr;
    working_state = Peasant_state_e::NOT_WORKING;
}
//Stops all current activity
void Peasant::stop()
{
    Agent::stop();
    stop_work();
}
//Starts working at the new source and destination. Announces
//If the source and dest are bad, and also begins depositing
//or collecting if it is already at one of the locations with/without
//food.
void Peasant::start_working(Structure *source_, Structure *destination_)
{
    Agent::stop();
    end_work();
    if(source_ == destination_) {
        throw Error{get_name() + ": I can't move food to and from the same place!"};
    }
    food_src = source_;
    food_dest = destination_;
    if(food == default_food_c){ //if we are not carrying any food
        if(get_location() == food_src->get_location()) {
            working_state = Peasant_state_e::COLLECTING;
            return;
        }
        Agent::move_to(food_src->get_location());
        working_state = Peasant_state_e::INBOUND;
        return;
    }
    //else if we have food:
    if(get_location() == food_dest->get_location()) {
        working_state = Peasant_state_e::DEPOSITING;
    }
    else {
        Agent::move_to(food_dest->get_location());
        working_state = Peasant_state_e::OUTBOUND;
    }
}
//Describes the agent as specified in spec
//also outputting information regarding if it's moving to a destination/src,
//or collecting/depositing food
void Peasant::describe() const
{
    cout << "Peasant ";
    Agent::describe();
    cout << "   Carrying " << food << endl;
    switch(working_state) {
        case Peasant_state_e::NOT_WORKING:
            break;//do nothing
        case Peasant_state_e::OUTBOUND:
            cout << "   Outbound to destination " <<
            food_dest->get_name() << endl;
            break;
        case Peasant_state_e::INBOUND:
            cout << "   Inbound to source " <<
            food_src->get_name() << endl;
            break;
        case Peasant_state_e::COLLECTING:
            cout << "   Collecting at source " <<
            food_src->get_name() << endl;
            break;
        case Peasant_state_e::DEPOSITING:
            cout << "   Depositing at destination " <<
            food_dest->get_name() << endl;
            break;
        default:
            assert(working_state == Peasant_state_e::NOT_WORKING
                   || working_state == Peasant_state_e::DEPOSITING
                   || working_state == Peasant_state_e::INBOUND
                   || working_state == Peasant_state_e::COLLECTING
                   || working_state == Peasant_state_e::OUTBOUND);
            //just error out: something unrecoverable happened
            break;
    }
    
}
