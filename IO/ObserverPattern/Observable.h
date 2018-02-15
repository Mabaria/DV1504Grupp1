#pragma once
#include "Observer.h"
#include <string>
#include <vector>
template <typename TAttribute>
class Observable
{
public:
	Observable()
	{

	}
	~Observable()
	{

	}

	void AddObserver(Observer<TAttribute> *rObserver)
	{
		this->mObserverList.push_back(rObserver);

	}/* Use this to add an observer to the list that will be notified when the
	 observable executes NotifyObservers() */

	bool RemoveObserver(Observer<TAttribute> *rObserver)
	{
		for (unsigned int i = 0; i < this->mObserverList.size(); i++)
		{
			// Remove the observer if it exists in the list
			if (this->mObserverList[i] == rObserver) {
				this->mObserverList.erase(this->mObserverList.begin() + i);
				return true;
			}
		}
		return false;
	}/* Use this if you want to remove an observer for any reason*/

	void NotifyObservers(TAttribute* attribute) 
	{
		typename std::vector<Observer<TAttribute>*>::const_iterator iter;
		for (iter = this->mObserverList.begin();
			iter != this->mObserverList.end();
			++iter)
		{
			if (*iter != 0)
			{
				(*iter)->Update(attribute); // Call update on every observer in the list
			}
		}
	}/* Calling this function will send the string to all subscribers.
	They choose how to handle it.*/


private:
	std::vector<Observer<TAttribute> *> mObserverList;
};