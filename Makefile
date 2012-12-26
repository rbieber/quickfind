CC=wcc
#CFLAGS=-ecc -bc -d3 -3 -hw -ml
CFLAGS=-zq -ecc -bc -3 -hw -ml -d_DOS
LINKER=wlink
#LFLAGS = option quiet,stack=8192 debug watcom 
LFLAGS = option quiet,stack=8192  

OBJS=qf.obj compress.obj qfvfy.obj qfllf.obj qfprint.obj qfdos.obj wildcard.obj

all: qf.exe

clean:
	del *.obj
	del *.exe
	
.c.obj: .autodepend
        $(CC) $CFLAGS $<

.c: .autodepend

qf.obj : qf.c 

qfdos.obj: qfdos.c

wildcard.obj: wildcard.c 

qfprint.obj: qfprint.c 

qfvfy.obj: qfvfy.c 

compress.obj: compress.c

qfllf.obj: qfllf.8
        a86 +C -S QFLLF.8 TO QFLLF.OBJ 

qf.exe: $(OBJS)
        $(LINKER) $(LFLAGS) name $@ file *.obj

