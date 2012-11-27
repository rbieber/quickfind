del *.obj
nmake /f qf.mak
del *.obj
nmake /f qf2.mak
del qf.exe
ren qf2.exe qf.exe
copy qf.exe c:\bin
