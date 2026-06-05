compilador: compilador.y compilador.l ast.c symtab.c codegen.c
	bison -d compilador.y
	flex compilador.l
	gcc -g -o compilador compilador.tab.c lex.yy.c ast.c symtab.c codegen.c -lfl

clean:
	rm -f lex.yy.c compilador compilador.tab.c compilador.tab.h a.out
