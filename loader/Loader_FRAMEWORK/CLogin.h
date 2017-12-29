#pragma once

struct login
{
	BOOL bSuccess;
	BOOL bCSGO;
};

class CLogin
{
public:
	void GetMACAddress(std::string &sOut);
	BOOL isValidInput(char* pszInput);
	std::string getCurrentTime();
	VOID processLogin(char* pszUsername, char* pszPassword);
	VOID initComboBox(HWND hCombo);
	login getSession();
	BOOL IsOutdated();		
private:
	login m_login;			
	std::string aes_key = "Generate key here"; //chttps://aesencryption.net/
	std::string aes_iv = "Generate key here"; //https://aesencryption.net/

}; extern CLogin g_Login;
