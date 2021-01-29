
%{
#include <stdio.h>
#include <time.h>

int yylex();
int yyerror(char *s);

time_t current_time;
int next_hh[3] = {-1,-1,-1};
int next_mm[2] = {-1,-1};
int next_tm[2] = {-1,-1};
int next_dy[2] = {-1,-1};

time_t dt_to_epoch(int dy, int tm); // Compute epoch of given dy and tm

%}

%token UVAL HVAL MVAL DVAL YVAL OTHER SEP

%type <uval> UVAL
%type <dval> DVAL
%type <hval> HVAL
%type <mval> MVAL
%type <yval> YVAL
%type <epoch> loops loop

%union{
    int uval;
    int dval;
    int hval;
    int mval;
    char *yval;
    struct { int dy; int tm; } epoch;
}

%%

input:
  loops {
        printf("%ld\n", dt_to_epoch($1.dy,$1.tm));
    }
;

loops:
  loop              { $$.dy = 1; $$.tm = 1; }
| loop loops        { $$.dy = 1; $$.tm = 2; }
| loop SEP loops    { $$.dy = 1; $$.tm = 3; }
;

loop:
  texps rexp {
    // A loop is found here so update the next struct here
    // printf("Found a loop\n");
  }
;

texps:
  %empty
| hexps
| hexps mexps
;

hexps:
  hexp
| hexp hexps
;

hexp:
  HVAL  { printf("Every hour at: %d\n", $1); }
;

mexps:
  mexp
| mexp mexps
;

mexp:
  MVAL  { printf("Every minute at: %d\n", $1); }
;

rexp:
  uexp
| dexp
| yexp
;

uexp:
  UVAL  { printf("Handle uexp: u%d\n", $1); }
;

dexp:
  DVAL  { printf("Handle dexp: d%d\n", $1); }
;

yexp:
  YVAL  { printf("Handle yexp: y%s\n", $1); }
;

%%

int yyerror(char *s)
{
    printf("Syntax Error on line %s\n", s);
    return 0;
}



time_t epoch_add_dt(time_t epoch, int dy, int tm)
{
    // Create localtime struct from current_time
    struct tm *lc_time = localtime(&epoch);

    // Force lc_time to start of today
    lc_time->tm_sec = lc_time->tm_min = lc_time->tm_hour = 0;

    // Convert lc_time back to epoch and add on dy and tm
    return mktime(lc_time) + dy*24*60*60 + tm;
}
time_t dt_to_epoch(int dy, int tm)  // Compute epoch of given dy and tm
{
    return epoch_add_dt(current_time, dy, tm);
}


int main()
{
    printf("\n");

    // current_time = (time_t)1611380990;  // Mock Sat Jan 23 12:49:39 2021
    current_time = time(NULL);

    printf("Today started at: %ld\n", epoch_add_dt(current_time, 0, 0));

    yyparse();

    return 0;
}
