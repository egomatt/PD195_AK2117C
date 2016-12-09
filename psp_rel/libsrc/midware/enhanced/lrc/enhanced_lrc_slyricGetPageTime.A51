#include <actos.h> 

EXTRN ECODE(slyricGetPageTime??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public lyricGetPageTime??
lyricGetPageTime??:
MOV	WR8,#WORD0(slyricGetPageTime??)
MOV	R10, #BYTE2(slyricGetPageTime??) 
LJMP    RSTMIDAPI 

END