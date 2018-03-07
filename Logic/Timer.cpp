#include "../Logic/Timer.h"

Timer::Timer()
{
	time(&this->mStartTime);
	this->pStartTimeStruct = localtime(&this->mStartTime);
	
}

Timer::Timer(time_t startTime)
{
	this->mStartTime = startTime;
	this->pStartTimeStruct = localtime(&this->mStartTime);
}

Timer::~Timer()
{
}

const std::string Timer::WhenTimerStarted() const
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

const int Timer::GetSeconds()
{
	this->UpdateTime();	
	time_t diff = this->mTimeNow - this->mStartTime; //Differance in time
	tm *pdiff_struct = localtime(&diff); //Differance to localtime to struct
	return pdiff_struct->tm_sec;
}

const int Timer::GetMinutes()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *pdiff_struct = localtime(&diff);
	return pdiff_struct->tm_min;
}

const int Timer::GetHours()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *pdiff_struct = localtime(&diff);
	if (pdiff_struct->tm_hour > 0)
	{
		pdiff_struct->tm_hour = pdiff_struct->tm_hour - 1;
	}
	else if (pdiff_struct->tm_hour == 0)
	{
		pdiff_struct->tm_hour = 23;
	}
	return pdiff_struct->tm_hour;
}

const int Timer::GetDays()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *pdiff_struct = localtime(&diff);
	return pdiff_struct->tm_yday;
}

const long Timer::GetTimeAsSecondsSinceStart()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	return (long)diff;
}

const std::string Timer::GetTimeAsStr()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *pdiff_struct = localtime(&diff);
	if (pdiff_struct->tm_hour > 0)
	{
		pdiff_struct->tm_hour = pdiff_struct->tm_hour - 1;
	}
	else if (pdiff_struct->tm_hour == 0)
	{
		pdiff_struct->tm_hour = 23;
	}
	char buffer[40];
	strftime(buffer, 40, "%X", pdiff_struct);
	std::string day_string = std::to_string(this->GetDays()) + ":";
	if (this->GetDays() < 10)
	{
		day_string = "0" + day_string;
	}
	return day_string + std::string(buffer);
}

const std::string Timer::GetSecondsAsStr()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *pdiff_struct = localtime(&diff);
	char buffer[30];
	strftime(buffer, 30, "%S", pdiff_struct);
	return std::string(buffer);
}

const std::string Timer::GetMinutesAsStr()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *pdiff_struct = localtime(&diff);
	char buffer[30];
	strftime(buffer, 30, "%M", pdiff_struct);
	return std::string(buffer);
}

const std::string Timer::GetHoursAsStr()
{
	this->UpdateTime();
	time_t diff = this->mTimeNow - this->mStartTime;
	tm *pdiff_struct = localtime(&diff);
	if (pdiff_struct->tm_hour > 0)
	{
		pdiff_struct->tm_hour = pdiff_struct->tm_hour - 1;
	}
	else if (pdiff_struct->tm_hour == 0)
	{
		pdiff_struct->tm_hour = 23;
	}
	char buffer[30];
	strftime(buffer, 30, "%H", pdiff_struct);
	return std::string(buffer);
}

const time_t Timer::GetTimeData()
{
	return this->mStartTime;
}

void Timer::UpdateTime()
{
	this->mTimeNow = time(0);
	this->pTimeNowStruct = localtime(&this->mTimeNow);
}