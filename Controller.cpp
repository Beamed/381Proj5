#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Agent_factory.h"
#include "Structure_factory.h"
#include "Utility.h"
#include "Agent.h"
#include <iostream>//cout, endl
#include <string>
#include <map>//for map
#include <functional>//for bind!
#include <utility>//make_pair
#include <cctype>//alphanum
#include <algorithm>//any_of

using std::string;
using std::map;
using std::function;
using std::cin;
using std::bind;
using std::exception;
using std::cout;
using std::endl;
using std::any_of;
using namespace std::placeholders;

const char* const exit_cmd_c = "quit";
const char* const error_reading_double_c = "Expected a double!";
const char* const error_reading_int_c = "Expected an integer!";
const int min_str_size_c = 2;
const char* const bad_object_name_error_c = "Invalid name for new object!";

//skips input until the first new_line character
void skip_Input_Line();

//Reads in a name from stdin, throwing an error if the name is not a valid one
string read_new_name();

//Reads in a point from stdin, by reading x and then y doubles.
//Throws an error if unable to read doubles.
Point get_Point();

//no work needed; simply instantiates the controller
Controller::Controller()
{
    //empty: no work needed
}
//Runs an infinite loop processing user commands.
//When the user eventually quits, deletes the view it instantiated
//and sent to Model.
void Controller::run()
{
    View* view = new View;
    //we're going to use this a lot, so grab it just for clarity
    Model::get_instance().attach(view);
    
    //set up maps of commands-to-functions:
    map<string, function<void(void)>> command_fcns;
    map<string, function<void(Agent*)>> agent_fcns;
    //the following can be called directly:
    command_fcns.insert(make_pair("default",
                                  bind(&View::set_defaults, view)));
    command_fcns.insert(make_pair("show", bind(&View::draw, view)));
    command_fcns.insert(make_pair("status",
                                   bind(&Model::describe,
                                        &Model::get_instance())));
    command_fcns.insert(make_pair("go",
                                  bind(&Model::update,
                                       &Model::get_instance())));
    //pass a ptr to Model, as we don't want to make any needless
    //copies
    
    //these require their own command functions:
    command_fcns.insert(make_pair("size",
                                  bind(&Controller::resize, this, view)));
    command_fcns.insert(make_pair("zoom",
                                  bind(&Controller::zoom, this, view)));
    command_fcns.insert(make_pair("pan", bind(&Controller::pan, this, view)));
    command_fcns.insert(make_pair("build", bind(&Controller::build, this)));
    command_fcns.insert(make_pair("train", bind(&Controller::train, this)));
    
    //the following require an agent's name to be read in before being called:
    agent_fcns.insert(make_pair("move", bind(&Controller::move,
                                             this, _1)));
    agent_fcns.insert(make_pair("work", bind(&Controller::work,
                                             this, _1)));
    agent_fcns.insert(make_pair("attack", bind(&Controller::attack,
                                               this, _1)));
    agent_fcns.insert(make_pair("stop", bind(&Controller::stop, this, _1)));
    
    while(true) {//run until "quit" has been read
        try {
            cout << "\nTime " << Model::get_instance().get_time()<< ": Enter command: ";
            string cmd;
            cin >> cmd;
            if(cmd == exit_cmd_c) {
                cout << "Done" << endl;
                Model::get_instance().detach(view);
                delete view;//model's dtor should handle the rest.
                return;//so let's abort
            }
            else if(Model::get_instance().is_agent_present(cmd)) {
                //in other words, if the input is an agent
                Agent* agent = Model::get_instance().get_agent_ptr(cmd);
                if(!agent->is_alive()) {
                    throw Error{"Agent is not alive!"};
                }
                cin >> cmd;
                auto fcn = agent_fcns.find(cmd);
                if(fcn == agent_fcns.end()) {
                    throw Error{"Unrecognized command!"};
                }
                fcn->second(agent);//otherwise call function
            }
            else {
                auto fcn_cmd = command_fcns.find(cmd);
                if(fcn_cmd == command_fcns.end()) {
                    throw Error{"Unrecognized command!"};
                }
                fcn_cmd->second();
            }
        }
        catch(exception& error) {
            cout << error.what() << endl;
            skip_Input_Line();
        }
        catch(...) {
            cout << "Unknown exception caught!" << endl;
            skip_Input_Line();
        }
    }
}
//Reads an integer "size" and calls view's set_size with the read in value
//Throws an error if unable to read an integer
void Controller::resize(View* view)
{
    int size;
    cin >> size;
    if(!cin) {
        throw Error{error_reading_int_c};
    }
    view->set_size(size);
}

//Reads in a double "zoom" and calls view's set_scale with the read in value
//Throws an error if unable to read a double.
void Controller::zoom(View* view)
{
    double zoom_val;
    cin >> zoom_val;
    if(!cin) {
        throw Error{error_reading_double_c};
    }
    view->set_scale(zoom_val);
}
//Reads in two doubles, x and y, and uses them to create a Point to pass view.
//Throws an error if unable to read doubles.
void Controller::pan(View* view)
{
    view->set_origin(get_Point());//can just construct point in-place
}

//Reads in the data for a new structure and adds it to the Model
void Controller::build()
{
    New_object new_obj = create_object();
    Structure* new_struct = create_structure(new_obj.name, new_obj.type,
                                             get_Point());
    Model::get_instance().add_structure(new_struct);
}
//Reads in and adds the data for a new agent to the Model.
void Controller::train()
{
    New_object new_obj = create_object();
    Agent* new_agent = create_agent(new_obj.name, new_obj.type,
                                    get_Point());
    Model::get_instance().add_agent(new_agent);
}
//Reads in the necessary data for a generic new object,
//verifying the name and doubles,
//before returning it in a "New_object" struct
Controller::New_object Controller::create_object() {
    string new_name = read_new_name();
    string type;
    cin >> type;
    return New_object{new_name, type};
}

//Reads in a name for a new object, and throws an error if it
//is not at least 2 chars and doesn't only consist of letters/number
string read_new_name()
{
    string new_name;
    cin >> new_name;
    if(new_name.length() < min_str_size_c) {
        throw Error{bad_object_name_error_c};
    }
    if(any_of(new_name.begin(), new_name.end(), [](const char& c) {
        return !isalnum(c);
    })) {//if any character is not alphanumeric, throw error:
        throw Error{bad_object_name_error_c};
    }
    if(Model::get_instance().is_name_in_use(new_name)) {
        throw Error{bad_object_name_error_c};
    }
    return new_name;
}

//Reads in x, y values from cin, and throws an error if it is not able to.
Point get_Point()
{
    double x, y;
    cin >> x >> y;
    if(!cin) {
        throw Error{error_reading_double_c};
    }
    return Point{x, y};
}

//calls move_to and orders the agent to move to a location read from cin
void Controller::move(Agent* agent)
{
    agent->move_to(get_Point());
}
//Reads in two structure names and orders the agent given to work with them.
//An error is thrown if either structure doesn't exist.
void Controller::work(Agent* agent)
{
    string source, dest;
    cin >> source >> dest;
    agent->start_working(Model::get_instance().get_structure_ptr(source),
                         Model::get_instance().get_structure_ptr(dest));
}
//Reads in an agent name and orders the agent to begin attacking him.
//An error is thrown if the target doesn't exist
void Controller::attack(Agent* agent)
{
    string target;
    cin >> target;
    agent->start_attacking(Model::get_instance().get_agent_ptr(target));
}
//Calls the agent's stop command.
void Controller::stop(Agent *agent)
{
    agent->stop();
}
//clears any bad state cin has and reads characters until the next newline
void skip_Input_Line()
{
    cin.clear();
    while(cin.get() != '\n');
}
