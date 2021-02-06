%{
#include "recur.c"

%}

%token UVAL WVAL HVAL MVAL DVAL YVAL OTHER SEP

%type <uval> UVAL
%type <wval> WVAL
%type <dval> DVAL
%type <hval> HVAL
%type <mval> MVAL
%type <yval> YVAL
%type <epoch> loops loop head tails tail

%union{
    int uval, wval, dval, hval, mval;
    char *yval;
    struct epoch { int dy; int tm; } epoch;
}

%%

input:
  head                  { say("input: head\n"); }
| head tails            { say("input: head tails\n"); }
;

tails:
  SEP tail              { say("tails: SEP tail\n"); }
| tails SEP tail        { say("tails: tails SEP tail\n"); };

head: loops             { say("head\n"); reset(); };
tail: loops             { say("tail\n"); reset(); };

loops:
  loop                  { say("loops: loop\n");       }
| loops loop            { say("loops: loops loop\n"); };

loop    : texps rexp    { on_loop(); } ;

texps   : %empty        { on_texps_empty(); }
        | hexps         { on_texps_hexps(); }
        | hexps mexps   { on_texps_hexps_mexps(); };

hexps   : hexp
        | hexp hexps;

hexp    : HVAL          { on_hexp($1); };

mexps   : mexp
        | mexp mexps;

mexp    : MVAL          { on_mexp($1); };

rexp    : UVAL          { on_uexp($1); }
        | WVAL          { on_wexp($1); }
        | DVAL          { on_dexp($1); }
        | YVAL          { on_yexp($1); };
%%
