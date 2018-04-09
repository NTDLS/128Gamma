@Echo Off

c:\lcc\bin\lcc -o -unused BlowFish.C
c:\lcc\bin\lcclnk.exe -s -dll /Out:BlowFish.Dll -subsystem:windows BlowFish.obj BlowFish.Res

del *.obj
pause


