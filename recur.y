
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
int mh1(int wday);  // Return hour of next recur given wday
int new_d(int d, int md);   // Determine new d given md

%}

%token UEEK UDAY H HOUR OTHER SEP

%type <uday> UDAY
%type <hour> HOUR

%union{
    int uday;
    int hour;
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


            // Update d1, d2 and h1
            d1 = new_d(d1, md1(wday));
            d2 = new_d(d2, md2(wday));
            h1 = new_d(h1, mh1(wday));
            // printf("md1:%d mh2:%d mh1:%d\n", md1(wday),md2(wday), mh1(wday));
            printf(" d1:%d  d2:%d  h1:%d\n",d1,d2,h1);
        }
        | H HOUR {
            printf("Every hour at: %d", $2);
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
int mh1(int wday)   // Return hour of next recur given wday
{
    return 0;
}
int new_d(int d, int md)    // Determine new d given md
{
    if (d == -1) {
        return md;
    } else {
        return d < md ? d : md;
    }
}


int main()
{
    // Initialize d1, d2 and h1
    d1 = d2 = h1 = -1;

    current_time = (time_t)1611380990;  // Mock Sat Jan 23 12:49:39 2021
    // current_time = time(NULL);

    local_time = localtime(&current_time);
    printf("Local wday:%d hour:%d\n",local_time->tm_wday,local_time->tm_hour);

    yyparse();
    return 0;
}
