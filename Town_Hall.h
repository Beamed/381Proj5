/* 
A Town_Hall is a structure that provides for depositing and withdrawing food,
but does no updating.
*/
#ifndef TOWN_HALL
#define TOWN_HALL

#include <string>
#include "Structure.h"

struct Point;//forward declare

class Town_Hall : public Structure{

public:
    //Constructs a Town Hall with the given name and location
	Town_Hall (const std::string& name_, Point location_);
	
	// deposit adds in the supplied amount
	void deposit(double deposit_amount) override;

	// Return whichever is less, the request or (the amount on hand - 10%) (a "tax"),
	// but amounts less than 1.0 are not supplied - the amount returned is zero.
	// update the amount on hand by subtracting the amount returned.
	double withdraw(double amount_to_obtain) override;

	// output information about the current state
	void describe() const override;
    //broadcasts additional information on the current amount of food
    //it has available
    void broadcast_current_state() override;
private:
    double food;
};
#endif
