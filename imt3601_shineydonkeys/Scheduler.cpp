#include "Scheduler.h"
#include "Logger.h"
#include "Blackboard.h"


Scheduler::Task::Task
(std::function<void()> callback, 
std::chrono::system_clock::time_point timePoint) :
_callback(callback),
_timePoint(timePoint)
{

}

bool Scheduler::Task::operator<(const Task& other) const 
{
	return _timePoint < other._timePoint;
}

bool Scheduler::Task::operator>(const Task& other) const 
{
	return _timePoint > other._timePoint;
}

Scheduler::Scheduler() : Module(SCHEDULER)
{

}

bool Scheduler::initialize() 
{
	LOG_I("Scheduler init");

	return true;
}

void Scheduler::update() 
{
	auto callbacks = Blackboard::getInstance()->getCallbacks(moduleType);
	for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
		(*it)(this);

	if(_prioQueue.size() == 0)
		return;

	auto now = NOW;
	auto top = *_prioQueue.top();

	if(now >= top._timePoint)
	{
		_prioQueue.pop();
		top._callback();
	}
}

void Scheduler::schedule(std::function<void()> callback, std::chrono::system_clock::time_point timePoint)
{
	_prioQueue.push(std::make_unique<Task>(callback, timePoint));
}
