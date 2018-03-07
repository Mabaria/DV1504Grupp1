#pragma once
#include <ctime>
#include <string>

class Timer
{
public:
	Timer();
	Timer(time_t startTime);
	~Timer();
	const std::string WhenTimerStarted() const;
	const std::string WhenTimerStoped();
	void StartTimer();
	const int GetSeconds();
	const int GetMinutes();
	const int GetHours();
	const int GetDays();
	const long GetTimeAsSecondsSinceStart();
	const std::string GetTimeAsStr();
	const std::string GetSecondsAsStr();
	const std::string GetMinutesAsStr();
	const std::string GetHoursAsStr();
	const time_t GetTimeData();
private:
	time_t mStartTime;
	tm *pStartTimeStruct;
	time_t mTimeNow;
	tm *pTimeNowStruct;
	void UpdateTime();

	// How the struct looks like
	//struct tm { 
	//	int tm_sec;   // seconds of minutes from 0 to 61
	//	int tm_min;   // minutes of hour from 0 to 59
	//	int tm_hour;  // hours of day from 0 to 24
	//	int tm_mday;  // day of month from 1 to 31
	//	int tm_mon;   // month of year from 0 to 11
	//	int tm_year;  // year since 1900
	//	int tm_wday;  // days since sunday
	//	int tm_yday;  // days since January 1st
	//	int tm_isdst; // hours of daylight savings time
	//}

};