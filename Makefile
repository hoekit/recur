default:
	clear
	flex -l recur.l
	bison -dv recur.y
	gcc -c \
		shift-one-char.c \
		is_leap_year.c
	ar rs libhelpers.a \
		shift-one-char.o \
		is_leap_year.o
	gcc -o recur recur.tab.c lex.yy.c -L . -lfl -lhelpers
	rm lex.yy.c recur.output recur.tab.h recur.tab.c

test:
	gcc -c \
		shift-one-char.c \
		is_leap_year.c
	ar rs libhelpers.a \
		shift-one-char.o \
		is_leap_year.o
	gcc -o t/t_helper \
		t/10_helpers.c \
		-I . -L . -lhelpers
	t/t_helper

clean:
	rm \
		shift-one-char.o \
		is_leap_year.o \
		t/t_helper


