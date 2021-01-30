
%{
#include <stdio.h>
#include <time.h>
#include "helpers.h"

int yylex();
int yyerror(char *s);

time_t current_time;
struct tm lc[1];     // local time

int next_hh[3] = {-1,-1,-1};
int next_mm[2] =
{
    -1,     // Earliest mm value earlier than or equal to current minute
            //   Valid values: -1,0-59
    -1      // Earliest mm value later than current minute
            //   Valid values: -1,0-59
};
int next_tm[2] =
{
    -1,     // Earliest time point today in seconds.      Valid: -1, 0-86400
    -1      // Earliest time point other days in seconds. Valid: -1, 0-86400
};
int next_dy[2] =
{
    -1,     // Days to next recurrence. If today, 0. Valid: -1, 0-366
    -1      // Days to next, next recurrence.        Valid: -1, 1-731
};

typedef struct { int dy; int tm; } Next;
Next next = { -1, -1 };

// HELPER FUNCTIONS
time_t dt_to_epoch(int dy, int tm); // Compute epoch of given dy and tm
void reval(int *curr, int val);     // Re-evaluate *curr given val
void upd_next_tm(int tval);         // Update next_tm given tval
void status(char *msg);
void iter_upd_next_tm();            // Generate possible tval and
                                    //   iteratively update next_tm

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
    struct epoch { int dy; int tm; } epoch;
}

%%

input:
  loops {
        printf("%ld\n", dt_to_epoch($1.dy,$1.tm));
    }
;

loops:
  loop           { D && printf("loops:1\n"); $$.dy = next.dy; $$.tm = next.tm; }
| loop loops     { D && printf("loops:2\n"); $$.dy = next.dy; $$.tm = next.tm; }
| loop SEP loops { D && printf("loops:3\n"); $$.dy = next.dy; $$.tm = next.tm; }
;

loop:
  texps rexp {
    // A loop is found here so update the next struct here

    Next temp = { -1, -1 };

    if (next_dy[0] == 0) {              // Next recurrence can be today

        if (next_tm[0] == -1) {         // But no valid times
            temp.dy = next_dy[1];       //   So use next, next recurrence
            temp.tm = next_tm[1];       //     At earliest time

        } else {                        // And a time for today exists
            temp.dy = 0;                //   So today is next recurrence
            temp.tm = next_tm[0];       //     At time for today
        }

    } else {                            // Recurrence tomorrow onwards
        temp.dy = next_dy[0];           //   So use first recurrence
        temp.tm = next_tm[1];           //     At earliest time for other days
    }

    D && printf("  temp.dy:%d temp.tm:%d\n", temp.dy, temp.tm);

    if ((next.dy == -1)                 // Update if not initialized
        || (temp.dy < next.dy)          //   or temp day is earlier
        || ((temp.dy == next.dy)        //   or same day
             && (temp.tm < next.tm))) { //      but temp time is earlier
        next.dy = temp.dy;
        next.tm = temp.tm;
    }

    // Reset helper structs
    next_hh[0] = next_hh[1] = next_hh[2] = -1;
    next_mm[0] = next_mm[1] = -1;
    next_tm[0] = next_tm[1] = -1;
    next_dy[0] = next_dy[1] = -1;

    status("On loop");
  }
;

texps:
  %empty {
    D && printf("On empty texps:\n");
    D && printf("  Default to 6am.\n");
    upd_next_tm(6*60*60);
    status("On texps \%empty");
  }
| hexps {
    D && printf("On HEXPS only:\n");
    D && printf("  Default minute to 00\n");
    next_mm[0] = 0;
    iter_upd_next_tm();
    status("On texps hexps");
  }
| hexps mexps {
    D && printf("On HEXPS MEXPS:\n");
    iter_upd_next_tm();
    status("On texps mexps");
  }
;

hexps:
  hexp
| hexp hexps
;

hexp:
  HVAL {
    D && printf("On H%02d:\n", $1);
    if ($1 < lc->tm_hour) {
        D && printf("  H%02d is earlier than current hour:%02d\n", $1, lc->tm_hour);
        D && printf("  Update next_hh[0] from %d to ", next_hh[0]);
        reval( &next_hh[0], $1 );
        D && printf("%d\n", next_hh[0]);
    } else if ($1 == lc->tm_hour) {
        D && printf("  H%02d is same as current hour:%02d\n", $1, lc->tm_hour);
        D && printf("  Update next_hh[1] from %d to ", next_hh[1]);
        reval( &next_hh[1], $1 );
        D && printf("%d\n", next_hh[1]);
    } else {
        D && printf("  H%02d is later than current hour:%02d\n", $1, lc->tm_hour);
        D && printf("  Update next_hh[2] from %d to ", next_hh[2]);
        reval( &next_hh[2], $1 );
        D && printf("%d\n", next_hh[2]);
    }
  }
;

mexps:
  mexp
| mexp mexps
;

mexp:
  MVAL {
    D && printf("On M%02d:\n", $1);
    if ($1 <= lc->tm_min) {
        D && printf("  M%d is earlier than current min:%d\n", $1, lc->tm_min);
        D && printf("  Update next_mm[0] from %d to ", next_mm[0]);
        reval( &next_mm[0], $1 );
        D && printf("%d\n", next_mm[0]);
    } else {
        D && printf("  M%d is later than current min:%d\n", $1, lc->tm_min);
        D && printf("  Update next_mm[1] from %d to ", next_mm[1]);
        reval( &next_mm[1], $1 );
        D && printf("%d\n", next_mm[1]);
    }
  }
;

rexp:
  uexp
| dexp
| yexp
;

uexp:
  UVAL {
    D && printf("On u%d:\n", $1);
    int wday = $1 == 7 ? 0 : $1;
    D && printf("  Given wday:%d and lc->tm_wday:%d\n", wday, lc->tm_wday);
    if (wday == lc->tm_wday) {
        D && printf("  Today is the next occurrence\n");
        reval(&next_dy[1], next_dy[0]);
        reval(&next_dy[0], 0);
    } else {
        D && printf("  Next occurrence on day of week:%d\n", $1);
        reval(&next_dy[1], next_dy[0]);
        reval(&next_dy[0], (wday + 7 - lc->tm_wday) % 7);
    }
    reval(&next_dy[1], next_dy[0] + 7);
    status("On uexp");
  }
;

dexp:
  DVAL {
    D && printf("On d%02d:\n", $1);
    int mday = $1;
    D && printf("  Given mday:%d and lc->tm_mday:%d\n", mday, lc->tm_mday);
    if (mday == lc->tm_mday) {
        D && printf("  Today is the next occurrence\n");
        reval(&next_dy[1], next_dy[0]);
        reval(&next_dy[0], 0);
    } else {
        D && printf("  Next occurrence on day of month:%d\n", $1);
        reval(&next_dy[1], next_dy[0]);
        reval(&next_dy[0], next_mday_days($1,lc));
    }
    reval(&next_dy[1], next_next_mday_days($1,lc));
    status("On dexp");
  }
;

yexp:
  YVAL  { D && printf("Handle yexp: y%s\n", $1); }
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
void reval(int *curr, int val)      // Re-evaluate *curr given val
{
    if ((*curr == -1) || (val < *curr))
        *curr = val;
}
void upd_next_tm(int tval)          // Update next_tm given tval
{
    // curr_tm is elapsed time since midnight of current day in seconds
    int curr_tm = lc->tm_hour*60*60 + lc->tm_min*60 + lc->tm_sec;

    if (tval > curr_tm) {
        D && printf("  tval:%d is AFTER current time:%d\n",
            tval, curr_tm);
        D && printf("    ∴ next recurrence can be today.\n");
        D && printf("  Update next_tm[0] from %d to ", next_tm[0]);
        reval( &next_tm[0], tval );
        D && printf("%d\n", next_tm[0]);
    }

    // Given a tval, always reval next_tm[1]
    D && printf("  Given tval:%d, always reval next_tm[1]:%d\n",
        tval, curr_tm);
    D && printf("  Value of next_tm[1] -- Before:%d ", next_tm[1]);
    reval( &next_tm[1], tval );
    D && printf("After:%d\n", next_tm[1]);
}
void iter_upd_next_tm()             // Generate possible tval and
{                                   //   iteratively update next_tm
    for (int h = 0; h < 3; h++) {
        if (next_hh[h] == -1) continue;
        for (int m = 0; m < 2; m++) {
            if (next_mm[m] == -1) continue;
            upd_next_tm(next_hh[h]*60*60 + next_mm[m]*60);
        }
    }
}

void status(char *msg)
{
    if (!D) return;
    printf("\nSTATUS: %s\n", msg);
    printf("  next_hh: [ %d %d %d ]\n", next_hh[0], next_hh[1], next_hh[2]);
    printf("  next_mm: [ %d %d ]\n",    next_mm[0], next_mm[1]);
    printf("  next_tm: [ %d %d ]\n",    next_tm[0], next_tm[1]);
    printf("  next_dy: [ %d %d ]\n",    next_dy[0], next_dy[1]);
    printf("  next   : [ %d %d ]\n",    next.dy,    next.tm);
    printf("\n");
}


int main()
{
    D && printf("\n");

    // current_time = (time_t)1611380990;  // Mock Sat Jan 23 12:49:39 2021
    current_time = time(NULL);

    // convert current_time to localtime and store into lc
    localtime_r(&current_time,lc);

    D && printf("Today started at: %ld\n", epoch_add_dt(current_time, 0, 0));

    yyparse();
    status("At End");

    return 0;
}
