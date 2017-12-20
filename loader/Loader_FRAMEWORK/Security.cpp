#include "main.h"


inline std::string getUsername()
{
	CHAR pszBufUser[256];
	ULONG ulSize = sizeof(pszBufUser);

	if (!GetUserNameHidden(pszBufUser, (LPDWORD)ulSize))
		return /*error*/XorStr<0xA7, 6, 0xC043E7A3>("\xC2\xDA\xDB\xC5\xD9" + 0xC043E7A3).s;

	return pszBufUser;
}
//=====================================================================================
//ANTI DEBUG GOES HERE

inline BOOL CheckDebugFlags()
{
	NTSTATUS status;
	DWORD NoDebugInherit = 0;

	status = NtQueryInformationProcessHidden(GetCurrentProcessHidden(), 0x1f, &NoDebugInherit, 4, NULL);

	if (status != 0x00000000)
		return FALSE;

	if (NoDebugInherit == FALSE)
		return TRUE;

	return FALSE;
}

inline BOOL CheckDebugObject()
{
	HANDLE hDebugObject = NULL;
	NTSTATUS status;

	status = NtQueryInformationProcessHidden(GetCurrentProcessHidden(), 0x1e, &hDebugObject, 4, NULL);

	if (status != 0x00000000)
		return FALSE;

	if (hDebugObject)
		return TRUE;
	else
		return FALSE;

}

inline BOOL HideThread()
{
	NTSTATUS status;

	status = NtSetInformationThreadHidden(GetCurrentThreadHidden(), 0x11, 0, 0);

	if (status != 0x00000000)
		return false;
	else
		return true;
}

inline BOOL CanOpenCsrss()
{
	HANDLE hCsrss = 0;

	hCsrss = OpenProcessHidden(PROCESS_ALL_ACCESS, FALSE, g_Injection.GetProcessIdByName(/*csrss.exe*/XorStr<0x23, 10, 0x1CABC8F1>("\x40\x57\x57\x55\x54\x06\x4C\x52\x4E" + 0x1CABC8F1).s));

	if (hCsrss != NULL)
	{
		CloseHandleHidden(hCsrss);
		return TRUE;
	}
	else
		return FALSE;

}

inline bool Int2DCheck()
{
	__try
	{
		__asm
		{
			int 0x2d
				xor eax, eax
				add eax, 2
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

inline bool IsDbgPresentPrefixCheck()
{
	__try
	{
		__asm __emit 0xF3 // 0xF3 0x64 disassembles as PREFIX REP:
		__asm __emit 0x64
		__asm __emit 0xF1 // One byte INT 1
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

inline bool checkGlobalFlags() 
{
	DWORD NtGlobalFlags;
	__asm
	{
			mov eax, fs:[30h]
			mov eax, [eax + 68h]
			mov NtGlobalFlags, eax
	}

	if (NtGlobalFlags & 0x70)
		return true;

	return false;
}



inline void checkWireshark()
{
	LPVOID drivers[1024];
	DWORD cbNeeded;
	int cDrivers, i;

	if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))
	{
		TCHAR szDriver[1024];
		cDrivers = cbNeeded / sizeof(drivers[0]);

		for (i = 0; i < cDrivers; i++)
		{
			if (GetDeviceDriverBaseName(drivers[i], szDriver, sizeof(szDriver) / sizeof(szDriver[0])))
			{
				if (strcmp(szDriver, /*npf.sys*/XorStr<0x6B, 8, 0x40100F3F>("\x05\x1C\x0B\x40\x1C\x09\x02" + 0x40100F3F).s) == 0)
				{
					MessageBoxHidden(0, /*Please uninstall Wireshark to use Pastedloader*/XorStr<0xED,47,0x90FA0D6F>("\xBD\x82\x8A\x91\x82\x97\xD3\x81\x9B\x9F\x99\x8B\x8D\x9B\x97\x90\xDD\xA9\x96\x72\x64\x71\x6B\x65\x77\x6D\x27\x7C\x66\x2A\x7E\x7F\x68\x2E\x5F\x71\x62\x66\x76\x70\x79\x79\x76\x7C\x7C\x68"+0x90FA0D6F).s,/*Wireshark*/XorStr<0xEA, 10, 0xD2849127>("\xBD\x82\x9E\x88\x9D\x87\x91\x83\x99" + 0xD2849127).s, 0);
					ExitProcessHidden(0);
				}
			}
		}
	}
}

LONG WINAPI UnhandledExcepFilter(PEXCEPTION_POINTERS pExcepPointers)
{
	SetUnhandledExceptionFilterHidden((LPTOP_LEVEL_EXCEPTION_FILTER)
		pExcepPointers->ContextRecord->Eax);

	pExcepPointers->ContextRecord->Eip += 2;

	return EXCEPTION_CONTINUE_EXECUTION;
}


inline void walkProcessList()
{
	std::vector<std::string> m_processList;
	m_processList.push_back(/*ollydbg.exe*/XorStr<0x03, 12, 0xD6009B1D>("\x6C\x68\x69\x7F\x63\x6A\x6E\x24\x6E\x74\x68" + 0xD6009B1D).s);
	m_processList.push_back(/*wireshark.exe*/XorStr<0x58, 14, 0x9A099587>("\x2F\x30\x28\x3E\x2F\x35\x3F\x2D\x0B\x4F\x07\x1B\x01" + 0x9A099587).s);
	m_processList.push_back(/*lordpe.exe*/XorStr<0xDF, 11, 0xAC8BF493>("\xB3\x8F\x93\x86\x93\x81\xCB\x83\x9F\x8D" + 0xAC8BF493).s);
	m_processList.push_back(/*hookshark.exe*/XorStr<0x7E, 14, 0x32799422>("\x16\x10\xEF\xEA\xF1\xEB\xE5\xF7\xED\xA9\xED\xF1\xEF" + 0x32799422).s);
	m_processList.push_back(/*idag.exe*/XorStr<0x4D, 9, 0x344CC654>("\x24\x2A\x2E\x37\x7F\x37\x2B\x31" + 0x344CC654).s);


	for (unsigned int ax = 0; ax < m_processList.size(); ax++)
	{
		std::string sProcess = m_processList.at(ax);
		if (g_Injection.GetProcessIdByName(sProcess.c_str()) != 0)
		{
			ExitProcessHidden(0);
		}
	}
}

inline void doChecks()
{
	checkWireshark();

	walkProcessList();

	SetUnhandledExceptionFilter(UnhandledExcepFilter);
	__asm{xor eax, eax}
	__asm{div eax}

	if (Int2DCheck() || IsDbgPresentPrefixCheck())
		ExitProcessHidden(0);

	if (CheckDebugObject())
		ExitProcessHidden(0);

	if (CheckDebugFlags())
		ExitProcessHidden(0);

	if (checkGlobalFlags())
		ExitProcessHidden(0);

}
void securityThread(LPARAM lpParam)
{
	while (true)
	{
		doChecks();
	}
}

void initSecurity()
{
	doChecks();
	CreateThreadHidden(NULL, NULL, (LPTHREAD_START_ROUTINE)securityThread, NULL, NULL, NULL);
}