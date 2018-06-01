#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>

#include "Observer.h"

class Observable
{
public:
	Observable();
	virtual ~Observable();

	void addObserver(Observer* ob);

	void notify();

private:
	std::vector<Observer*> observers;
};

#endif
