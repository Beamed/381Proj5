#include "Structure.h"
#include "Model.h"
#include <iostream>//cout, endl

using std::string;
using std::cout;
using std::endl;

const double default_amount_c = 0.0;

//Constructs itself by calling Sim_object with the name, and updating
//its internal location with location. 
Structure::Structure(const string &name_, Point location):
    Sim_object(name_), cur_location(location)
{
}
//Empty dtor by necessity; it is a pure virtual class
Structure::~Structure()
{
}
//Simply outputs name and location of structure
void Structure::describe() const
{
    cout << get_name() << " at " << cur_location << endl;
}
//Notifies model of the current location
void Structure::broadcast_current_state()
{
    Model::get_instance().notify_location(get_name(), get_location());
    
}
//Returns a default amt
double Structure::withdraw(double amount_to_get)
{
    return default_amount_c;
}

