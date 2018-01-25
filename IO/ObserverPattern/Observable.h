#pragma once
#include "Observer.h"
#include <string>
#include <vector>

class Observable
{
public:
	Observable();
	~Observable();

	void AddObserver(Observer *rObserver); /* Use this to add an observer to the
										   list that will be notified when the
										   observable executes NotifyObservers() */

	bool RemoveObserver(Observer *rObserver); /* Use this if you want to remove
											  an observer for any reason*/

	void NotifyObservers(const std::string &event) const; /* Calling this function
														  will send the string
														  to all subscribers.
														  They choose how to 
														  handle it.*/


private:
	std::vector<Observer*> mObserverList;
};