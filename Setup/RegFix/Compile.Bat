@Echo Off

rem c:\lcc\bin\lrc Resources/RegFix.Rc
c:\lcc\bin\lcc -O -unused RegFix.C
c:\lcc\bin\lcclnk -s -subsystem:windows /Out:RegFix.Exe RegFix.obj RegFix.Res

del *.obj

pause