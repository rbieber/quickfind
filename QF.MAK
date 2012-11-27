PROJ = QF
PROJFILE = QF.MAK
DEBUG = 0

PWBRMAKE  = pwbrmake
NMAKEBSC1  = set
NMAKEBSC2  = nmake
LINKER  = link
ILINK  = ilink
LRF  = echo > NUL
BIND  = bind
RC  = rc
IMPLIB  = implib
LFLAGS_G  =  /NOI /ST:8192  /BATCH
LFLAGS_D  = /CO /INC /FAR /PACKC
LFLAGS_R  = /EXE /FAR /PACKC
MAPFILE_D  = NUL
MAPFILE_R  = NUL
CC  = cl
CFLAGS_G  = /AM /W3 /G2 /D_DOS /BATCH
CFLAGS_D  = /qc /Gi$(PROJ).mdt /Zr /Zi /Od
CFLAGS_R  = /Ot /Oi /Ol /Oe /Og /Gs
LLIBS_R  =  /NOD:MLIBCE MLIBCER
LLIBS_D  =  /NOD:MLIBCE MLIBCER
ASM  = masm
AFLAGS_G  = /Mx /T
AFLAGS_D  = /Zi

OBJS_EXT  = QFLLF.OBJ
OBJS  = $(OBJS_EXT) COMPRESS.obj QF.obj QFDOS.obj QFPRINT.obj QFVFY.obj\
        WILDCARD.obj 
SBRS  = COMPRESS.sbr QF.sbr QFDOS.sbr QFPRINT.sbr QFVFY.sbr WILDCARD.sbr

all: $(PROJ).exe

.SUFFIXES: .c .sbr .obj

COMPRESS.obj : COMPRESS.C

COMPRESS.sbr : COMPRESS.C

QF.obj : QF.C

QF.sbr : QF.C

QFDOS.obj : QFDOS.C

QFDOS.sbr : QFDOS.C

QFPRINT.obj : QFPRINT.C

QFPRINT.sbr : QFPRINT.C

QFVFY.obj : QFVFY.C

QFVFY.sbr : QFVFY.C

WILDCARD.obj : WILDCARD.C

WILDCARD.sbr : WILDCARD.C

QFLLF.OBJ: QFLLF.8
	A86 +C -S QFLLF.8 TO QFLLF.OBJ
	
$(PROJ).bsc : $(SBRS)
        $(PWBRMAKE) @<<
$(BRFLAGS) $(SBRS)
<<

$(PROJ).exe : $(OBJS)
!IF $(DEBUG)
        $(LRF) @<<$(PROJ).lrf
$(RT_OBJS: = +^
) $(OBJS: = +^
)
$@
$(MAPFILE_D)
$(LLIBS_G: = +^
) +
$(LLIBS_D: = +^
) +
$(LIBS: = +^
)
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_D);
<<
!ELSE
        $(LRF) @<<$(PROJ).lrf
$(RT_OBJS: = +^
) $(OBJS: = +^
)
$@
$(MAPFILE_R)
$(LLIBS_G: = +^
) +
$(LLIBS_R: = +^
) +
$(LIBS: = +^
)
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_R);
<<
!ENDIF
!IF $(DEBUG)
        $(ILINK) -a -e "$(LINKER) @$(PROJ).lrf" $@
!ELSE
        $(LINKER) @$(PROJ).lrf
!ENDIF


.c.sbr :
!IF $(DEBUG)
        $(CC) /Zs $(CFLAGS_G) $(CFLAGS_D) /FR$@ $<
!ELSE
        $(CC) /Zs $(CFLAGS_G) $(CFLAGS_R) /FR$@ $<
!ENDIF

.c.obj :
!IF $(DEBUG)
        $(CC) /c $(CFLAGS_G) $(CFLAGS_D) /Fo$@ $<
!ELSE
        $(CC) /c $(CFLAGS_G) $(CFLAGS_R) /Fo$@ $<
!ENDIF


run: $(PROJ).exe
        $(PROJ).exe $(RUNFLAGS)

debug: $(PROJ).exe
        CV $(CVFLAGS) $(PROJ).exe $(RUNFLAGS)
