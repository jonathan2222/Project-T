#include "Observable.h"

Observable::Observable()
{
}

Observable::~Observable()
{
}

void Observable::addObserver(Observer * ob)
{
	this->observers.push_back(ob);
}

void Observable::notify()
{
	for (Observer* ob : this->observers)
		ob->update();
}
