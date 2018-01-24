#include "Timer.h"

Timer::Timer()
{
	time(&this->mStartTime);
	this->pStartTimeStruct = localtime(&this->mStartTime);
	
}

Timer::~Timer()
{
}

std::string Timer::WhenTimerStarted() const
{	
	// %F YYYY-MM-DD  %X HH:MM:SS
	char buffer[30];
	strftime(buffer, 30, "%F %X", this->pStartTimeStruct); 
	return std::string(buffer);
}

const std::string Timer::WhenTimerStoped()
{
	this->UpdateTime();
	char buffer[30];
	strftime(buffer, 30, "%F %X", this->pTimeNowStruct);
	return std::string(buffer);
}

void Timer::StartTimer()
{
	time(&this->mStartTime);
	this->pStartTimeStruct = localtime(&this->mStartTime);
}

const int Timer::GetSec()
{
	this->UpdateTime();	
	time_t diff = this->mTimeNow - this->mStartTime; //Differance in time
	tm *diffStruct = localtime(&diff); //Differance to localtime to struct
	return diffStruct->tm_sec;
}

const int Timer::GetMin()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *diffStruct = localtime(&diff);
	return diffStruct->tm_min;
}

const int Timer::GetHour()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *diffStruct = localtime(&diff);
	if (diffStruct->tm_hour > 0)
	{
		diffStruct->tm_hour = diffStruct->tm_hour - 1;
	}
	else if (diffStruct->tm_hour == 0)
	{
		diffStruct->tm_hour = 23;
	}
	return diffStruct->tm_hour;
}

const int Timer::GetDay()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *diffStruct = localtime(&diff);
	return diffStruct->tm_yday;
}

const std::string Timer::GetTimeAsStr()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime; 
	tm *diffStruct = localtime(&diff);
	if (diffStruct->tm_hour > 0)
	{
		diffStruct->tm_hour = diffStruct->tm_hour - 1;
	}
	else if (diffStruct->tm_hour == 0)
	{
		diffStruct->tm_hour = 23;
	}
	char buffer[40];
	strftime(buffer, 40, "%X", diffStruct);
	return std::string(buffer);
}

const std::string Timer::GetSecAsStr()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *diffStruct = localtime(&diff);
	char buffer[30];
	strftime(buffer, 30, "%S", diffStruct);
	return std::string(buffer);
}

const std::string Timer::GetMinAsStr()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *diffStruct = localtime(&diff);
	char buffer[30];
	strftime(buffer, 30, "%M", diffStruct);
	return std::string(buffer);
}

const std::string Timer::GetHourAsStr()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *diffStruct = localtime(&diff);
	if (diffStruct->tm_hour > 0)
	{
		diffStruct->tm_hour = diffStruct->tm_hour - 1;
	}
	else if (diffStruct->tm_hour == 0)
	{
		diffStruct->tm_hour = 23;
	}
	char buffer[30];
	strftime(buffer, 30, "%H", diffStruct);
	return std::string(buffer);
}

const std::string Timer::GetDayAsStr()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *diffStruct = localtime(&diff);
	diffStruct->tm_mday = diffStruct->tm_mday - 1;
	char buffer[30];
	strftime(buffer, 30, "%j", diffStruct);
	buffer[2] = buffer[2] - 1;
	return std::string(buffer);
}

void Timer::UpdateTime()
{
	this->mTimeNow = time(0);
	this->pTimeNowStruct = localtime(&this->mTimeNow);
}