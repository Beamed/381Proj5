#include "Views.h"
#include "Utility.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <iterator>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::ios;
using std::setw;
using std::list;
using std::map;
using std::ostream_iterator;

const int min_map_size_c = 7;
const int max_map_size_c = 30;
const double lower_scale_bound_c = 0.0;
const char* const empty_tile_c = ". ";
const char* const multiple_objs_in_tile_c = "* ";
const int num_chars_per_tile_c = 2;
const int axis_print_frequency_c = 3;
const int axis_precision_c = 0;
const int local_map_size_c = 9;
const double local_map_scale_c = 2.0;

//constructs tile_view with given parameters
Tile_view::Tile_view(int size_, double scale_,
                     double origin_x, double origin_y):
size(size_), scale(scale_), origin(origin_x, origin_y)
{
}
//empty dtor to enforce abstractedness
Tile_view::~Tile_view()
{
    
}

//Updates or creates the location of any object w/the given name
void Tile_view::update_location(const string &name, Point location)
{
    objects[name] = location;//just overwrite old one; otherwise
    //ignore. easy-peasy!
}
//removes the given name from the map of objects
//if no object matches, does nothing.
void Tile_view::update_remove(const string &name)
{
    objects.erase(name);
}

//draws the map as specified to stdout
void Tile_view::draw()
{
    draw_map(gen_map());
}

vector<vector<string>> Tile_view::gen_map()
{
    vector<vector<string>> map(size, vector<string>(size, empty_tile_c));
    //create a 30x30 vector of strings initialized to the default empty tile
    for(const auto &object_pair: objects) {
        int x, y;//x and y location of object
        if(get_subscripts(x, y, object_pair.second)) {
            if(map[y][x] == empty_tile_c)
                map[y][x] = object_pair.first.substr(0, num_chars_per_tile_c);
            //0 as 0 is the start of the string.
            else//otherwise use our default "multiple objects" val:
                map[y][x] = multiple_objs_in_tile_c;
        }
    }
    return map;//move semantics make this return not very
}//costly at all!

void Tile_view::draw_map(const vector<vector<string>>& map)
{
    for(int y = 0; y < size; y++) {
        int axis_val = size - y - 1;
        //since we're printing in opposite order, invert index(size - y)
        //and account for our y being off by one.
        if(axis_val % axis_print_frequency_c == 0) {
            auto old_settings = cout.precision();//save old settings
            cout << std::fixed<< std::setprecision(axis_precision_c);
            cout << setw(4) << get_axis_label(axis_val, origin.y) << " ";
            cout.precision(old_settings);//restore old settings
        }
        else {
            cout << "     ";
        }
        const vector<string>& line = map[axis_val];
        ostream_iterator<string> out_iter(cout);
        copy(line.begin(), line.end(), out_iter);
        cout << endl;//new line after every individual line.
    }//we have now printed each row, now to print the x axis
    for(int x = 0; x < size; x++) {
        if(x%axis_print_frequency_c == 0) {
            auto old_settings = cout.precision();
            cout << std::fixed <<std::setprecision(axis_precision_c);
            cout << "  " << setw(4) << get_axis_label(x, origin.x);
            cout.precision(old_settings);
        }
    }
    cout << endl;//phew; done!
}

//Performs the equation which returns the expected value to be printed
//as an axis label.
double Tile_view::get_axis_label(int coord, double origin_mod)
{
    return coord * scale + origin_mod;
}

// Calculate the cell subscripts corresponding to the supplied location parameter,
// using the current size, scale, and origin of the display.
// This function assumes that origin is a  member variable of type Point,
// scale is a double value, and size is an integer for the number of rows/columns
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool Tile_view::get_subscripts(int &ix, int &iy, Point location)
{
    // adjust with origin and scale
    Cartesian_vector subscripts = (location - origin) / scale;
    // truncate coordinates to integer after taking the floor
    // floor function will return the largest integer smaller than the supplied value
    // even for negative values, so -0.05 => -1., which will be outside the array.
    ix = int(floor(subscripts.delta_x));
    iy = int(floor(subscripts.delta_y));
    // if out of range, return false
    if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
        return false;
    }
    else
        return true;
}

//clears the view of all currently saved objects
void Tile_view::clear()
{
    objects.clear();
}

//sets the size of the view to the given size.
//throws an error if out of bounds.
void Tile_view::set_size(int size_)
{
    if(size_ > max_map_size_c) {
        throw Error{"New map size is too big!"};
    }
    if(size_ < min_map_size_c) {
        throw Error{"New map size is too small!"};
    }
    size = size_;
}

//sets the scale to the given scale number
//Throws an error if it is out of bounds.
void Tile_view::set_scale(double scale_)
{
    if(scale_ <= lower_scale_bound_c) {
        throw Error{"New map scale must be positive!"};
    }
    scale = scale_;
}
//sets the origin equal to the specified origin
void Tile_view::set_origin(Point origin_) {
    origin = origin_;
}
//Outputs information about the current state of the map
//to stdout. 
void Tile_view::describe()
{
    cout << "Display size: " <<
    size << ", scale: " << scale << ", origin: " <<
    origin << endl;
}

//Returns a list of all objects currently outside of the grid
list<string> Tile_view::get_outside_objects()
{
    list<string> outside_objects;
    for(auto obj_pair_iter = objects.begin(); obj_pair_iter != objects.end();
        obj_pair_iter ++ ) {
        int garbagex, garbagey;//no need for the returned values here
        if(!get_subscripts(garbagex, garbagey, obj_pair_iter->second)) {
            outside_objects.push_back(obj_pair_iter->first);
        }
    }
    return outside_objects;
}

//Returns an object of the given name. Assumes that one exists.
Point Tile_view::get_object_location(const string& name)
{
    return objects.find(name)->second;
}


//initializes map view and underlying tile_view with default settings
Map_view::Map_view() :
Tile_view()
{
}
//restores parameters to the default values
void Map_view::set_defaults()
{
    set_size(default_size_c);
    set_scale(default_scale_c);
    set_origin(Point{default_origin_x_c, default_origin_y_c});
}
//Outputs current information about the map, any users who are currently
//outside the map, and then the map.
void Map_view::draw()
{
    describe();
    list<string> outside_objs = get_outside_objects();
    if(!outside_objs.empty()) {
        for(const string& obj : outside_objs) {
            cout << obj;
            if(obj != outside_objs.back()) {
                cout << ", ";
            }
        }
        cout << " outside the map" << endl;
    }
    Tile_view::draw();
}
//Returns a string representation of this view
string Map_view::get_name()
{
    return map_view_name_c;
}
//Constructs a tile view with the given "set" values of a local view's
//parameters,
Local_view::Local_view(const string& name) :
Tile_view(local_map_size_c, local_map_scale_c), followed_object(name)
{
    Tile_view::set_origin(get_Origin_Value(get_object_location(name)));
}
//returns the point from which the map's origin should be
//if it is to be centered on the given location
Point Local_view::get_Origin_Value(const Point& location)
{
    double new_x_coord =
        location.x - (local_map_size_c / 2.) * local_map_scale_c;
    double new_y_coord =
        location.y - (local_map_size_c / 2.) * local_map_scale_c;
    return Point{new_x_coord, new_y_coord};
}
//Updates the location of the given object. If the object is
//the one we are currently following, update the origin as well.
void Local_view::update_location(const string& name, Point location)
{
    Tile_view::update_location(name, location);
    if(name == followed_object) {
        Tile_view::set_origin(get_Origin_Value(get_object_location(name)));
    }
}

void Local_view::draw()
{
    cout << "Local view for: " << followed_object << endl;
    Tile_view::draw();
}
//Constructs an instance of this with the given label of output
Info_view::Info_view(const std::string& name_of_data)  :
data_name(name_of_data)
{
}
//empty destructor to enforce abstractedness
Info_view::~Info_view()
{
}
//Outputs information about the data given for each object
void Info_view::draw()
{
    cout << "Current " << data_name << ":" << endl;
    cout << "--------------" << endl;
    for(auto map_iter = object_data.begin(); map_iter != object_data.end();
        map_iter++) {
        cout << map_iter->first << ": " << map_iter->second << endl;
    }
    cout << "--------------" << endl;
}
//Updates the information for the given object
void Info_view::insert(const std::string &name, double data)
{
    object_data[name] = data;
}
//Clears the map of all objects
void Info_view::clear()
{
    object_data.clear();
}
//Removes the given object from the map
void Info_view::update_remove(const string &name)
{
    object_data.erase(name);
}
//Constructs Health_view by notifying base class of what info it contains
Health_view::Health_view() : Info_view("Health")
{}
//Updates the health of the given object
void Health_view::update_health(const string &name, double health)
{
    insert(name, health);
}
//Returns a string representation of this view
string Health_view::get_name()
{
    return health_view_name_c;
}
//Constructs Info_view by notifying base class it contains info on amounts
Amount_view::Amount_view() : Info_view("Amounts")
{}
//Calls insert to update the amount given
void Amount_view::update_amount(const std::string &name, double amount)
{
    insert(name, amount);
}
//Returns a string representation of this view
string Amount_view::get_name()
{
    return amounts_view_name_c;
}
