//
// Created by Samuel on 17/06/2017.
//

#ifndef KEYLOGGER_HELPER_H
#define KEYLOGGER_HELPER_H

#include <ctime>
#include <string>
#include <sstream>
#include <ios>
#include <fstream>

using namespace std;

namespace Helper
{
	template <class T>

	string ToString(const T &);

	struct DateTime
	{
		DateTime()
		{
			time_t ms;
			time(&ms);

			struct tm *info = localtime(&ms);

			D = info->tm_mday;
			m = info->tm_mon + 1;
			y = 1900 + info->tm_year;
			M = info->tm_min;
			H = info->tm_hour;
			S = info->tm_sec;
		}

		DateTime(int D, int m, int y, int H, int M, int S) : D(D), m(m), y(y), H(H), M(M), S(S) {}

		DateTime(int D, int m, int y) : D(D), m(m), y(y), H(0), M(0), S(0) {}

		DateTime Now() const
		{
			return DateTime();
		}

		int D, m, y, H, M, S;

		string GetDateString() const
		{
			return string(D < 10 ? "0" : "") + ToString(D) +
				string(m < 10 ? ".0" : ".") + ToString(m) +
				"." + ToString(y);
		}

		string GetTimeString(const string &sep = ":") const
		{
			return string(H < 10 ? "0" : "") + ToString(H) + sep +
				string(M < 10 ? "0" : "") + to_string(M) + sep +
				string(S < 10 ? sep : "") + ToString(S);
		}

		string GetDateTimeString(const string &sep = ":") const
		{
			return GetDateString() + " " + GetTimeString(sep);
		}
	};

	template <class T>

	string ToString(const T &e)
	{
		ostringstream s;
		s << e;
		return s.str();
	}

	void WriteAppLog(const string &s)
	{
		ofstream file("AppLog.txt", ios::app);
		file << "[" << DateTime().GetDateTimeString() << "]"
			<< "\n" << s << endl << "\n";
		file.close();
	}
}

#endif //KEYLOGGER_HELPER_H
