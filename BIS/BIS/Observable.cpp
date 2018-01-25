#include "Observable.h"

void Observable::AddObserver(Observer * rObserver)
{
	this->mObserverList.push_back(rObserver);
}

bool Observable::RemoveObserver(Observer * rObserver)
{
	for (std::vector<Observer*>::const_iterator iter = this->mObserverList.begin();
		iter != this->mObserverList.end();
		++iter)
	{
		if (*iter == rObserver) {

		}
	}
}

void Observable::NotifyObservers(const std::string & event) const
{
	for (std::vector<Observer*>::const_iterator iter = this->mObserverList.begin();
		iter != this->mObserverList.end();
		++iter)
	{
		if (*iter != 0)
		{
			(*iter)->Update(event);
		}
	}
}
