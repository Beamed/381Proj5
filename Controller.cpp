#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Views.h"
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
#include <memory>

using std::string;
using std::map;
using std::function;
using std::cin;
using std::bind;
using std::exception;
using std::cout;
using std::endl;
using std::any_of;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using namespace std::placeholders;

const char* const exit_cmd_c = "quit";
const char* const error_reading_double_c = "Expected a double!";
const char* const error_reading_int_c = "Expected an integer!";
const int min_str_size_c = 2;
const char* const bad_object_name_error_c = "Invalid name for new object!";
const char* const map_view_name_c = "map";
const char* const map_unopened_c = "No map view is open!";

//skips input until the first new_line character
void skip_Input_Line();

//Reads in a name from stdin, throwing an error if the name is not a valid one
string read_new_name();

//Reads in a point from stdin, by reading x and then y doubles.
//Throws an error if unable to read doubles.
Point get_Point();

//Returns a pointer to a map_view if one exists
//If one doesn't exist, throws an error indicating such
shared_ptr<Map_view> get_map();

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
    //set up maps of commands-to-functions:
    map<string, function<void(void)>> command_fcns;
    map<string, function<void(shared_ptr<Agent>)>> agent_fcns;
    //the following can be called directly:
    
    command_fcns.insert(make_pair("status", bind(&Controller::describe,
                                                 this)));
    command_fcns.insert(make_pair("go",bind(&Controller::update,
                                            this)));
    command_fcns.insert(make_pair("open", bind(&Controller::open, this)));
    command_fcns.insert(make_pair("default",
                                  bind(&Controller::set_default_map, this)));
    command_fcns.insert(make_pair("show", bind(&Controller::draw, this)));
    command_fcns.insert(make_pair("close", bind(&Controller::close, this)));
    //these require their own command functions:
    command_fcns.insert(make_pair("size",
                                  bind(&Controller::resize, this)));
    command_fcns.insert(make_pair("zoom",
                                  bind(&Controller::zoom, this)));
    command_fcns.insert(make_pair("pan", bind(&Controller::pan, this)));
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
                return;//so let's abort
            }
            else if(Model::get_instance().is_agent_present(cmd)) {
                //in other words, if the input is an agent
                shared_ptr<Agent> agent =
                Model::get_instance().get_agent_ptr(cmd);
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
//Opens a new view according to user input.
//If the user gave bad information, throws the relevant error.
void Controller::open()
{
    string type_of_view;
    cin >> type_of_view;
    if(Model::get_instance().has_view(type_of_view)) {
        throw Error{"View of that name already open!"};
    }
    if(type_of_view == map_view_name_c) {
        Model::get_instance().attach(shared_ptr<View>{new Map_view});
    } else if(type_of_view == "health") {
        Model::get_instance().attach(shared_ptr<View>{new Health_view});
    } else if(type_of_view == "amounts") {
        Model::get_instance().attach(shared_ptr<View>{new Amount_view});
    } else if(Model::get_instance().is_name_in_use(type_of_view)) {
        Model::get_instance().attach(
                                     shared_ptr<View>{ new Local_view{type_of_view}});
    } else {
        throw Error{"No object of that name!"};
    }
}
//Closes a view according to user input.
//If the user gave bad information, throws relevant error.
void Controller::close()
{
    string type_of_view;
    cin >> type_of_view;
    shared_ptr<View> view = Model::get_instance().get_view(type_of_view);
    if(view == nullptr) {
        throw Error{"No view of that name is open!"};
    }
    Model::get_instance().detach(view);
}

//Reads an integer "size" and calls view's set_size with the read in value
//Throws an error if unable to read an integer
void Controller::resize()
{
    shared_ptr<Map_view> view = get_map();
    int size;
    cin >> size;
    if(!cin) {
        throw Error{error_reading_int_c};
    }
    view->set_size(size);
}

shared_ptr<Map_view> get_map()
{
    shared_ptr<Map_view> view =
    dynamic_pointer_cast<Map_view>(Model::get_instance().get_view(map_view_name_c));
    if(view == nullptr) {
        throw Error{map_unopened_c};
    }
    return view;
}

//Reads in a double "zoom" and calls view's set_scale with the read in value
//Throws an error if unable to read a double.
void Controller::zoom()
{
    shared_ptr<Map_view> view = get_map();
    double zoom_val;
    cin >> zoom_val;
    if(!cin) {
        throw Error{error_reading_double_c};
    }
    view->set_scale(zoom_val);
}
//Reads in two doubles, x and y, and uses them to create a Point to pass view.
//Throws an error if unable to read doubles.
void Controller::pan()
{
    get_map()->set_origin(get_Point());//can just construct point in-place
}

//Sets the map's defaults
void Controller::set_default_map()
{
    get_map()->set_defaults();
}

//Has the Model render all of the views currently available
void Controller::draw()
{
    Model::get_instance().draw();
}

//Has the Model describe all objects currently in existence.
void Controller::describe()
{
    Model::get_instance().describe();
}
//Has the Model update all objects in existence.
void Controller::update()
{
    Model::get_instance().update();
}

//Reads in the data for a new structure and adds it to the Model
void Controller::build()
{
    New_object new_obj = create_object();
    shared_ptr<Structure> new_struct =
    create_structure(new_obj.name, new_obj.type, get_Point());
    Model::get_instance().add_structure(new_struct);
}
//Reads in and adds the data for a new agent to the Model.
void Controller::train()
{
    New_object new_obj = create_object();
    shared_ptr<Agent> new_agent =
    create_agent(new_obj.name, new_obj.type, get_Point());
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
void Controller::move(shared_ptr<Agent> agent)
{
    agent->move_to(get_Point());
}
//Reads in two structure names and orders the agent given to work with them.
//An error is thrown if either structure doesn't exist.
void Controller::work(shared_ptr<Agent> agent)
{
    string source, dest;
    cin >> source >> dest;
    agent->start_working(
                         Model::get_instance().get_structure_ptr(source),
                         Model::get_instance().get_structure_ptr(dest));
}
//Reads in an agent name and orders the agent to begin attacking him.
//An error is thrown if the target doesn't exist
void Controller::attack(shared_ptr<Agent> agent)
{
    string target;
    cin >> target;
    agent->start_attacking(Model::get_instance().get_agent_ptr(target));
}
//Calls the agent's stop command.
void Controller::stop(shared_ptr<Agent> agent)
{
    agent->stop();
}
//clears any bad state cin has and reads characters until the next newline
void skip_Input_Line()
{
    cin.clear();
    while(cin.get() != '\n');
}
