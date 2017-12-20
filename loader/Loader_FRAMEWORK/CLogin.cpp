#include "main.h"

CGlobals g_Globals;

std::string CLogin::getCurrentTime()
{
	//formt H-M:S
	time_t now = time(0);
	struct tm timestruct;
	char buf[256];
	timestruct = *localtime(&now);	
	strftime(buf, sizeof(buf),/*%M*/XorStr<0xBA, 3, 0xAB3FD5CA>("\x9F\xF6" + 0xAB3FD5CA).s, &timestruct);

	return buf;
}

void CLogin::GetMACAddress(std::string &sOut)
{
	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBufLen = sizeof(AdapterInfo);

	char *pszMacAddy = new char[256];

	GetAdaptersInfo(AdapterInfo, &dwBufLen);

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;

	do
	{
		sprintf(pszMacAddy, /*%02X:%02X:%02X:%02X:%02X:%02X*/XorStr<0xCD, 30, 0x4BB1266F>("\xE8\xFE\xFD\x88\xEB\xF7\xE3\xE6\x8D\xEC\xF2\xE8\xEB\x82\xE1\xF9\xED\xEC\x87\xDA\xC4\xD2\xD1\xBC\xDF\xC3\xD7\xDA\xB1" + 0x4BB1266F).s,
			pAdapterInfo->Address[0], pAdapterInfo->Address[1],
			pAdapterInfo->Address[2], pAdapterInfo->Address[3],
			pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

		sOut = pszMacAddy;
		delete[] pszMacAddy;
		return;

		pAdapterInfo = pAdapterInfo->Next;

	} while (pAdapterInfo);
}

/////////////////////////////////////////////
// Processes the login 
//! param - PCHAR pszUsername
//! param - PCHAR pszPassword
//! ret - VOID
// TODO: Whole function lel
/////////////////////////////////////////////
VOID CLogin::processLogin(char* pszUsername, char* pszPassword)
{	
	std::string cryptedTime = Crypt::encrypt(getCurrentTime(), aes_key, aes_iv);
	
	HW_PROFILE_INFO hwProfileInfo;

	//Hide api call
	GetCurrentHwProfileHidden(&hwProfileInfo);	
	
	std::string sMAC;
	GetMACAddress(sMAC);	

	char request[512];
	sprintf(request, "/login.php?action=login&username=%s&password=%s" , pszUsername, pszPassword);

	g_Winsock.InitWinsock();
	g_Winsock.Connect("Example.com"); //change
	CString cstResponse;
	g_Winsock.SendHttpGet(request, cstResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();
	
	
	std::string sRet((LPCTSTR)cstResponse);	

	std::cout << sRet << std::endl;

	if (strcmp(sRet.c_str(),/*error*/XorStr<0x34, 6, 0x4DA3BC70>("\x51\x47\x44\x58\x4A" + 0x4DA3BC70).s) == 0)
	{
		m_login.bSuccess = FALSE;
		return;
	}


	char hrequest[512];
	sprintf(hrequest, "/hwid.php?username=%s&hwid=%s", pszUsername, sMAC.c_str());

	g_Winsock.InitWinsock();
	g_Winsock.Connect("Example.com"); //change
	CString hcstResponse;
	g_Winsock.SendHttpGet(hrequest, hcstResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();

	std::string hsRet((LPCTSTR)hcstResponse);

	std::cout << hsRet << std::endl;

	if (strcmp(hsRet.c_str(),/*hwiderror*/XorStr<0x79, 10, 0x2F105E98>("\x11\x0D\x12\x18\x18\x0C\x0D\xEF\xF3" + 0x2F105E98).s) == 0)
	{
		MessageBoxHidden(0,/*hwiderror*/XorStr<0x79, 10, 0x2F105E98>("\x11\x0D\x12\x18\x18\x0C\x0D\xEF\xF3" + 0x2F105E98).s,/*Error*/XorStr<0x66, 6, 0x9F5A486C>("\x23\x15\x1A\x06\x18" + 0x9F5A486C).s, 0);
		ExitProcessHidden(0);
		int* p = 0;
		*p = 0;
	}

	char grequest[512];
	sprintf(grequest, "/group.php?username=%s", pszUsername);

	g_Winsock.InitWinsock();
	g_Winsock.Connect("Example.com"); //change
	CString gcstResponse;
	g_Winsock.SendHttpGet(grequest, gcstResponse);
	g_Winsock.Disconnect();
	g_Winsock.CleanUp();

	std::string sgroup((LPCTSTR)gcstResponse);

	std::cout << sgroup << std::endl;

	std::string sDecrypted = Crypt::decrypt(sRet, aes_key, aes_iv);

	std::string sGames = sDecrypted;


	if (strcmp(sgroup.c_str(), "group number to allow login") == 0) //change
	if (strstr(sGames.c_str(), "csgo"))
		m_login.bSuccess = TRUE;	
	
	if (strcmp(sgroup.c_str(), "group number to allow login") == 0) //change
	if (strstr(sGames.c_str(), "csgo"))
	{
		m_login.bSuccess = TRUE;
		m_login.bCSGO = TRUE;
	}
	
}

login CLogin::getSession()
{
	return m_login;
}


BOOL CLogin::IsOutdated()
{
	return FALSE;
}

BOOL CLogin::isValidInput(char* pszInput)
{
	//Empty?
	if (strlen(pszInput) == 0)
		return FALSE;

	//Got space?
	for (unsigned int ax = 0; ax < strlen(pszInput); ax++)
	{
		if (pszInput[ax] == char(32))
			return FALSE;
	}

	return TRUE;
}

VOID addComboEntry(HWND hCombo, LPCTSTR entry)
{
	SendMessageHidden(hCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCTSTR)entry));
}

VOID CLogin::initComboBox(HWND hCombo)
{
		if (m_login.bCSGO == TRUE)
		addComboEntry(hCombo, /*CS:GO*/XorStr<0xC0, 6, 0x2F0C09F0>("\x83\x92\xF8\x84\x8B" + 0x2F0C09F0).s);
}