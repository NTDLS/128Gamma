#define WIN32_LEAN_AND_MEAN

#if !defined(__CRegistry_H_)
	#define __CRegistry_H_

#include <Windows.H>
#include <Shlwapi.H>
#include <tchar.H>

typedef DWORD (__stdcall* SHDELKEYPROC)(HKEY, LPCTSTR);

int IsWinNTor2K();
int CreateKey(HKEY hKeyRoot, LPCTSTR pszSubKey);
int DeleteKey(HKEY hKeyRoot, LPCTSTR pszSubKey);
int DeleteValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue);
int SetStringValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszString);

#endif

///////////////////////////////////////////////////////////////////////

int IsWinNTor2K()
{
	OSVERSIONINFO os;
	os.dwOSVersionInfoSize=sizeof(os);

	if(GetVersionEx(&os))
	{
		if(os.dwPlatformId == 1)
		{
			if(os.dwMinorVersion == 0)
				return FALSE;
			if(os.dwMinorVersion == 10)
				return FALSE;
		}

		if(os.dwPlatformId==2)
		{
			if(os.dwMajorVersion == 3)
				return TRUE;
			if(os.dwMajorVersion == 4)
				return TRUE;
			if(os.dwMajorVersion == 5)
				return TRUE;
		}
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////

int CreateKey(HKEY hKeyRoot, LPCTSTR pszSubKey)
{
	HKEY hKey;
	DWORD dwFunc;
	LONG  lRet;

	lRet = RegCreateKeyEx(
		hKeyRoot,
		pszSubKey,
		0,
		(LPTSTR)NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		(LPSECURITY_ATTRIBUTES)NULL,
		&hKey,
		&dwFunc
	);

	if(lRet==ERROR_SUCCESS)
	{

		RegCloseKey(hKey);
		hKey = (HKEY)NULL;
		return TRUE;
	}

	SetLastError((DWORD)lRet);
	return FALSE;
}

///////////////////////////////////////////////////////////////////////

int DeleteKey(HKEY hKeyRoot, LPCTSTR pszSubKey)
{
	DWORD dwRet=ERROR_SUCCESS;

	if(IsWinNTor2K())
	{
		if(RegDeleteKey(hKeyRoot, pszSubKey) != ERROR_SUCCESS)
		{

			HINSTANCE hLibInst = LoadLibrary(_T("shlwapi.dll"));

			if(!hLibInst)
			{
//				  printf("ERROR_NO_SHLWAPI_DLL\n");
			}

			#if defined(UNICODE) || defined(_UNICODE)
			SHDELKEYPROC DeleteKeyRecursive = (SHDELKEYPROC)GetProcAddress(hLibInst, "SHDeleteKeyW");
			#else
			SHDELKEYPROC DeleteKeyRecursive = (SHDELKEYPROC)GetProcAddress(hLibInst, "SHDeleteKeyA");
			#endif

			if(!DeleteKeyRecursive)
			{
				FreeLibrary(hLibInst);
			}

			dwRet = DeleteKeyRecursive(hKeyRoot, pszSubKey);

			FreeLibrary(hLibInst);
		}
	}
	else
	{
		dwRet = RegDeleteKey(hKeyRoot, pszSubKey);
	}

	if(dwRet == ERROR_SUCCESS)
		return TRUE;

	SetLastError(dwRet);
	return FALSE;
}

///////////////////////////////////////////////////////////////////////

int DeleteValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue)
{
	HKEY hKey;
	LONG lRes;

	if((lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_SET_VALUE, &hKey))!=ERROR_SUCCESS)
	{
		SetLastError((DWORD)lRes);
		return FALSE;
	}

	lRes = RegDeleteValue(hKey, pszValue);

	RegCloseKey(hKey);

	if(lRes==ERROR_SUCCESS)
		return TRUE;

	SetLastError(lRes);
	return FALSE;
}

int SetStringValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszString)
{
	HKEY  hKey;
	LONG  lRes;
	DWORD dwSize = lstrlen(pszString) * sizeof(TCHAR);

	if((lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_WRITE, &hKey))!=ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return FALSE;
	}

	lRes = RegSetValueEx(hKey, pszValue, 0, REG_SZ, (unsigned char*)pszString, dwSize);

	RegCloseKey(hKey);

	if(lRes!=ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////

