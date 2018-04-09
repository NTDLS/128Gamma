@Echo Off

c:\lcc\bin\lcc -o -unused Delta.C
c:\lcc\bin\lcclnk.exe -s -dll /Out:Delta.Dll -subsystem:windows Delta.obj Delta.Res

del *.obj
pause


