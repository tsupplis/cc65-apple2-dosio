0  REM     06/06/83100  TEXT : HOME :D$ =  CHR$ (4): PRINT D$"CATALOG":B =  PEEK (37) - 2: IF B > 22 THEN B = 22110 T = 0:CH = 4: FOR CV = 5 TO 23: GOSUB 1000: IF C <  > 160 THEN  POKE P - 1,219: POKE P,T + 193: POKE P + 1,221:T =T + 1:S = CV120  NEXT CV: VTAB 24:A$ = "TYPE LETTER TO RUN, OR LOAD=1 LOCK=2 UNLOCK=3 DELETE=4 EXIT=5...."130 B$ = "RUN": HTAB 1: PRINT  LEFT$ (A$,39);:A$ =  MID$ (A$,2) + LEFT$ (A$,1):K =  PEEK ( -16384): IF K < 128 THEN  FOR K = 1 TO 75: NEXT K:K =  FRE (0): GOTO 130140  POKE  - 16368,0:K = K - 176: IF K < 1 OR K > 5 THEN 300200  HTAB 1: CALL  - 868: IF K = 5 THEN  END210  PRINT "PRESS 'LETTER' YOU WISH TO ";: IF K = 1 THEN B$ ="LOAD"220  IF K = 2 THEN B$ = "LOCK"230  IF K = 3 THEN B$ = "UNLOCK"240  IF K = 4 THEN B$ = "DELETE": FLASH250  PRINT B$;: CALL  - 198: NORMAL: GET K$:K =  ASC (K$) - 48300  IF K < 17 OR K > T + 16 THEN 130310 CH = 1:CV = S - T + K - 16: GOSUB 1000: IF C = 194 AND (B$ = "RUN" OR B$ = "LOAD") THEN B$ = "B" + B$320  FOR CH = 6 TO 39: GOSUB 1000: B$ = B$ +  CHR$ (C): NEXT CH: HTAB 1: CALL  - 868: PRINT B$: PRINT D$;B$: GOTO 1001000 C1 =  INT (CV / 8):C2 = CV - C1 * 8:P = 1024 + 128 * C2 + 40 * C1 + CH:C =  PEEK (P): RETURN
