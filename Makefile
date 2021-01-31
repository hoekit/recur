
recur: lib/libhelpers.a recur.l recur.y
	flex -l recur.l
	bison -dv recur.y
	gcc -o recur recur.tab.c lex.yy.c -Llib -lfl -lhelpers -lbase
	rm -f lex.yy.c recur.output recur.tab.h recur.tab.c

src/is_leap_year.o: src/is_leap_year.c
	cd src && gcc -c is_leap_year.c

lib/libbase.a: src/is_leap_year.o
	ar rs lib/libbase.a src/is_leap_year.o

src/shift-one-char.o: src/shift-one-char.c
	cd src && gcc -c shift-one-char.c

src/eom_days.o: lib/libbase.a src/eom_days.c
	cd src && gcc -c eom_days.c -I.. -L../lib -lbase

src/mth_days.o: lib/libbase.a src/mth_days.c
	cd src && gcc -c mth_days.c -I.. -L../lib -lbase

src/next_mday_days.o: src/mth_days.o src/next_mday_days.c
	cd src && gcc -c next_mday_days.c -I .. -L../lib -lbase

helperfiles=src/shift-one-char.o src/mth_days.o src/next_mday_days.o src/eom_days.o
lib/libhelpers.a: $(helperfiles)
	ar rs lib/libhelpers.a ${helperfiles}

test: t/10_helpers.c lib/libhelpers.a lib/libbase.a
	cd t && gcc -o t_helper 10_helpers.c -I.. -L../lib -lhelpers -lbase
	t/t_helper

clean:
	rm -f lex.yy.c recur.output recur.tab.h recur.tab.c
	rm -f src/*.o
	rm -f lib/*.a
	rm -f t/t_helper

