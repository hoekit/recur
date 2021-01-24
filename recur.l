%{
#include <stdio.h>
#include <string.h>
#include "recur.tab.h"
void showError();
%}


uday        ([1-7])


%%
"u"             {return (UEEK);}
{uday}          {yylval.uday = atoi(yytext); return (UDAY);}
","             {return (SEP);}
.               {showError(); return(OTHER);}
%%


void showError(){
    printf("Other input");
}
