
%{
#include <stdio.h>
#include <time.h>

int yylex();
int yyerror(char *s);

time_t current_time;
struct tm* local_time;
int next_hh[3] = {-1,-1,-1};
int next_mm[2] = {-1,-1};
int next_tm[2] = {-1,-1};
int next_dy[2] = {-1,-1};

int wday;
int d1,d2,h1;

int md1(int wday);  // Return number of days to next recur given wday
int md2(int wday);  // Return number of days to next next recur given wday
int mh1(int wday);  // Return hour of next recur given wday
int new_d(int d, int md);   // Determine new d given md

%}

%token UVAL HVAL MVAL DVAL YVAL OTHER SEP

%type <uval> UVAL
%type <dval> DVAL
%type <hval> HVAL
%type <mval> MVAL
%type <yval> YVAL

%union{
    int uval;
    int dval;
    int hval;
    int mval;
    char *yval;
}

%%

prog:
  loops {
    printf("loops\n");
  }
;

loops:
        loop | loop loops | loop SEP loops

loop:
        texps rexp {
            // A loop is found here so update the next struct here
            // printf("Found a loop\n");
        }

texps:
        | hexps | hexps mexps

hexps:
        hexp | hexp hexps

hexp:   HVAL {
            printf("Every hour at: %d\n", $1);
        }

mexps:
        mexp | mexp mexps

mexp:   MVAL {
            printf("Every minute at: %d\n", $1);
        }

rexp:
        uexp | dexp | yexp

uexp:
        UVAL {
            printf("Handle uexp: u%d\n", $1);

            wday = $1 - 1;
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
            // printf("md1:%d mh2:%d mh1:%d\n", md1(wday),md2(wday), mh1(wday));
            printf(" d1:%d  d2:%d  h1:%d\n",d1,d2,h1);
        }

dexp:
        DVAL {
            printf("Handle dexp: d%d\n", $1);
        }

yexp:
        YVAL {
            printf("Handle yexp: y%s\n", $1);
        }
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

    // current_time = (time_t)1611380990;  // Mock Sat Jan 23 12:49:39 2021
    current_time = time(NULL);

    local_time = localtime(&current_time);
    printf("Local wday:%d hour:%d\n",local_time->tm_wday,local_time->tm_hour);

    yyparse();
    return 0;
}
