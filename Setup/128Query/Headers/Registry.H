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

