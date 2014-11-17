/*
Model is part of a simplified Model-View-Controller pattern.
Model keeps track of the Sim_objects in our little world. It is the only
component that knows how many Structures and Agents there are, but it does not
know about any of their derived classes, nor which Agents are of what kind of Agent. 
It has facilities for looking up objects by name, and removing Agents.  When
created, it creates an initial group of Structures and Agents using the Structure_factory
and Agent_factory.
Finally, it keeps the system's time.

Controller tells Model what to do; Model in turn tells the objects what do, and
when asked to do so by an object, tells all the Views whenever anything changes that might be relevant.
Model also provides facilities for looking up objects given their name.

Notice how only the Standard Library headers need to be included - reduced coupling!

*/
#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <set>
#include <list>

class Model;//forward declare for ptrs:
class Agent;
class Structure;
class Sim_object;//forward declare so we don't include into header
class View;
struct Point;
 
class Model {
public:
	// create the initial objects
	Model();
	
	// destroy all objects
	~Model();
    
    //Returns the current instance of Model.
    //If none exists, implicitly creates one.
    static Model& get_instance();

	// return the current time
	int get_time() {return time;}

	// is name already in use for either agent or structure?
    // return true if the name matches the name of an existing
    //agent or structure
	bool is_name_in_use(const std::string& name) const;

	// is there a structure with this name?
	bool is_structure_present(const std::string& name) const;
	// add a new structure; assumes none with the same name
	void add_structure(Structure* structure);
	// will throw Error("Structure not found!") if no structure of that name
	Structure* get_structure_ptr(const std::string& name) const;

	// is there an agent with this name?
	bool is_agent_present(const std::string& name) const;
	// add a new agent; assumes none with the same name
	void add_agent(Agent* agent);
	// will throw Error("Agent not found!") if no agent of that name
	Agent* get_agent_ptr(const std::string& name) const;
	
	// tell all objects to describe themselves to the console
	void describe() const;
	// increment the time, and tell all objects to update themselves
	void update();	
	
	/* View services */
	// Attaching a View adds it to the container and causes it to be updated
    // with all current objects'location (or other state information.
	void attach(View* view);
	// Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
	void detach(View* view);
    // notify the views about an object's location
	void notify_location(const std::string& name, Point location);
	// notify the views that an object is now gone
	void notify_gone(const std::string& name);
	
private:
    //function object to ensure objects are stored in correct order
    struct Less_than_obj_ptr {
        bool operator() (Sim_object* p1, Sim_object* p2);
    };
    
    std::map<std::string, Agent*> agents;
    std::map<std::string, Structure*> structures;
    std::set<Sim_object*, Less_than_obj_ptr> objects;
    int time;
    std::list<View*> views;
    
    //inserts a structure into the relevant containers
    void insert_structure(Structure* structure);
    
    //inserts an agent into the relevant containers
    void insert_agent(Agent* agent);
    //removes agent from all containers and deletes them.
    void remove_agent(Agent* agent);

	// disallow copy/move construction or assignment
	Model(const Model&) = delete;
	Model& operator= (const Model&)  = delete;
	Model(Model&&) = delete;
	Model& operator= (Model&&) = delete;
};
#endif
