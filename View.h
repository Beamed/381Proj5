
#ifndef VIEW_H
#define VIEW_H

#include <string>
class Point;
/*View provides an interface for the various Views
 to use; it provides no actual implementation for anything.
 */
class View {
public:
    virtual ~View() = 0;//make pure virtual class
    
	//Takes in a name and a location, and updates it
    //Does nothing for the abstract class
	virtual void update_location(const std::string& name,
                                 Point location);
    
    //Takes in a name and an amount, and updates it
    //Does nothing for the abstract class
    virtual void update_amount(const std::string& name,
                                    double amount) {}
    //takes in a name and an amount to update it
    //Does nothing for the abstract class
    virtual void update_health(const std::string& name,
                               double health) {}
	
	// Remove the name and its location; no error if the name is not present.
    virtual void update_remove(const std::string& name) {}
	
	// prints out the current view
    virtual void draw() {}
	
	// Discard any saved information
    virtual void clear() {}
    
    //Returns a string representation of the View.
    virtual std::string get_name() = 0;

};

#endif
