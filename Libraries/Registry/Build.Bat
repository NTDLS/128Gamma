@Echo Off

c:\lcc\bin\lcc -o -unused Registry.C
c:\lcc\bin\lcclnk.exe -s -dll /Out:Registry.Dll -subsystem:windows Registry.obj Registry.Res

del *.obj
pause


