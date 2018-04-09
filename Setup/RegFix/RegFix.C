#include <Windows.H>
#include <Stdio.H>
#include "Headers/Registry.H"

//////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{

	if(argc > 1)
	{
		if(strcmp(argv[1],"Uninstall") == 0)
		{
			//-- Re-Create "Open With" Key
			CreateKey(HKEY_CLASSES_ROOT,"Unknown\\shell\\openas\\command");
			SetStringValue(HKEY_CLASSES_ROOT,"Unknown\\shell","","openas");
			SetStringValue(HKEY_CLASSES_ROOT,"Unknown\\shell\\openas\\command","","rundll32.exe shell32.dll,OpenAs_RunDLL %1");

			//-- Delete 128Gamma Keys
			DeleteValue(HKEY_CLASSES_ROOT,"*\\Shell","");
			DeleteKey(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption");
			DeleteKey(HKEY_CLASSES_ROOT,"*\\Shell\\openas");
			DeleteKey(HKEY_CLASSES_ROOT,".128");
			DeleteKey(HKEY_CLASSES_ROOT,"128File");
		}
	}
	else{
		MessageBox (NULL, "RegFix Is Used By 128Gamma To Ensure That \r\n Registry Entries Are Removed Upon Un-Installation." ,"RegFix -NetWorkDLS",0 + MB_SYSTEMMODAL);
		return 0;
	}
	return 0;
}

