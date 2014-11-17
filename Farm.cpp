#include "Farm.h"
#include <iostream>//cout, endl

const double default_starting_food_c = 50.0;
const double default_production_c = 2.0;

using std::string;
using std::cout;
using std::endl;

//constructs Farm by invoking Structure constructor
Farm::Farm(const string& name_, Point location_) :
Structure(name_, location_), cur_amount(default_starting_food_c)
{
    cout << "Farm " << name_ << " constructed" << endl;
}
//destructs Farm
Farm::~Farm()
{
    cout << "Farm " << get_name() << " destructed" << endl;
}
//withdraws either the amount requested, or the closest amount
//we can
double Farm::withdraw(double amount_to_get)
{
    if(amount_to_get > cur_amount) {
        amount_to_get = cur_amount;
        cur_amount = 0;//we've emptied out all we can
    }
    else {
        cur_amount -= amount_to_get;
    }
    return amount_to_get;
}

//adds the production amount to our current stored amount
void Farm::update()
{
    cur_amount += default_production_c;
    cout << "Farm " << get_name() << " now has " << cur_amount << endl;
}
//Simply announces it's a farm, calls the structure describe,
//and outputs the current amount of food available.
void Farm::describe() const
{
    cout << "Farm ";
    Structure::describe();
    cout << "   Food available: " << cur_amount << endl;
}
