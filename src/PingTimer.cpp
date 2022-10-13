#include "PingTimer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

PingTimer::PingTimer()
{
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
	timesToPing = mustPingTimes;
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
