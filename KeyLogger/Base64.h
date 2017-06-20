//
// Created by Samuel on 18/06/2017.
//

#ifndef KEYLOGGER_BASE64_H
#define KEYLOGGER_BASE64_H

#include <vector>
#include <string>

using namespace std;

namespace Base64
{
	//string base64_encode(const string &);

	const string &BASE64_CODES = "ZYXWVUTSRQPONMLKJIHGFEDCBAabcdefghijklmnopqrstuvwxyz22021978#/";

	string base64_encode(const string &s)
	{
		string ret;
		int val = 0;
		int bits = -6;
		const unsigned int b63 = 0x3F;

		for (const auto &c : s)
		{
			val = (val << 8) + c;
			bits += 8;
			while (bits >= 0)
			{
				ret.push_back(BASE64_CODES[(val >> bits) & b63]);
				bits -= 6;
			}

		}

		if (bits > -6)
			ret.push_back(BASE64_CODES[((val << 8) >> (bits + 8)) & b63]);

		while (ret.size() % 4)
			ret.push_back('=');

		return ret;
	}

	const string &SALT1 = "LB::SB::RW::SB::HB";
	const string &SALT2 = "__//:://__IfItWorks";
	const string &SALT3 = "22021978150119801609199992104201022022017";

	string EncryptB64(string s)
	{
		s = SALT1 + s + SALT2 + SALT3;
		s = base64_encode(s);
		s.insert(11, SALT2);
		s = base64_encode(s);
		s += SALT3;
		s = base64_encode(s);
		s.insert(5, "Q!");
		s.insert(9, "pp$");
		return s;
	}
}

#endif //KEYLOGGER_BASE64_H
