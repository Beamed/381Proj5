#include "Town_Hall.h"
#include "Model.h"
#include <iostream>//cout , endl

const double default_food_c = 0.0;
const double tax_on_food_c = 0.1;
const double min_food_withdrawal_c = 1.0;

using std::string;
using std::cout;
using std::endl;
//constructs and announces construction of Town Hall
Town_Hall::Town_Hall(const string& name_, Point location) :
Structure(name_, location), food(default_food_c)
{
}
//deposits the given amount to Town Hall, no other scruples
void Town_Hall::deposit(double deposit_amount)
{
    food += deposit_amount;
    broadcast_current_state();//let Model know about changes to food
}
//deducts 10% of the amount available as a tax,
//then returns the least amount between the request
//and the remaining amount on hand.
//if < min_food_withdrawal_c is left, return nothing
double Town_Hall::withdraw(double amount_to_obtain)
{
    double avail_amt = food - (food * tax_on_food_c);//get food - tax 
    if(avail_amt < min_food_withdrawal_c) {
        avail_amt = default_food_c;
    }
    if(amount_to_obtain > avail_amt) {
        food -= avail_amt;
        return avail_amt;
    }
    food -= amount_to_obtain;
    broadcast_current_state();//let model know about changes to food
    return amount_to_obtain;
}
//Outputs information about being a town hall, as well as the food contained.
//Otherwise delegates back to Structure::describe()
void Town_Hall::describe() const
{
    cout << "Town_Hall ";
    Structure::describe();
    cout << "   Contains " << food << endl;
}
//broadcasts additional information on current amount in town_hall
void Town_Hall::broadcast_current_state()
{
    Structure::broadcast_current_state();
    Model::get_instance().notify_amount(get_name(), food);
}
