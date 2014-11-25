#include "Agent_factory.h"
#include "Geometry.h"
#include "Warriors.h"
#include "Peasant.h"
#include "Utility.h"

using std::string;
using std::shared_ptr;

const char* const soldier_name_c = "Soldier";
const char* const peasant_name_c = "Peasant";
const char* const archer_name_c = "Archer";


//Creates and returns a shared_ptr to the specified Agent type
//Throws an error if the type is unrecognized.
//Is not responsible for the dynamically allocated memory. 
shared_ptr<Agent> create_agent(
                               const string& name,const string& type,
                               Point location)
{
    if(type == soldier_name_c) {
        return shared_ptr<Agent>{new Soldier{name, location}};
    }
    else if(type == peasant_name_c) {
        return shared_ptr<Agent>{new Peasant{name, location}};
    }
    else if(type == archer_name_c) {
        return shared_ptr<Agent>{new Archer{name, location}};
    }
    else {
        throw Error{"Trying to create agent of unknown type!"};
    }
}
