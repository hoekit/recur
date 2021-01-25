%{
#include <stdio.h>
#include <string.h>
#include "recur.tab.h"
void showError();
%}


uday        ([1-7])
hour        (0[0-9]|1[0-9]|2[0-3])


%%
"u"             {return (UEEK);}
{uday}          {yylval.uday = atoi(yytext); return (UDAY);}

"H"             {return (H);}
{hour}          {yylval.hour = atoi(yytext); return (HOUR);}

","             {return (SEP);}
.               {showError(); return(OTHER);}
%%


void showError(){
    printf("Other input");
}
