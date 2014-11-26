#include "View.h"
#include "Geometry.h"

//provides an interface to use this function; does nothing by default
//Included here so Point doesn't need to be defined in View.h
void View::update_location(const std::string &name, Point location)
{}//do nothing
//empty dtor to enforce abstractedness
View::~View()
{
    
}
