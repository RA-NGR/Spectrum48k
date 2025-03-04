#include "nirvana+.bas"

   CLS
   DIM spr(0 to 47) AS UBYTE = { 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255,\
 			         0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255,\
				 1, 1, 19, 19, 37, 37, 55, 55,\
				 65, 65, 83, 83, 101, 101, 119, 119 }

    NIRVANAtiles(@spr)
    NIRVANAstart()

    DO
 	FOR tile = 0 TO 30 STEP 2
           NIRVANAhalt()
           NIRVANAdrawT(0, 16, tile)
'           NIRVANAdrawT(0, 16, tile * 8)
'           NIRVANAdrawT(0, 16, tile * 8 + 2)
'           NIRVANAdrawT(0, 16, tile * 8 + 4)
'           NIRVANAdrawT(0, 16, tile * 8 + 6)
        NEXT tile
        WHILE INKEY$<>""
        END WHILE
    LOOP
