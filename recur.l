%{
#include <stdio.h>
#include <string.h>
#include "recur.tab.h"
#include "_private.h"

char val[5] = "";       // Container for values of tokens

void showError();
%}

uval        u([1-7])
hval        H(0[0-9]|1[0-9]|2[0-3])
mval        M([0-5][0-9])
dval        d(0[0-9]|1[0-9]|2[0-9]|3[0-1])
yval        y(0[1-9]|11|12)(0[0-9]|1[0-9]|2[0-9]|3[0-1])

%%
{uval}      {shift(yytext,val); yylval.uval = atoi(val); return UVAL;}
{dval}      {shift(yytext,val); yylval.dval = atoi(val); return DVAL;}
{hval}      {shift(yytext,val); yylval.hval = atoi(val); return HVAL;}
{mval}      {shift(yytext,val); yylval.mval = atoi(val); return MVAL;}
{yval}      {shift(yytext,val); yylval.yval = val;       return YVAL;}

","             {return (SEP);}

.               {showError(); return(OTHER);}
%%

void showError(){
    printf("Other input");
}

