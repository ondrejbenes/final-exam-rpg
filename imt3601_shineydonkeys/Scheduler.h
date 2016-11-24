#pragma once

#include "Module.h"

#include <functional>
#include <chrono>
#include <queue>
#include <memory>

#define NOW std::chrono::system_clock::now()

class Scheduler : public Module
{
public:
	Scheduler();

	bool initialize() override;
	void update() override;

	void schedule(std::function<void()> callback, std::chrono::system_clock::time_point timePoint);
private:
	class Task
	{
	public:
		Task(std::function<void()> callback, std::chrono::system_clock::time_point timePoint);

		bool operator<(const Task& other) const;
		bool operator>(const Task& other) const;
		std::function<void()> _callback;
		std::chrono::system_clock::time_point _timePoint;
	};

	class TaskLessComparator
	{
	public:
		bool operator() (const std::unique_ptr<Task>& lval, const std::unique_ptr<Task>& rval) const 
		{
			return *lval < *rval;
		}
	};

	class TaskGreaterComparator
	{
	public:
		bool operator() (const std::unique_ptr<Task>& lval, const std::unique_ptr<Task>& rval) const
		{
			return *lval > *rval;
		}
	};

	std::priority_queue<std::unique_ptr<Task>, std::vector<std::unique_ptr<Task>>, TaskGreaterComparator> _prioQueue;
};

