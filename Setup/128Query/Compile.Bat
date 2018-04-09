@Echo Off

rem c:\lcc\bin\lrc 128Query.Rc

c:\lcc\bin\lcc -o -unused 128Query.C

c:\lcc\bin\lcclnk -s -subsystem:Windows /Out:128Query.Exe 128Query.obj 128Query.res shell32.lib Registry.Lib Windows.Lib

 del *.obj
rem del *.res

pause