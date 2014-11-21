#include "View.h"
#include "Geometry.h"

//provides an interface to use this function; does nothing by itself
void View::update_location(const std::string &name, Point location)
{}//do nothing
//empty dtor to enforce abstractedness
View::~View()
{
    
}
