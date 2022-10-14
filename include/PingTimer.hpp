#ifndef PINGTIMER_HPP
# define PINGTIMER_HPP

# include <iostream>
# include <string>
# include <sys/time.h>
# include "const.hpp"

class PingTimer
{

	public:

		PingTimer();
		~PingTimer();
		bool isNoResponce();
		void doPing();
		void reset();
		long long getCurrTimeMs();

	private:
		long long lastPingTimeMs;
		int timesToPing;
		long long timeToResponceMs;

};

#endif /* ******************************************************* PINGTIMER_H */