/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <string>
#include <memory>
class View;
class Agent;

class Controller {
public:	
	Controller();

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();
	
private:
    //opens a view of the given type, ie. map if "map", health if "health"
    void open();
    //has map_view's default settings set, if open
    void set_default_map();
    //reads an integer and has map view's size changed based on it
    void resize();

    //reads a double and has the current map_view zoom changed
    //based on it
    void zoom();
    
    //reads in two doubles and notifies map_view to change the origin
    void pan();
    //orders all views to draw themselves
    void draw();
    //closes the view read in from stdin. If no view exists, throws error
    void close();
    //forces every object in existence to describe itself by contacting
    //model
    void describe();
    //forces every object to update its current existence - go ahead 1 turn
    void update();
    //reads in a name, type and location for the new structure,
    //and passes it to model, verifying input in the process.
    //If input is incorrect, throws an Error.
    void build();
    
    //Reads in a name, type and location for the new agent,
    //and passes it to model, verifying input in process.
    //If input is incorrect, throws an Error.
    void train();
    
    //Calls the necessary functions ot have an agent move
    //to an x,y position read in from stdin.
    void move(std::shared_ptr<Agent> agent);
    
    //Reads in two structure names and orders the specified
    //agent to work between them. Throws an error if either
    //structure does not exist.
    void work(std::shared_ptr<Agent> agent);
    
    //Reads in a target name for the specified agent to attack
    //If the target's name doesn't exist, an error is thrown.
    void attack(std::shared_ptr<Agent> agent);
    
    //Orders the agent to stop.
    void stop(std::shared_ptr<Agent> agent);
    
    struct New_object {
        std::string name;
        std::string type;
    };
    //Function which reads in a name,
    //type, and location information,
    //performing all necessary error-checking while doing so.
    //Returns a struct with the data it read
    New_object create_object();
    
};

#endif
