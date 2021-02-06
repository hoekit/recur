#include <stdio.h>      /* printf   */
#include <stdlib.h>     /* atoi     */
#include "recur.h"
#include "_private.h"

#ifndef D
#define D 0
#endif


// FLEX & BISON STRUCTS
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yyparse();
extern int yylex();


// INTERNAL VARIABLES
static time_t current_time;
static time_t recur_time = -1;
static struct tm lc[1];     // local time

static int next_hh[3] = {-1,-1,-1};
static int next_mm[2] =
{
    -1,     // Earliest mm value earlier than or equal to current minute
            //   Valid values: -1,0-59
    -1      // Earliest mm value later than current minute
            //   Valid values: -1,0-59
};
static int next_tm[2] =
{
    -1,     // Earliest time point today in seconds.      Valid: -1, 0-86400
    -1      // Earliest time point other days in seconds. Valid: -1, 0-86400
};
static int next_dy[2] =
{
    -1,     // Days to next recurrence. If today, 0. Valid: -1, 0-366
    -1      // Days to next, next recurrence.        Valid: -1, 1-731
};
static int reset_count = 0;

typedef struct { int dy; int tm; } Next;
static Next next = { -1, -1 };


// SYMBOL HANDLERS
static void on_loop();
static void on_texps_empty();
static void on_texps_hexps();
static void on_texps_hexps_mexps();
static void on_hexp(int hval);
static void on_mexp(int mval);
static void on_uexp(int uval);
static void on_wexp(int wval);
static void _on_uexp_wexp(int val);
static void on_dexp(int dval);
static void on_yexp(char *yval);


// HELPER FUNCTIONS
static void say(char *str);                 // Debug-aware printf
static time_t dt_to_epoch(int dy, int tm);  // Compute epoch of given dy and tm
static void reval(int *curr, int val);      // Re-evaluate *curr given val
static void reval_t(time_t *curr, time_t val);
                                            // Re-evaluate *curr given val
static void upd_next_tm(int tval);          // Update next_tm given tval
static void status(char *msg);
static void iter_upd_next_tm();             // Generate possible tval and
                                            //   iteratively update next_tm
static void reset();                        // Reset helper structs


// SYMBOL HANDLERS
static void on_loop()
{
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

    recur_time = dt_to_epoch(next.dy,next.tm);

    status("On loop");
}
static void on_texps_empty()
{
    say("On empty texps:\n");

    if ((next_tm[0] == -1) && (next_tm[1] == -1)) {
        say("  Default to 6am.\n");
        upd_next_tm(6*60*60);
    } else {
        say("  Don't default to 6am. next_tm has values.\n");
    }
    status("On texps \%empty");
}
static void on_texps_hexps()
{
    say("On texps: hexps\n");
    say("  Default minute to 00\n");
    next_mm[0] = 0;
    iter_upd_next_tm();
    status("On texps: hexps");
}
static void on_texps_hexps_mexps()
{
    say("On texps: hexps mexps\n");
    iter_upd_next_tm();
    status("On texps: hexps mexps");
}
static void on_hexp(int hval)
{
    D && printf("On H%02d:\n", hval);
    if (hval < lc->tm_hour) {
        D && printf("  H%02d is earlier than current hour:%02d\n", hval, lc->tm_hour);
        D && printf("  Update next_hh[0] from %d to ", next_hh[0]);
        reval( &next_hh[0], hval );
        D && printf("%d\n", next_hh[0]);
    } else if (hval == lc->tm_hour) {
        D && printf("  H%02d is same as current hour:%02d\n", hval, lc->tm_hour);
        D && printf("  Update next_hh[1] from %d to ", next_hh[1]);
        reval( &next_hh[1], hval );
        D && printf("%d\n", next_hh[1]);
    } else {
        D && printf("  H%02d is later than current hour:%02d\n", hval, lc->tm_hour);
        D && printf("  Update next_hh[2] from %d to ", next_hh[2]);
        reval( &next_hh[2], hval );
        D && printf("%d\n", next_hh[2]);
    }
}
static void on_mexp(int mval)
{
    D && printf("On M%02d:\n", mval);
    if (mval <= lc->tm_min) {
        D && printf("  M%d is earlier than current min:%d\n", mval, lc->tm_min);
        D && printf("  Update next_mm[0] from %d to ", next_mm[0]);
        reval( &next_mm[0], mval );
        D && printf("%d\n", next_mm[0]);
    } else {
        D && printf("  M%d is later than current min:%d\n", mval, lc->tm_min);
        D && printf("  Update next_mm[1] from %d to ", next_mm[1]);
        reval( &next_mm[1], mval );
        D && printf("%d\n", next_mm[1]);
    }
}
static void on_uexp(int uval)
{
    D && printf("On u%d:\n", uval);
    int wday = uval == 7 ? 0 : uval;
    _on_uexp_wexp(wday);
    status("On uexp");
}
static void on_wexp(int wval)
{
    D && printf("On w%d:\n", wval);
    int wday = wval;
    _on_uexp_wexp(wday);
    status("On wexp");
}
static void _on_uexp_wexp(int wday)
{
    D && printf("  Given wday:%d and lc->tm_wday:%d\n", wday, lc->tm_wday);
    if (wday == lc->tm_wday) {
        if  (next_tm[0] >= 0) {
            D && printf("  Today can be the next occurrence\n");
            reval(&next_dy[1], next_dy[0]);
            reval(&next_dy[0], 0);
        } else {
            D && printf("  Today's not next occurrence. No suitable time.\n");
            reval(&next_dy[1], next_dy[0]);
            reval(&next_dy[0], 7);  // Set to same day next week
        }
    } else {
        D && printf("  Next occurrence on day of week:%d\n", wday);
        reval(&next_dy[1], next_dy[0]);
        reval(&next_dy[0], (wday + 7 - lc->tm_wday) % 7);
    }
    reval(&next_dy[1], next_dy[0] + 7);
}
static void on_dexp(int dval)
{
    D && printf("On d%02d:\n", dval);
    int mday = dval;
    D && printf("  Given mday:%d and lc->tm_mday:%d\n", mday, lc->tm_mday);
    if (mday == lc->tm_mday) {
        if  (next_tm[0] >= 0) {
            D && printf("  Today can be next occurrence\n");
            reval(&next_dy[1], next_dy[0]);
            reval(&next_dy[0], 0);
        } else {
            D && printf("  Today's not next occurrence. No suitable time.\n");
            reval(&next_dy[1], next_dy[0]);
            reval(&next_dy[0], next_next_mday_days(dval,lc));
        }
    } else {
        D && printf("  Next occurrence on day of month:%d\n", dval);
        reval(&next_dy[1], next_dy[0]);
        reval(&next_dy[0], next_mday_days(dval,lc));
    }
    reval(&next_dy[1], next_next_mday_days(dval,lc));
    status("On dexp");
}
static void on_yexp(char *yval)
{
    D && printf("On y%s:\n", yval);
    D && printf("  On yval:%s tm_mon:%d tm_mday:%d\n",
        yval,lc->tm_mon,lc->tm_mday);

    // Read month and day values into mm and dd
    char _mm[3] = "00"; _mm[0] = yval[0]; _mm[1] = yval[1];
    char _dd[3] = "00"; _dd[0] = yval[2]; _dd[1] = yval[3];
    int mm = atoi(_mm);
    int dd = atoi(_dd);
    D && printf("     mm:%d dd:%d\n", mm, dd);

    int n1 = next_yday_days(mm,dd,lc);
    int n2 = next_next_yday_days(mm,dd,lc);

    D && printf("n1: %d\n",n1);
    D && printf("n2: %d\n",n2);

    if (n1 == 0) {
        if  (next_tm[0] >= 0) {
            reval(&next_dy[1], next_dy[0]);
            reval(&next_dy[0], n1);
            reval(&next_dy[1], n2);
        } else {
            reval(&next_dy[1], next_dy[0]);
            reval(&next_dy[0], n2);     // Reval with next next value
            reval(&next_dy[1], n2);
        }
    } else {
        reval(&next_dy[1], next_dy[0]);
        reval(&next_dy[0], n1);
        reval(&next_dy[1], n2);
    }

    status("On yexp");
}


// HELPER FUNCTIONS
static void say(char *str)                 // Debug-aware printf
{
    D && printf("%s",str);
}
static int yyerror(char *s)
{
    D && printf("Syntax Error on line %s\n", s);
    return -1;
}
static time_t epoch_add_dt(time_t epoch, int dy, int tm)
{
    // Create localtime struct from current_time
    struct tm *lc_time = localtime(&epoch);

    // Force lc_time to start of today
    lc_time->tm_sec = lc_time->tm_min = lc_time->tm_hour = 0;

    // Convert lc_time back to epoch and add on dy and tm
    return mktime(lc_time) + dy*24*60*60 + tm;
}
static time_t dt_to_epoch(int dy, int tm)  // Compute epoch of given dy and tm
{
    return epoch_add_dt(current_time, dy, tm);
}
static void reval(int *curr, int val)      // Re-evaluate *curr given val
{
    if ((*curr == -1) || (val < *curr))
        *curr = val;
}
static void reval_t(time_t *curr, time_t val)      // Re-evaluate *curr given val
{
    if ((*curr == -1) || (val < *curr))
        *curr = val;
}
static void upd_next_tm(int tval)          // Update next_tm given tval
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
static void iter_upd_next_tm()             // Generate possible tval and
{                                   //   iteratively update next_tm
    for (int h = 0; h < 3; h++) {
        if (next_hh[h] == -1) continue;
        for (int m = 0; m < 2; m++) {
            if (next_mm[m] == -1) continue;
            upd_next_tm(next_hh[h]*60*60 + next_mm[m]*60);
        }
    }
}
static void reset()                        // Reset helper structs
{
    reset_count++;
    D && printf("-- RESET: %d --\n",reset_count);
    next_hh[0] = next_hh[1] = next_hh[2] = -1;
    next_mm[0] = next_mm[1] = -1;
    next_tm[0] = next_tm[1] = -1;
    next_dy[0] = next_dy[1] = -1;
}
static void status(char *msg)
{
    if (!D) return;
    printf("\nSTATUS: %s\n", msg);
    printf("  next_hh: [ %d %d %d ]\n", next_hh[0], next_hh[1], next_hh[2]);
    printf("  next_mm: [ %d %d ]\n",    next_mm[0], next_mm[1]);
    printf("  next_tm: [ %d %d ]\n",    next_tm[0], next_tm[1]);
    printf("  next_dy: [ %d %d ]\n",    next_dy[0], next_dy[1]);
    printf("  next   : [ %d %d ]\n",    next.dy,    next.tm);
    printf("  recur_time: %ld\n",       recur_time);
    printf("\n");
}


time_t recur(char *str)
{
    D && printf("\n");

    // Initialize all variables and structs
    reset(); recur_time = next.dy = next.tm = -1; reset_count = 0;
    status("At Start");

    current_time = time(NULL);

    // convert current_time to localtime and store into lc
    localtime_r(&current_time,lc);

    D && printf("Today started at: %ld\n", epoch_add_dt(current_time, 0, 0));

    YY_BUFFER_STATE bp = yy_scan_string(str);
    yyparse();
    yy_delete_buffer(bp);

    status("At End");

    return recur_time;
}
