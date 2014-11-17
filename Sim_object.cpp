#include "Sim_object.h"
#include <string>
#include <iostream>//cout, endl
using std::string;
using std::cout;
using std::endl;
//simply announces its construction
Sim_object::Sim_object(const string& name_) : name(name_)
{
    cout << "Sim_object " << name << " constructed" << endl;
}

Sim_object::~Sim_object()
{
    cout << "Sim_object " << get_name() << " destructed" << endl;
}
