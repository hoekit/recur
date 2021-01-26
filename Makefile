default:
	clear
	flex -l recur.l
	bison -dv recur.y
	gcc -c shift-one-char.c
	ar rs librecur.a shift-one-char.o
	gcc -o recur recur.tab.c lex.yy.c -L . -lfl -lrecur
	rm lex.yy.c recur.output recur.tab.h recur.tab.c
