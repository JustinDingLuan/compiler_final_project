/*************** startsym.h ***************/
#include "sym.h"
char factor_start[symSYMMAX];
char term_start[symSYMMAX];
char expression_start[symSYMMAX];
char condition_start[symSYMMAX];
char statement_start[symSYMMAX];
char block_start[symSYMMAX];

void startsyminit()
{  

   statement_start[symIF] = 1;   
   statement_start[symINT] = 1;
   statement_start[symWHILE] = 1;
   statement_start[symLBRACE] = 1;
   statement_start[symIDENTIFIER] = 1;
   statement_start[symSTRINGDeclare] = 1;
   statement_start[symWRITE] = 1;
   statement_start[symREAD] = 1;
   statement_start[symDO] = 1;   
}
