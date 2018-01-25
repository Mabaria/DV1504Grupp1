#pragma once
#include <string>

class Observer
{
public:
	Observer() {}
	virtual ~Observer() {}
	virtual void Update(std::string eventString) = 0;



};