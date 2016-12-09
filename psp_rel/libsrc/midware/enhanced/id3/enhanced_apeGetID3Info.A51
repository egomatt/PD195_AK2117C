#include <actos.h> 

EXTRN ECODE(sApeGetID3Info??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public ApeGetID3Info??
ApeGetID3Info??:
MOV	WR8,#WORD0(sApeGetID3Info??)
MOV	R10, #BYTE2(sApeGetID3Info??) 
LJMP    RSTMIDAPI 

END