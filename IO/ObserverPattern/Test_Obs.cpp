#include "Test_Obs.h"
#include "Observable.h"
#include <iostream>

class TestRoom : public Observable<int>
{
public:
	TestRoom() { this->activeEvent = "Fire"; }
	~TestRoom() {}
	std::string GetActiveEvent() { return this->activeEvent; }
private:
	std::string activeEvent;
};

class EventReporter : public Observer<int> 
{
public:
	EventReporter() {}
	~EventReporter() {}
	void Update(int* attribute)
	{
		if (*attribute == 5)
		{
			std::cout << "THE ELD HAS BRYTIT UT" << std::endl;
			system("pause");
		}
	}
};

void Test_Observer()
{
	EventReporter observer1;
	TestRoom observable1;

	observable1.AddObserver(&observer1); // Add observer1 to observable1's sub list
	//observable1.NotifyObservers(5);

	observable1.RemoveObserver(&observer1);
	//observable1.NotifyObservers(3);

	observable1.AddObserver(&observer1);
	//observable1.NotifyObservers(2);
	// Send event to all subscribers of observable
}
