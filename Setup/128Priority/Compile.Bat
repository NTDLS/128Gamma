@Echo Off

rem c:\lcc\bin\lrc 128Priority.Rc

c:\lcc\bin\lcc -o -unused 128Priority.C

c:\lcc\bin\lcclnk -s -subsystem:windows /Out:128Priority.Exe 128Priority.obj 128Priority.Res Shell32.Lib Registry.Lib Windows.Lib

 del *.obj
rem del *.res

pause