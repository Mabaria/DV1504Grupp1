#include "Observable.h"

Observable::Observable()
{
}

Observable::~Observable()
{
}

void Observable::AddObserver(Observer * rObserver)
{
	this->mObserverList.push_back(rObserver);
}

bool Observable::RemoveObserver(Observer * rObserver)
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
}

void Observable::NotifyObservers(const std::string & event) const
{
	for (std::vector<Observer*>::const_iterator iter = this->mObserverList.begin();
		iter != this->mObserverList.end();
		++iter)
	{
		if (*iter != 0)
		{
			(*iter)->Update(event); // Call update on every observer in the list
		}
	}
}
