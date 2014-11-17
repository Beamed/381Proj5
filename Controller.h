/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <string>

class View;
class Agent;

class Controller {
public:	
	Controller();

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();
	
private:
    //reads an integer and has view's size changed based on it
    void resize(View* view);

    //reads a double and has the current view zoom changed
    //based on it
    void zoom(View* view);
    
    //reads in two doubles and notifies view to change the origin
    void pan(View* view);
    
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
    void move(Agent* agent);
    
    //Reads in two structure names and orders the specified
    //agent to work between them. Throws an error if either
    //structure does not exist.
    void work(Agent* agent);
    
    //Reads in a target name for the specified agent to attack
    //If the target's name doesn't exist, an error is thrown.
    void attack(Agent* agent);
    
    //Orders the agent to stop.
    void stop(Agent* agent);
    
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
