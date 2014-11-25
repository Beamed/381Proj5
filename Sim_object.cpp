#include "Sim_object.h"
#include <string>
#include <iostream>//cout, endl
using std::string;
using std::cout;
using std::endl;
//simply announces its construction
Sim_object::Sim_object(const string& name_) : name(name_)
{
}

Sim_object::~Sim_object()
{
}
