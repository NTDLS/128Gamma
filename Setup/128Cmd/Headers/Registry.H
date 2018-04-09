
///////////////////////////////////////////////////////////////////////

#if !defined(__CRegistry_H_)
#define __CRegistry_H_

#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>

typedef DWORD (__stdcall* SHDELKEYPROC)(HKEY, LPCTSTR);

int IsWinNTor2K();

int DeleteValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue);

int CreateKey(HKEY hKeyRoot, LPCTSTR pszSubKey);
int DeleteKey(HKEY hKeyRoot, LPCTSTR pszSubKey);

int SetBinaryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, PVOID pData, DWORD dwSize);
int GetBinaryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, PVOID pData, DWORD& rdwSize);

int SetDWORDValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dwValue);
int GetDWORDValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD& rdwBuff);

int SetStringValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszString);
int GetStringValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, LPTSTR pszBuffer, DWORD& rdwSize);

#endif

///////////////////////////////////////////////////////////////////////

// Returns TRUE if the OS is Windows NT or Windows 2000
int IsWinNTor2K()
{
	OSVERSIONINFO os;
	os.dwOSVersionInfoSize=sizeof(os);

	if(GetVersionEx(&os))
	{
		if(os.dwPlatformId==2)
		{
			if(os.dwMajorVersion == 3) //NT 3
				return TRUE;
			if(os.dwMajorVersion == 4) //NT 4
				return TRUE;
			if(os.dwMajorVersion == 5) //NT 5 (W2K)
				return TRUE;
//			  if(os.dwMajorVersion > 3) For Future Versions
		}
		return FALSE;
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////

// Deletes a value from a given subkey and root
int DeleteValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue)
{
	HKEY hKey;
	LONG lRes;

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_SET_VALUE, &hKey);

	if(lRes != ERROR_SUCCESS)
	{
		SetLastError((DWORD)lRes);
		return FALSE;
	}

	lRes = RegDeleteValue(hKey, pszValue);

	RegCloseKey(hKey);

	if(lRes == ERROR_SUCCESS)
		return TRUE;

	SetLastError(lRes);
	return FALSE;
}

///////////////////////////////////////////////////////////////////////

// Creates a key specified by pszSubKey - you can't create
// keys directly under HKEY_LOCAL_MACHINE in Windows NT or 2000
// just for an extra bit of info.
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

	if(lRet == ERROR_SUCCESS)
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
	DWORD dwRet = ERROR_SUCCESS;

	if(IsWinNTor2K())
	{
		// WinNT/2K will not allow you to delete keys which have
		// subkeys/values inside them. MS's platform SDK tells you
		// to use the SHDeleteKey function in shlwapi.dll. This dll
		// is not available on NT platforms without IE 4.0 or later.
		// Because of this I first attempt to delete the key in the
		// hope that it is empty. If that is not possible I load shlwapi
		// and call the function in that. This prevents the app bombing
		// out if the dll can't be found.
		if(RegDeleteKey(hKeyRoot, pszSubKey) != ERROR_SUCCESS)
		{
			HINSTANCE hLibInst = LoadLibrary(_T("shlwapi.dll"));

			if(!hLibInst)
			{
				//throw ERROR_NO_SHLWAPI_DLL;
			}

			#if defined(UNICODE) || defined(_UNICODE)
			SHDELKEYPROC DeleteKeyRecursive = (SHDELKEYPROC)GetProcAddress(hLibInst, "SHDeleteKeyW");
			#else
			SHDELKEYPROC DeleteKeyRecursive = (SHDELKEYPROC)GetProcAddress(hLibInst, "SHDeleteKeyA");
			#endif

			if(!DeleteKeyRecursive)
			{
				FreeLibrary(hLibInst);
				//throw ERROR_NO_SHDELETEKEY;
			}

			dwRet = DeleteKeyRecursive(hKeyRoot, pszSubKey);

			FreeLibrary(hLibInst);
		}
	}
	else {
		// Windows 9x will allow RegDeleteKey to delete keys
		// even if they have subkeys/values.
		dwRet = RegDeleteKey(hKeyRoot, pszSubKey);
	}

	if(dwRet == ERROR_SUCCESS)
		return TRUE;

	SetLastError(dwRet);
	return FALSE;
}

///////////////////////////////////////////////////////////////////////

// Fetch a binary value. If the size specified by rdwSize is too small, rdwSize will
// be set to the correct size.
int GetBinaryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, PVOID pBuffer, DWORD& rdwSize)
{
	HKEY  hKey;
	DWORD dwType = REG_BINARY;
	DWORD dwSize = rdwSize;
	LONG lRes	= 0;

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_READ, &hKey);

	if(lRes != ERROR_SUCCESS)
	{
		SetLastError((DWORD)lRes);
		return FALSE;
	}

	lRes = RegQueryValueEx(hKey, pszValue, 0, &dwType, (LPBYTE)pBuffer, &dwSize);

	rdwSize = dwSize;
	RegCloseKey(hKey);

	if(lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return FALSE;
	}

	if(dwType != REG_BINARY)
	{
		//throw ERROR_WRONG_TYPE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////

// Fetch a little endian DWORD from the registry
//(see platform SDK "Registry Value Types")
int GetDWORDValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD &rdwBuff)
{
	HKEY hKey;
	DWORD dwType  = REG_DWORD;
	DWORD dwSize  = sizeof(DWORD);
	DWORD dwValue = 0;
	LONG  lRes;

	rdwBuff = 0;

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_READ, &hKey);

	if(lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return FALSE;
	}

	lRes = RegQueryValueEx(hKey, pszValue, 0, &dwType, (LPBYTE)&dwValue, &dwSize);

	RegCloseKey(hKey);

	if(dwType!=REG_DWORD)
		//throw ERROR_WRONG_TYPE;

	if(lRes!=ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return FALSE;
	}

	rdwBuff = dwValue;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////

// Retrieve a string value. If the given buffer for the string is too small (specified
// by rdwSize), rdwSize is increased to the correct value. If the buffer is bigger than
// the retrieved string, rdwSize is set to the length of the string (in bytes) including
// the terminating null.
int GetStringValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, LPTSTR pszBuffer, DWORD& rdwSize)
{
	HKEY  hKey;
	LONG  lRes;
	DWORD dwType = KEY_ALL_ACCESS;

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_READ, &hKey);
	if(lRes != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		SetLastError(lRes);
		return FALSE;
	}

	lRes = RegQueryValueEx(hKey, pszValue, NULL, &dwType, (unsigned char*)pszBuffer, &rdwSize);
	lRes = RegQueryValueEx(hKey, pszValue, NULL, &dwType, (unsigned char*)pszBuffer, &rdwSize);
	if(lRes != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		SetLastError(lRes);
		return FALSE;
	}

	lRes = RegCloseKey(hKey);
	if(lRes != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		SetLastError(lRes);
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////

// Writes a binary value to the registry
int SetBinaryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, PVOID pData, DWORD dwSize)
{
	HKEY hKey;
	LONG lRes = 0;

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_WRITE, &hKey);

	if(lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return FALSE;
	}

	lRes = RegSetValueEx(hKey, pszValue, 0, REG_BINARY, (unsigned char*)pData, dwSize);

	RegCloseKey(hKey);

	if(lRes!=ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////

// Writes a DWORD value to the registry
int SetDWORDValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dwValue)
{
	HKEY hKey;
	LONG lRes;

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_WRITE, &hKey);

	if(lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return FALSE;
	}

	lRes = RegSetValueEx(hKey, pszValue,0,REG_DWORD,(unsigned char*)&dwValue,sizeof(DWORD));

	RegCloseKey(hKey);

	if(lRes!=ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////

// Writes a string to the registry.
int SetStringValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszString)
{
	HKEY  hKey;
	LONG  lRes;
	DWORD dwSize = lstrlen(pszString) * sizeof(TCHAR);

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_WRITE, &hKey);

	if(lRes != ERROR_SUCCESS)
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

