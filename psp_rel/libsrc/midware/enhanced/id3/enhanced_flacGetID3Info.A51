#include <actos.h> 

EXTRN ECODE(sflacGetID3Info??)

MIDAPI SEGMENT ECODE

rseg MIDAPI

///////////////////////////////////////////////
public flacGetID3Info??
flacGetID3Info??:
MOV	WR8,#WORD0(sflacGetID3Info??)
MOV	R10, #BYTE2(sflacGetID3Info??) 
LJMP    RSTMIDAPI 

END