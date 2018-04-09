@Echo Off

Rem C:\Lcc\Bin\Lrc 128Gamma.Rc
C:\Lcc\Bin\Lcc -O -unused 128Gamma.C
C:\Lcc\Bin\Lcclnk -s -subsystem:Windows /Out:128Gamma.Exe 128Gamma.Obj Resources\128Gamma.Res Libraries\Registry\Registry.Lib Libraries\Windows\Windows.Lib Libraries\BlowFish\BlowFish.Lib Shell32.Lib

Del *.Obj
Rem Del *.Res

Pause