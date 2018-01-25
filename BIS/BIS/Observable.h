#pragma once
#include "Observer.h"
#include <string>
#include <vector>

class Observable
{
public:

	void AddObserver(Observer *rObserver);
	bool RemoveObserver(Observer *rObserver);
	void NotifyObservers(const std::string &event) const;


private:
	std::vector<Observer*> mObserverList;
};