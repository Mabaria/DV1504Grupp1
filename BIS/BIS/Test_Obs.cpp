#include "Test_Obs.h"
#include "Observable.h"
#include <iostream>

class ConcreteObserver: public Observer 
{
public:
	ConcreteObserver() {}
	~ConcreteObserver() {}
	void Update(std::string eventString)
	{
		// If an observable that this observer is subscribed to sends out the event
		// string "Fire", print it out
		if (eventString.compare(std::string("Fire")) == 0)
		{
			std::cout << "THE ELD HAS BRYTIT UT" << std::endl;
			system("pause");
		}
	}
};

class ConcreteObservable : public Observable
{
public:
	ConcreteObservable() {}
	~ConcreteObservable() {}

};
void Test_Observer()
{
	ConcreteObserver observer1;
	ConcreteObservable observable1;

	observable1.AddObserver(&observer1);

	observable1.NotifyObservers(std::string("Fire"));
	// Send fire event to all subscribers
}
