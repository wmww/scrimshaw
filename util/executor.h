#pragma once

#include <mutex>
#include <functional>
#include <vector>

class Executor
{
public:
	Executor() = default;
	Executor(Executor const&) = delete;

	void run(std::function<void()> action);
	void iteration();

private:
	std::mutex mutex;
	std::vector<std::function<void()>> actions;
};
