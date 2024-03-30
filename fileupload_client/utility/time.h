#pragma once

#include <ctime>
#include <iostream>
#include <string>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

using std::string;

namespace yazi
{
	namespace utility
	{

		class Time
		{
		 public:
			Time();
			~Time() = default;

			int year() const;
			int month() const;
			int day() const;
			int hour() const;
			int minute() const;
			int second() const;
			int week() const;

			double seconds() const;
			double milliseconds() const;
			double operator-(const Time& other) const;

			static void sleep(int milliseconds);

			// format = "%Y-%m-%d %H:%M:%S"
			string format(const string& format = "%Y-%m-%d %H:%M:%S") const;

			void show() const;

		 private:
			void get_local_time(struct tm* tm, const time_t* ticks);
			void get_time_of_day(struct timeval* tv);

		 private:
			struct tm m_tm = { 0 };
			int m_sec = 0;
			int m_usec = 0;
		};

	} // utility
} // yazi

