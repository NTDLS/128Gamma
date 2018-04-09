@Echo Off

c:\lcc\bin\lcc -o -unused Windows.C
c:\lcc\bin\lcclnk.exe -s -dll /Out:Windows.Dll -subsystem:windows Windows.obj Windows.Res

del *.obj
pause


