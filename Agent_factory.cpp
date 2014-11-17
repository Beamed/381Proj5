#include "Agent_factory.h"
#include "Geometry.h"
#include "Soldier.h"
#include "Peasant.h"
#include "Utility.h"

using std::string;

const char* const soldier_name_c = "Soldier";
const char* const peasant_name_c = "Peasant";


//Creates and returns a pointer to the specified Agent type
//Throws an error if the type is unrecognized.
//Is not responsible for the dynamically allocated memory. 
Agent* create_agent(const string& name, const string& type,
                    Point location)
{
    if(type == soldier_name_c) {
        return new Soldier{name, location};
    }
    else if(type == peasant_name_c) {
        return new Peasant{name, location};
    }
    else {
        throw Error{"Trying to create agent of unknown type!"};
    }
}
