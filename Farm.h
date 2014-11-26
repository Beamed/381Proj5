/*
A Farm is a Structure that when updated, increments the amount of food on hand
by the production rate amount.
Food can be withdrawn, but no provision is made for depositing any.
*/
#ifndef FARM_H
#define FARM_H

#include "Structure.h"

class Farm : public Structure {
public:
    //constructs a Farm from the given name and location
	Farm (const std::string& name_, Point location_);
		
	// returns the specified amount, or the remaining amount, whichever is less,
	// and deducts that amount from the amount on hand
	double withdraw(double amount_to_get) override;

	//	update adds the production amount to the stored amount
	void update() override;

	// output information about the current state
	void describe() const override;
	//notify Model about food as well as other structure information
    void broadcast_current_state() override;
private:
    //current amount of food
    double cur_amount;
};
#endif
