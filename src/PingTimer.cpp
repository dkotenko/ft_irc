#include "PingTimer.hpp"
#include <iostream>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

PingTimer::PingTimer()
{
	timeToResponceMs = PING_TIMEOUT * 1000;
	reset();
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

PingTimer::~PingTimer()
{
}

void PingTimer::reset() {
	
	lastPingTimeMs = getCurrTimeMs();
	timesToPing = PING_TIMES;
}

bool PingTimer::isNeedsPing() {
	std::cout << "needs ping in " << lastPingTimeMs + timeToResponceMs - getCurrTimeMs() << " ms" << std::cout;
	return timesToPing > 0 && lastPingTimeMs + timeToResponceMs < getCurrTimeMs();
}

void PingTimer::doPing() {
	timesToPing--;
	lastPingTimeMs = getCurrTimeMs();
}

long long	PingTimer::getCurrTimeMs(void)
{
	struct timeval	te;
	long long		milliseconds;

	gettimeofday(&te, NULL);
	milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
	return (milliseconds);
}

bool PingTimer::isNoResponce() {
	return timesToPing <= 0 && lastPingTimeMs + timeToResponceMs < getCurrTimeMs();
}

