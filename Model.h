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

#include <map>//for map of objects-to-names
#include <set>//for overall set of objs
#include <list>//for list of views
#include <memory>

//forward declarations:
class Agent;
class Structure;
class Sim_object;
class View;
struct Point;
 
class Model {
public:
	// create the initial objects
	Model();
    
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
	void add_structure(std::shared_ptr<Structure> structure);
	// will throw Error("Structure not found!") if no structure of that name
	std::shared_ptr<Structure>
        get_structure_ptr(const std::string& name) const;

	// is there an agent with this name?
	bool is_agent_present(const std::string& name) const;
	// add a new agent; assumes none with the same name
	void add_agent(std::shared_ptr<Agent> agent);
	// will throw Error("Agent not found!") if no agent of that name
	std::shared_ptr<Agent>
        get_agent_ptr(const std::string& name) const;
	
	// tell all objects to describe themselves to the console
	void describe() const;
	// increment the time, and tell all objects to update themselves
	void update();	
	
	/* View services */
	// Attaching a View adds it to the container and causes it to be updated
    // with all current objects'location (or other state information.
	void attach(std::shared_ptr<View> view);
	// Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
	void detach(std::shared_ptr<View> view);
    // notify the views about an object's location
	void notify_location(const std::string& name, Point location);
    //notify the views about an object's amount
    void notify_amount(const std::string& name, double amount);
    //Notify the views about an object's amount of health
    void notify_health(const std::string&name, double health);
	// notify the views that an object is now gone
	void notify_gone(const std::string& name);
    //removes agent from all containers.
    void remove_agent(std::shared_ptr<Agent> agent);
    //returns true if a view of that name exists, false otherwise
    bool has_view(const std::string& name);
    //returns the view with the name requested, or nullptr if none
    //exists
    std::shared_ptr<View> get_view(const std::string& name);
    
    //calls each view's Draw function
    void draw();
    
    //Returns the agent which is closest to the provided agent
    //according to the cartesian distance.
    std::shared_ptr<Agent> get_closest_agent(
                                            std::shared_ptr<Agent> agent);

    std::shared_ptr<Structure> get_closest_structure(
                                            std::shared_ptr<Agent> agent);
    
private:
    //function object to ensure objects are stored in correct order
    struct Less_than_obj_ptr {
        bool operator()
            (std::shared_ptr<Sim_object> p1,
             std::shared_ptr<Sim_object> p2);
    };
    
    std::map<std::string, std::shared_ptr<Agent>> agents;
    std::map<std::string, std::shared_ptr<Structure>> structures;
    std::set<std::shared_ptr<Sim_object>, Less_than_obj_ptr> objects;
    int time;
    std::list<std::shared_ptr<View>> views;
    
    //inserts a structure into the relevant containers
    void insert_structure(std::shared_ptr<Structure> structure);
    
    //inserts an agent into the relevant containers
    void insert_agent(std::shared_ptr<Agent> agent);

	// disallow copy/move construction or assignment
	Model(const Model&) = delete;
	Model& operator= (const Model&)  = delete;
	Model(Model&&) = delete;
	Model& operator= (Model&&) = delete;
};
#endif
