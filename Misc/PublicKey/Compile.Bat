@Echo Off

rem c:\lcc\bin\lrc PublicKey.Rc

c:\lcc\bin\lcc -o -unused PublicKey.C

c:\lcc\bin\lcclnk -s -subsystem:console /Out:PublicKey.Exe PublicKey.obj PublicKey.res Libraries\Delta\Delta.Lib shell32.lib

 del *.obj
rem del *.res

pause