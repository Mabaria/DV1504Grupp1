#pragma once
#include <string>

template <typename TAttribute>
class Observer
{
public:
	Observer() {}
	virtual ~Observer() {}
	virtual void Update(const TAttribute* attribute) = 0; /*This function will be called
													  by observables that this
													  observer is subscribed to.
													  This needs to be customized
													  for every class that inherits
													  from Observer.*/



};