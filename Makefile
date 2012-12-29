CC=wcc
#CFLAGS=-ecc -bc -d3 -3 -hw -ml
CFLAGS=-zq -ecc -bc -ox -5 -ml -d_DOS
LINKER=wlink
#LFLAGS = option quiet,stack=8192 debug watcom 
LFLAGS = option quiet,stack=8192  

OBJS=qf.obj compress.obj qfllf.obj qfprint.obj qfdos.obj wildcard.obj

all: qf.exe

.c.obj: .autodepend
        $(CC) $CFLAGS $<

.c: .autodepend

qf.obj : qf.c 

qfdos.obj: qfdos.c

wildcard.obj: wildcard.c 

qfprint.obj: qfprint.c 

compress.obj: compress.c

qfllf.obj: qfllf.8
        a86 +C -S QFLLF.8 TO QFLLF.OBJ 

qf.exe: $(OBJS)
        $(LINKER) $(LFLAGS) name $@ file *.obj

clean: .symbolic
   del *.obj
   del *.exe

