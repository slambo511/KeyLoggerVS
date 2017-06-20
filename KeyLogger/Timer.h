//
// Created by Samuel on 19/06/2017.
//

#ifndef KEYLOGGER_TIMER_H
#define KEYLOGGER_TIMER_H

#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;
using namespace this_thread;

class Timer
{
	thread Thread;
	bool Alive = false;
	long CallNumber = -1L;
	long repeat_count = -1L;
	milliseconds interval = milliseconds(0);
	function<void(void)> funct = nullptr;


	void SleepAndRun() const
	{
		sleep_for(interval);
		if (Alive)
			Function()();
	}

	void ThreadFunc()
	{
		if (CallNumber == Infinite)
			while (Alive)
				SleepAndRun();
		else
			while (repeat_count--)
				SleepAndRun();
	}

public:
	static const long Infinite = -1L;

	Timer(){}

	Timer(const function<void(void)> &f) : funct(f){}

	Timer(const function<void(void)> &f, const unsigned long &i, const long repeat = Infinite) : funct(f), interval(milliseconds(i)), CallNumber(repeat) {}

	void Start(bool Async = true)
	{
		if (IsAlive())
			return;
		Alive = true;
		repeat_count = CallNumber;
		if (Async)
			Thread = thread(&Timer::ThreadFunc, this);
		else
			this->ThreadFunc();
	}

	void Stop()
	{
		Alive = false;
		Thread.join();
	}

	void SetFunction(const function<void(void)> &f)
	{
		funct = f;
	}

	bool IsAlive() const { return Alive; }

	void RepeatCount(const long r)
	{
		if (Alive)
			return;
		CallNumber = r;
	}

	long GetLeftCount() const { return repeat_count; }

	long RepeatCount() const { return CallNumber; }

	void SetInterval(const unsigned long &i)
	{
		if (Alive)
			return;
		interval = milliseconds(i);
	}

	unsigned long Iterval() const { return static_cast<long>(interval.count()); }

	const function<void(void)> &Function() const
	{
		return funct;
	}
};

#endif //KEYLOGGER_TIMER_H