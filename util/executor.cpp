#include "executor.h"

void Executor::run(std::function<void()> action)
{
	std::lock_guard<std::mutex> lock(mutex);
	actions.push_back(action);
}

void Executor::iteration()
{
	std::vector<std::function<void()>> actions_local;

	{
		std::lock_guard<std::mutex> lock(mutex);
		actions_local = move(actions);
		actions.clear();
	}

	for (auto action : actions_local)
	{
		action();
	}
}
