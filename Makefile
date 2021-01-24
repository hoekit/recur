default:
	clear
	flex -l recur.l
	bison -dv recur.y 
	gcc -o recur recur.tab.c lex.yy.c -lfl
	rm lex.yy.c recur.output recur.tab.h recur.tab.c
