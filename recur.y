
%{
#include <stdio.h>
#include <time.h>

int yylex();
int yyerror(char *s);

time_t current_time;
struct tm* local_time;
int wday;
int d1,d2,h1;

int md1(int wday);  // Return number of days to next recur given wday
int md2(int wday);  // Return number of days to next next recur given wday

%}

%token UEEK UDAY OTHER SEP

%type <uday> UDAY

%union{
    int uday;
}

%%

prog:
  loops
;

loops:
        | loop loops SEP loops

loop:
        UEEK UDAY {
            wday = $2 - 1;
            switch(wday) {
                case 0:
                    printf("Every Monday\n");
                    break;
                case 1:
                    printf("Every Tuesday\n");
                    break;
                case 2:
                    printf("Every Wednesday\n");
                    break;
                case 3:
                    printf("Every Thursday\n");
                    break;
                case 4:
                    printf("Every Friday\n");
                    break;
                case 5:
                    printf("Every Saturday\n");
                    break;
                case 6:
                    printf("Every Sunday\n");
                    break;
                yyerror("1");
            }
            printf("md1: %d\n",md1(wday));
            printf("md2: %d\n",md2(wday));
        }
        | OTHER
;

%%

int yyerror(char *s)
{
    printf("Syntax Error on line %s\n", s);
    return 0;
}
int md1(int wday)   // Return number of days to next recur given wday
{
    return (wday + 8 - local_time->tm_wday) % 7;
}
int md2(int wday)   // Return number of days to next next recur given wday
{
    return md1(wday) + 7;
}


int main()
{
    // Initialize d1, d2 and h1
    d1 = d2 = h1 = -1;

    current_time = (time_t)1611415429;  // Mock Sat Jan 23 22:25:02 2021
    // current_time = time(NULL);

    local_time = localtime(&current_time);
    printf("Local wday: %d\n", local_time->tm_wday);

    yyparse();
    return 0;
}
