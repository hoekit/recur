
recurfiles = recur.l recur.y
default: helpers $(recurfiles)
	clear
	flex -l recur.l
	bison -dv recur.y
	gcc -o recur recur.tab.c lex.yy.c -L . -lfl -lhelpers
	rm lex.yy.c recur.output recur.tab.h recur.tab.c

srcfiles = shift-one-char.c is_leap_year.c
source: $(srcfiles)
	gcc -c \
		shift-one-char.c \
		is_leap_year.c
	ar rs libhelpers.a \
		shift-one-char.o \
		is_leap_year.o

helperfiles = src/eom_days.c src/mth_days.c
helpers: source $(helperfiles)
	cd src && gcc -c eom_days.c -I .. -L .. -lhelpers
	cd src && gcc -c mth_days.c -I .. -L .. -lhelpers
	cd src && gcc -c next_mday_days.c -I .. -L .. -lhelpers
	ar rs libhelpers.a \
		src/eom_days.o \
		src/mth_days.o \
		src/next_mday_days.o

test: helpers
	cd t && gcc -o t_helper 10_helpers.c -I .. -L .. -lhelpers
	t/t_helper

clean:
	rm -f *.o
	rm -f src/*.o
	rm -f libhelpers.a
	rm -f t/t_helper

