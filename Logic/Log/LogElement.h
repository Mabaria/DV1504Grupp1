#pragma once

class LogElement
{
public:
	LogElement();
	~LogElement();

	virtual int GetID() const = 0;
private:
};