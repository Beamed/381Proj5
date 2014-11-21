#ifndef VIEWS_H
#define VIEWS_H
#include "View.h"
#include "Geometry.h"//Point
#include <map>//map
#include <vector>//generated map
#include <list>//list of objects outside the map return

static const int default_size_c = 25;
static const double default_scale_c = 2.0;
static const double default_origin_x_c = -10.0;
static const double default_origin_y_c = -10.0;

//This class provides a basic interface for drawing tiles to the
//stdout in the form of a map. Note that it does not have any details
//on the objects it prints, just the location and name
class Tile_view : public View {
public:
    //make pure virtual:
    virtual ~Tile_view() = 0;
    
    // Save the supplied name and location for future use in a draw() call
    // If the name is already present,the new location replaces the previous one.
    virtual void update_location(const std::string& name,
                                 Point location) override;
    
    // Remove the name and its location; no error if the name is not present.
    virtual void update_remove(const std::string& name) override;
    
    // prints out the current map
    virtual void draw();
    // Discard the saved information - drawing will show only a empty pattern
    void clear() override;
    
    // modify the display parameters
    // if the size is out of bounds will throw Error("New map size is too big!")
    // or Error("New map size is too small!")
    virtual void set_size(int size_);
    
    // If scale is not postive, will throw Error("New map scale must be positive!");
    virtual void set_scale(double scale_);
    
    // any values are legal for the origin
    virtual void set_origin(Point origin_);
    
protected:
    //Default constructor takes in the size, scale, and origin coords
    //And initializes them to the given values
    Tile_view(int size_ = default_size_c,
              double scale_ = default_scale_c,
              double origin_x = default_origin_x_c,
              double origin_y = default_origin_y_c);
    
    //draws out a given 2-dimensional map to stdout.
    void draw_map(const std::vector<std::vector<std::string>>& map);
    
    //generates a 2-dimensional map from the current map of objects
    std::vector<std::vector<std::string>> gen_map();
    
    //outputs information about the current state of the map to cout
    void describe();
    
    //returns a list of objects which are outside of the current grid
    std::list<std::string> get_outside_objects();
    
    //returns the current location of a given object
    Point get_object_location(const std::string& name);
    
private:
    int size;
    double scale;
    Point origin;
    std::map<std::string, Point> objects;
    
    //Performs the equation which returns the expected value to
    //be printed as an axis label.
    double get_axis_label(int coord, double origin_mod);
    
    //returns true if point is within the grid, false otherwise
    bool get_subscripts(int &ix, int &iy, Point location);

};

class Map_view: public Tile_view
{
    public:
    //constructs a map view with the specified default vals
    Map_view();
    
    //overrides draw to also give information on anyone outside
    //the map, and give current map parameters
    void draw() override;
    
    
    // set the parameters to the default values
    void set_defaults();
    //returns the internal str representation of this view
    std::string get_name() override {
        return "map";
    }
    
};

class Local_view: public Tile_view
{
    public:
    //Constructs a view of a map centered on a single object.
    Local_view(const std::string &name);
    //Updates the location of the given object,
    //and if it is our followed object,
    //updates our origin as well. 
    void update_location(const std::string& name,
                         Point location) override;
    //Overrides virtual function to do nothing
    void set_size(int size_) override {}
    
    //Overrides virtual function to do nothing
    void set_scale(double scale_) override {}
    //Overrides virtual function to do nothing
    void set_origin(Point origin_) override {}
    //returns the name of the object we're following
    std::string get_name() override {
        return followed_object;
    }
    //Outputs information about what is being drawn
    //before drawing
    void draw() override;
private:
    //returns the point which should be an origin to center
    //at a given location.
    Point get_Origin_Value(const Point& location);
    
    std::string followed_object;
};

//Info_view holds a single datum about the given
class Info_view: public View
{
public:
    virtual ~Info_view() = 0;//force abstractedness
    //Outputs information about the data given for each object. 
    void draw() override;
    //Clears the object of any given objects
    void clear() override;
    //Removes the given object from the map of objects.
    void update_remove(const std::string& name) override;
protected:
    //Builds an info_view with an internal name for the data
    Info_view(const std::string& name_of_data);
    //Inserts the given pair into the object_data map.
    void insert(const std::string& name, double data);
private:
    std::map<std::string, double> object_data;
    std::string data_name;
};

class Health_view: public Info_view
{
public:
    //constructs Info_view with the given type of data
    Health_view();
    //Updates the health for the given object via. calling insert
    void update_health(const std::string& name, double health) override;
    //Returns the internal representation of the view
    std::string get_name() override {
        return "health";
    }
};

class Amount_view: public Info_view
{
public:
    //Updates the amount given in object_data with new amount
    void update_amount(const std::string& name, double amount) override;
    //Constructs Info_view with the given type of data
    Amount_view();
    //Returns the internal representation of the view's name
    std::string get_name() override {
        return "amounts";
    }
};


#endif
