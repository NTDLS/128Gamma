@Echo Off

c:\lcc\bin\lcc -o -unused -c -O 128Cmd.C 
c:\lcc\bin\lcclnk -s -subsystem:console -o 128Cmd.Exe 128Cmd.obj Resources\128Cmd.res
del *.obj

pause