PROJ = QF2
PROJFILE = QF2.MAK
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
LFLAGS_G  =  /NOI /NOD /ST:10240  /BATCH
LFLAGS_D  =  /CO /INC /F /PACKC /PACKD  /PMTYPE:VIO /AL:16
LFLAGS_R  =  /E /F /PACKC /PACKD  /PMTYPE:VIO /AL:16
MAPFILE_D  = NUL
MAPFILE_R  = NUL
CC  = cl
CFLAGS_G  = /AM /W2 /G2 /D__OS2__ /Zp /BATCH
CFLAGS_D  = /qc /Gi$(PROJ).mdt /Zr /Zi /Od
CFLAGS_R  = /Ot /Ol /Og /Oe /Oi /Ow /Gs
LLIBS_R  =  /NOD:MLIBCE MLIBCEP
LLIBS_D  =  /NOD:MLIBCE MLIBCEP
ASM  = masm
AFLAGS_G  = /Mx /T
AFLAGS_D  = /Zi
LLIBS_G  = os2.lib

DEF_FILE  = qf2.def
OBJS  = COMPRESS.obj QF.obj QFDOS.obj QFPRINT.obj QFVFY.obj WILDCARD.obj\
        QFOS2.obj
SBRS  = COMPRESS.sbr QF.sbr QFDOS.sbr QFPRINT.sbr QFVFY.sbr WILDCARD.sbr\
        QFOS2.sbr

all: $(PROJ).exe

.SUFFIXES: .c .sbr .obj

COMPRESS.obj : COMPRESS.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\stdlib.h\
        D:\C600\INCLUDE\dos.h D:\C600\INCLUDE\direct.h D:\C600\INCLUDE\io.h\
        D:\C600\INCLUDE\fcntl.h D:\C600\INCLUDE\string.h\
        D:\C600\INCLUDE\ctype.h D:\C600\INCLUDE\malloc.h\
        D:\C600\INCLUDE\time.h qf.h qfdos.h compress.h wildcard.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\conio.h D:\C600\INCLUDE\os2def.h\
        D:\C600\INCLUDE\bse.h D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h\
        D:\C600\INCLUDE\bsesub.h D:\C600\INCLUDE\bseerr.h\
        D:\C600\INCLUDE\bsedev.h D:\C600\INCLUDE\pmwin.h\
        D:\C600\INCLUDE\pmgpi.h D:\C600\INCLUDE\pmdev.h\
        D:\C600\INCLUDE\pmavio.h D:\C600\INCLUDE\pmpic.h\
        D:\C600\INCLUDE\pmord.h D:\C600\INCLUDE\pmbitmap.h\
        D:\C600\INCLUDE\pmfont.h D:\C600\INCLUDE\pmtypes.h\
        D:\C600\INCLUDE\pmmle.h D:\C600\INCLUDE\pmshl.h\
        D:\C600\INCLUDE\pmerr.h D:\C600\INCLUDE\pmhelp.h

COMPRESS.sbr : COMPRESS.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\stdlib.h\
        D:\C600\INCLUDE\dos.h D:\C600\INCLUDE\direct.h D:\C600\INCLUDE\io.h\
        D:\C600\INCLUDE\fcntl.h D:\C600\INCLUDE\string.h\
        D:\C600\INCLUDE\ctype.h D:\C600\INCLUDE\malloc.h\
        D:\C600\INCLUDE\time.h qf.h qfdos.h compress.h wildcard.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\conio.h D:\C600\INCLUDE\os2def.h\
        D:\C600\INCLUDE\bse.h D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h\
        D:\C600\INCLUDE\bsesub.h D:\C600\INCLUDE\bseerr.h\
        D:\C600\INCLUDE\bsedev.h D:\C600\INCLUDE\pmwin.h\
        D:\C600\INCLUDE\pmgpi.h D:\C600\INCLUDE\pmdev.h\
        D:\C600\INCLUDE\pmavio.h D:\C600\INCLUDE\pmpic.h\
        D:\C600\INCLUDE\pmord.h D:\C600\INCLUDE\pmbitmap.h\
        D:\C600\INCLUDE\pmfont.h D:\C600\INCLUDE\pmtypes.h\
        D:\C600\INCLUDE\pmmle.h D:\C600\INCLUDE\pmshl.h\
        D:\C600\INCLUDE\pmerr.h D:\C600\INCLUDE\pmhelp.h

QF.obj : QF.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\stdlib.h\
        D:\C600\INCLUDE\dos.h D:\C600\INCLUDE\direct.h D:\C600\INCLUDE\io.h\
        D:\C600\INCLUDE\fcntl.h D:\C600\INCLUDE\string.h\
        D:\C600\INCLUDE\ctype.h D:\C600\INCLUDE\malloc.h\
        D:\C600\INCLUDE\memory.h D:\C600\INCLUDE\signal.h\
        D:\C600\INCLUDE\conio.h qf.h qfdos.h compress.h wildcard.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\os2def.h D:\C600\INCLUDE\bse.h\
        D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h D:\C600\INCLUDE\bsesub.h\
        D:\C600\INCLUDE\bseerr.h D:\C600\INCLUDE\bsedev.h\
        D:\C600\INCLUDE\pmwin.h D:\C600\INCLUDE\pmgpi.h\
        D:\C600\INCLUDE\pmdev.h D:\C600\INCLUDE\pmavio.h\
        D:\C600\INCLUDE\pmpic.h D:\C600\INCLUDE\pmord.h\
        D:\C600\INCLUDE\pmbitmap.h D:\C600\INCLUDE\pmfont.h\
        D:\C600\INCLUDE\pmtypes.h D:\C600\INCLUDE\pmmle.h\
        D:\C600\INCLUDE\pmshl.h D:\C600\INCLUDE\pmerr.h\
        D:\C600\INCLUDE\pmhelp.h

QF.sbr : QF.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\stdlib.h\
        D:\C600\INCLUDE\dos.h D:\C600\INCLUDE\direct.h D:\C600\INCLUDE\io.h\
        D:\C600\INCLUDE\fcntl.h D:\C600\INCLUDE\string.h\
        D:\C600\INCLUDE\ctype.h D:\C600\INCLUDE\malloc.h\
        D:\C600\INCLUDE\memory.h D:\C600\INCLUDE\signal.h\
        D:\C600\INCLUDE\conio.h qf.h qfdos.h compress.h wildcard.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\os2def.h D:\C600\INCLUDE\bse.h\
        D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h D:\C600\INCLUDE\bsesub.h\
        D:\C600\INCLUDE\bseerr.h D:\C600\INCLUDE\bsedev.h\
        D:\C600\INCLUDE\pmwin.h D:\C600\INCLUDE\pmgpi.h\
        D:\C600\INCLUDE\pmdev.h D:\C600\INCLUDE\pmavio.h\
        D:\C600\INCLUDE\pmpic.h D:\C600\INCLUDE\pmord.h\
        D:\C600\INCLUDE\pmbitmap.h D:\C600\INCLUDE\pmfont.h\
        D:\C600\INCLUDE\pmtypes.h D:\C600\INCLUDE\pmmle.h\
        D:\C600\INCLUDE\pmshl.h D:\C600\INCLUDE\pmerr.h\
        D:\C600\INCLUDE\pmhelp.h

QFDOS.obj : QFDOS.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\stdlib.h\
        D:\C600\INCLUDE\stdarg.h D:\C600\INCLUDE\conio.h\
        D:\C600\INCLUDE\ctype.h D:\C600\INCLUDE\memory.h qf.h qfdos.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\os2def.h D:\C600\INCLUDE\bse.h\
        D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h D:\C600\INCLUDE\bsesub.h\
        D:\C600\INCLUDE\bseerr.h D:\C600\INCLUDE\bsedev.h\
        D:\C600\INCLUDE\pmwin.h D:\C600\INCLUDE\pmgpi.h\
        D:\C600\INCLUDE\pmdev.h D:\C600\INCLUDE\pmavio.h\
        D:\C600\INCLUDE\pmpic.h D:\C600\INCLUDE\pmord.h\
        D:\C600\INCLUDE\pmbitmap.h D:\C600\INCLUDE\pmfont.h\
        D:\C600\INCLUDE\pmtypes.h D:\C600\INCLUDE\pmmle.h\
        D:\C600\INCLUDE\pmshl.h D:\C600\INCLUDE\pmerr.h\
        D:\C600\INCLUDE\pmhelp.h

QFDOS.sbr : QFDOS.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\stdlib.h\
        D:\C600\INCLUDE\stdarg.h D:\C600\INCLUDE\conio.h\
        D:\C600\INCLUDE\ctype.h D:\C600\INCLUDE\memory.h qf.h qfdos.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\os2def.h D:\C600\INCLUDE\bse.h\
        D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h D:\C600\INCLUDE\bsesub.h\
        D:\C600\INCLUDE\bseerr.h D:\C600\INCLUDE\bsedev.h\
        D:\C600\INCLUDE\pmwin.h D:\C600\INCLUDE\pmgpi.h\
        D:\C600\INCLUDE\pmdev.h D:\C600\INCLUDE\pmavio.h\
        D:\C600\INCLUDE\pmpic.h D:\C600\INCLUDE\pmord.h\
        D:\C600\INCLUDE\pmbitmap.h D:\C600\INCLUDE\pmfont.h\
        D:\C600\INCLUDE\pmtypes.h D:\C600\INCLUDE\pmmle.h\
        D:\C600\INCLUDE\pmshl.h D:\C600\INCLUDE\pmerr.h\
        D:\C600\INCLUDE\pmhelp.h

QFPRINT.obj : QFPRINT.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\stdlib.h\
        D:\C600\INCLUDE\dos.h D:\C600\INCLUDE\string.h\
        D:\C600\INCLUDE\malloc.h D:\C600\INCLUDE\memory.h qf.h qfdos.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\conio.h D:\C600\INCLUDE\os2def.h\
        D:\C600\INCLUDE\bse.h D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h\
        D:\C600\INCLUDE\bsesub.h D:\C600\INCLUDE\bseerr.h\
        D:\C600\INCLUDE\bsedev.h D:\C600\INCLUDE\pmwin.h\
        D:\C600\INCLUDE\pmgpi.h D:\C600\INCLUDE\pmdev.h\
        D:\C600\INCLUDE\pmavio.h D:\C600\INCLUDE\pmpic.h\
        D:\C600\INCLUDE\pmord.h D:\C600\INCLUDE\pmbitmap.h\
        D:\C600\INCLUDE\pmfont.h D:\C600\INCLUDE\pmtypes.h\
        D:\C600\INCLUDE\pmmle.h D:\C600\INCLUDE\pmshl.h\
        D:\C600\INCLUDE\pmerr.h D:\C600\INCLUDE\pmhelp.h

QFPRINT.sbr : QFPRINT.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\stdlib.h\
        D:\C600\INCLUDE\dos.h D:\C600\INCLUDE\string.h\
        D:\C600\INCLUDE\malloc.h D:\C600\INCLUDE\memory.h qf.h qfdos.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\conio.h D:\C600\INCLUDE\os2def.h\
        D:\C600\INCLUDE\bse.h D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h\
        D:\C600\INCLUDE\bsesub.h D:\C600\INCLUDE\bseerr.h\
        D:\C600\INCLUDE\bsedev.h D:\C600\INCLUDE\pmwin.h\
        D:\C600\INCLUDE\pmgpi.h D:\C600\INCLUDE\pmdev.h\
        D:\C600\INCLUDE\pmavio.h D:\C600\INCLUDE\pmpic.h\
        D:\C600\INCLUDE\pmord.h D:\C600\INCLUDE\pmbitmap.h\
        D:\C600\INCLUDE\pmfont.h D:\C600\INCLUDE\pmtypes.h\
        D:\C600\INCLUDE\pmmle.h D:\C600\INCLUDE\pmshl.h\
        D:\C600\INCLUDE\pmerr.h D:\C600\INCLUDE\pmhelp.h

QFVFY.obj : QFVFY.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\io.h\
        D:\C600\INCLUDE\fcntl.h D:\C600\INCLUDE\sys\types.h\
        D:\C600\INCLUDE\sys\stat.h D:\C600\INCLUDE\malloc.h\
        D:\C600\INCLUDE\memory.h D:\C600\INCLUDE\string.h qf.h qfdos.h crc.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\conio.h D:\C600\INCLUDE\os2def.h\
        D:\C600\INCLUDE\bse.h D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h\
        D:\C600\INCLUDE\bsesub.h D:\C600\INCLUDE\bseerr.h\
        D:\C600\INCLUDE\bsedev.h D:\C600\INCLUDE\pmwin.h\
        D:\C600\INCLUDE\pmgpi.h D:\C600\INCLUDE\pmdev.h\
        D:\C600\INCLUDE\pmavio.h D:\C600\INCLUDE\pmpic.h\
        D:\C600\INCLUDE\pmord.h D:\C600\INCLUDE\pmbitmap.h\
        D:\C600\INCLUDE\pmfont.h D:\C600\INCLUDE\pmtypes.h\
        D:\C600\INCLUDE\pmmle.h D:\C600\INCLUDE\pmshl.h\
        D:\C600\INCLUDE\pmerr.h D:\C600\INCLUDE\pmhelp.h

QFVFY.sbr : QFVFY.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\io.h\
        D:\C600\INCLUDE\fcntl.h D:\C600\INCLUDE\sys\types.h\
        D:\C600\INCLUDE\sys\stat.h D:\C600\INCLUDE\malloc.h\
        D:\C600\INCLUDE\memory.h D:\C600\INCLUDE\string.h qf.h qfdos.h crc.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\conio.h D:\C600\INCLUDE\os2def.h\
        D:\C600\INCLUDE\bse.h D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h\
        D:\C600\INCLUDE\bsesub.h D:\C600\INCLUDE\bseerr.h\
        D:\C600\INCLUDE\bsedev.h D:\C600\INCLUDE\pmwin.h\
        D:\C600\INCLUDE\pmgpi.h D:\C600\INCLUDE\pmdev.h\
        D:\C600\INCLUDE\pmavio.h D:\C600\INCLUDE\pmpic.h\
        D:\C600\INCLUDE\pmord.h D:\C600\INCLUDE\pmbitmap.h\
        D:\C600\INCLUDE\pmfont.h D:\C600\INCLUDE\pmtypes.h\
        D:\C600\INCLUDE\pmmle.h D:\C600\INCLUDE\pmshl.h\
        D:\C600\INCLUDE\pmerr.h D:\C600\INCLUDE\pmhelp.h

WILDCARD.obj : WILDCARD.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\stdlib.h\
        D:\C600\INCLUDE\string.h D:\C600\INCLUDE\ctype.h qf.h wildcard.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\conio.h D:\C600\INCLUDE\os2def.h\
        D:\C600\INCLUDE\bse.h D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h\
        D:\C600\INCLUDE\bsesub.h D:\C600\INCLUDE\bseerr.h\
        D:\C600\INCLUDE\bsedev.h D:\C600\INCLUDE\pmwin.h\
        D:\C600\INCLUDE\pmgpi.h D:\C600\INCLUDE\pmdev.h\
        D:\C600\INCLUDE\pmavio.h D:\C600\INCLUDE\pmpic.h\
        D:\C600\INCLUDE\pmord.h D:\C600\INCLUDE\pmbitmap.h\
        D:\C600\INCLUDE\pmfont.h D:\C600\INCLUDE\pmtypes.h\
        D:\C600\INCLUDE\pmmle.h D:\C600\INCLUDE\pmshl.h\
        D:\C600\INCLUDE\pmerr.h D:\C600\INCLUDE\pmhelp.h

WILDCARD.sbr : WILDCARD.C D:\C600\INCLUDE\stdio.h D:\C600\INCLUDE\stdlib.h\
        D:\C600\INCLUDE\string.h D:\C600\INCLUDE\ctype.h qf.h wildcard.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\conio.h D:\C600\INCLUDE\os2def.h\
        D:\C600\INCLUDE\bse.h D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h\
        D:\C600\INCLUDE\bsesub.h D:\C600\INCLUDE\bseerr.h\
        D:\C600\INCLUDE\bsedev.h D:\C600\INCLUDE\pmwin.h\
        D:\C600\INCLUDE\pmgpi.h D:\C600\INCLUDE\pmdev.h\
        D:\C600\INCLUDE\pmavio.h D:\C600\INCLUDE\pmpic.h\
        D:\C600\INCLUDE\pmord.h D:\C600\INCLUDE\pmbitmap.h\
        D:\C600\INCLUDE\pmfont.h D:\C600\INCLUDE\pmtypes.h\
        D:\C600\INCLUDE\pmmle.h D:\C600\INCLUDE\pmshl.h\
        D:\C600\INCLUDE\pmerr.h D:\C600\INCLUDE\pmhelp.h

QFOS2.obj : QFOS2.C qf.h D:\C600\INCLUDE\dos.h D:\C600\INCLUDE\string.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\conio.h D:\C600\INCLUDE\os2def.h\
        D:\C600\INCLUDE\bse.h D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h\
        D:\C600\INCLUDE\bsesub.h D:\C600\INCLUDE\bseerr.h\
        D:\C600\INCLUDE\bsedev.h D:\C600\INCLUDE\pmwin.h\
        D:\C600\INCLUDE\pmgpi.h D:\C600\INCLUDE\pmdev.h\
        D:\C600\INCLUDE\pmavio.h D:\C600\INCLUDE\pmpic.h\
        D:\C600\INCLUDE\pmord.h D:\C600\INCLUDE\pmbitmap.h\
        D:\C600\INCLUDE\pmfont.h D:\C600\INCLUDE\pmtypes.h\
        D:\C600\INCLUDE\pmmle.h D:\C600\INCLUDE\pmshl.h\
        D:\C600\INCLUDE\pmerr.h D:\C600\INCLUDE\pmhelp.h

QFOS2.sbr : QFOS2.C qf.h D:\C600\INCLUDE\dos.h D:\C600\INCLUDE\string.h\
        D:\C600\INCLUDE\os2.h D:\C600\INCLUDE\conio.h D:\C600\INCLUDE\os2def.h\
        D:\C600\INCLUDE\bse.h D:\C600\INCLUDE\pm.h D:\C600\INCLUDE\bsedos.h\
        D:\C600\INCLUDE\bsesub.h D:\C600\INCLUDE\bseerr.h\
        D:\C600\INCLUDE\bsedev.h D:\C600\INCLUDE\pmwin.h\
        D:\C600\INCLUDE\pmgpi.h D:\C600\INCLUDE\pmdev.h\
        D:\C600\INCLUDE\pmavio.h D:\C600\INCLUDE\pmpic.h\
        D:\C600\INCLUDE\pmord.h D:\C600\INCLUDE\pmbitmap.h\
        D:\C600\INCLUDE\pmfont.h D:\C600\INCLUDE\pmtypes.h\
        D:\C600\INCLUDE\pmmle.h D:\C600\INCLUDE\pmshl.h\
        D:\C600\INCLUDE\pmerr.h D:\C600\INCLUDE\pmhelp.h


$(PROJ).bsc : $(SBRS)
        $(PWBRMAKE) @<<
$(BRFLAGS) $(SBRS)
<<

$(PROJ).exe : $(DEF_FILE) $(OBJS)
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
        CVP $(CVFLAGS) $(PROJ).exe $(RUNFLAGS)
