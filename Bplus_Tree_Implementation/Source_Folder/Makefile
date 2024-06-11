all : Store_Management.o CustomerInvoice.o BPlus.o Map.o
	gcc Store_Management.o CustomerInvoice.o BPlus.o Map.o -o final.exe

CustomerInvoice.o : CustomerInvoice.c 
	gcc -c CustomerInvoice.c 

Store_Management.o : Store_Management.c 
	gcc -c Store_Management.c 

BPlus.o : BPlus.c
	gcc -c BPlus.c

Map.o : Map.c
	gcc -c Map.c

clean : 
	del *.o *.exe 