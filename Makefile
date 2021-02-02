INCL = helpers.h _private.h
LIBS = lib/libhelpers.a lib/libbase.a

recur: ${LIBS} ${INCL} recur.l recur.y
	flex -l recur.l
	bison -dv recur.y
	gcc -o recur recur.tab.c lex.yy.c -Llib -lfl -lhelpers -lbase
	rm -f lex.yy.c recur.output recur.tab.h recur.tab.c

lib/libbase.a: src/is_leap_year.o
	ar rs lib/libbase.a src/is_leap_year.o

helperfiles = src/shift-one-char.o src/mth_days.o \
	src/next_mday_days.o src/next_yday_days.o \
	src/eom_days.o
lib/libhelpers.a: $(helperfiles)
	ar rs lib/libhelpers.a ${helperfiles}

src/%.o: src/%.c
	cd src && gcc -I.. -L../lib -lbase -c ../$^ -o ../$@

test: t/10_helpers.c ${INCL} ${LIBS}
	cd t && gcc -o t_helper 10_helpers.c -I.. -L../lib -lhelpers -lbase
	t/t_helper

clean:
	rm -f lex.yy.c recur.output recur.tab.h recur.tab.c
	rm -f src/*.o
	rm -f lib/*.a
	rm -f t/t_helper

