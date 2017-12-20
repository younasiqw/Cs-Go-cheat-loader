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
	std::string aes_key = Your aes_key; //change
	std::string aes_iv = Your aes_iv; //change

}; extern CLogin g_Login;